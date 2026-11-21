#include "Orion/Renderer/AssimpMeshImporter.h"

#include "Orion/Core/Base.h"
#include "Orion/Core/FileSystem/VirtualPath.h"
#include "Orion/Renderer/Internal/AssimpIOSystem.h"
#include "Orion/Renderer/Internal/AssimpUtils.h"
#include "Orion/Renderer/MeshSource.h"

#include <assimp/Importer.hpp>
#include <assimp/material.h>
#include <assimp/mesh.h>
#include <assimp/postprocess.h>
#include <assimp/scene.h>
#include <assimp/types.h>
#include <span>

namespace Orion
{
    namespace
    {
        constexpr unsigned int ImportFlags = aiProcess_Triangulate | aiProcess_JoinIdenticalVertices |
                                             aiProcess_SortByPType | aiProcess_GenSmoothNormals |
                                             aiProcess_CalcTangentSpace | aiProcess_GenUVCoords |
                                             aiProcess_ImproveCacheLocality | aiProcess_ValidateDataStructure;

        void ImportSubmesh(MeshSource& source, const aiMesh* mesh)
        {
            Submesh& submesh      = source.Submeshes.emplace_back();
            submesh.VertexOffset  = static_cast<uint32_t>(source.Vertices.size());
            submesh.FirstIndex    = static_cast<uint32_t>(source.Indices.size());
            submesh.MaterialIndex = mesh->mMaterialIndex;
            submesh.Name          = mesh->mName.C_Str();

            if (!(mesh->mPrimitiveTypes & aiPrimitiveType_TRIANGLE))
                return;

            const auto positions  = Internal::MakeSpan(mesh->mVertices, mesh->mNumVertices);
            const auto normals    = Internal::MakeSpan(mesh->mNormals, mesh->mNumVertices);
            const auto tangents   = Internal::MakeSpan(mesh->mTangents, mesh->mNumVertices);
            const auto bitangents = Internal::MakeSpan(mesh->mBitangents, mesh->mNumVertices);
            const auto uvs        = Internal::MakeSpan(mesh->mTextureCoords[0], mesh->mNumVertices);

            source.Vertices.reserve(source.Vertices.size() + positions.size());
            for (unsigned int i = 0; i < positions.size(); ++i)
            {
                Vertex vertex{};
                vertex.Position = Internal::ToVec3(positions[i]);
                if (!normals.empty())
                    vertex.Normal = Internal::ToVec3(normals[i]);
                if (!tangents.empty() && !bitangents.empty())
                {
                    vertex.Tangent   = Internal::ToVec3(tangents[i]);
                    vertex.Bitangent = Internal::ToVec3(bitangents[i]);
                }
                if (!uvs.empty())
                    vertex.TexCoord = {uvs[i].x, uvs[i].y};
                source.Vertices.push_back(vertex);
            }

            const auto faces = Internal::MakeSpan(mesh->mFaces, mesh->mNumFaces);

            source.Indices.reserve(source.Indices.size() + static_cast<size_t>(mesh->mNumFaces * 3));
            for (const aiFace& face : faces)
            {
                OE_CORE_ASSERT(face.mNumIndices == 3, "We expect a triangle. A face should have 3 faces.");

                const auto indices = Internal::MakeSpan(face.mIndices, 3);
                source.Indices.push_back(indices[0]);
                source.Indices.push_back(indices[1]);
                source.Indices.push_back(indices[2]);
            }

            submesh.VertexCount = mesh->mNumVertices;
            submesh.IndexCount  = mesh->mNumFaces * 3;
        }

        uint32_t ImportNode(MeshSource& source, const aiNode* assimpNode, uint32_t parent)
        {
            const auto index = static_cast<uint32_t>(source.Nodes.size());
            source.Nodes.emplace_back();

            // Use a scope so "meshNode" reference goes out of scope BEFORE the recursion.
            // The recursion may reallocate the vector, and the "meshNode" may be invalid after that.
            {
                MeshNode& meshNode      = source.Nodes[index];
                meshNode.Name           = assimpNode->mName.C_Str();
                meshNode.Parent         = parent;
                meshNode.LocalTransform = Internal::Mat4FromAssimp(assimpNode->mTransformation);
                for (const unsigned int meshIndex : Internal::MakeSpan(assimpNode->mMeshes, assimpNode->mNumMeshes))
                    meshNode.Submeshes.push_back(meshIndex);
            }
            // After this point, we let the recursion do its thing
            // and only access the meshNode safely using the index (source.Nodes[index])

            for (const aiNode* assimpChild : Internal::MakeSpan(assimpNode->mChildren, assimpNode->mNumChildren))
            {
                const uint32_t child = ImportNode(source, assimpChild, index);
                source.Nodes[index].Children.push_back(child);
            }

            return index;
        }

        void ImportEmbeddedTexture(MeshSource& source, const aiScene* scene, const std::string& path)
        {
            if (path.empty())
                return;

            const aiTexture* texture = scene->GetEmbeddedTexture(path.c_str());
            if (!texture)
                return;

            if (texture->mHeight != 0)
            {
                OE_CORE_LOG(Warn, "Embedded texture \"{0}\" is uncompressed: not supported", path);
                return;
            }

            const auto* bytes = reinterpret_cast<const uint8_t*>(texture->pcData);
            const auto data   = Internal::MakeSpan(bytes, texture->mWidth);
            source.EmbeddedTextures[path].assign(data.begin(), data.end());
        }

        Ref<MeshSource> ConvertScene(const aiScene* scene, const std::string& name, const std::string& directory)
        {
            if (!scene->mRootNode || (scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE))
            {
                OE_CORE_LOG(Error, "Imported scene \"{0}\" is incomplete", name);
                return nullptr;
            }

            Ref<MeshSource> source = MakeRef<MeshSource>();
            source->Name           = name;
            source->Directory      = directory;

            // Submeshes keep the SAME order as aiScene::mMeshes: nodes reference them by that index
            source->Submeshes.reserve(scene->mNumMeshes);
            for (const aiMesh* meshes : Internal::MakeSpan(scene->mMeshes, scene->mNumMeshes))
                ImportSubmesh(*source, meshes);

            // Materials: only names and texture paths for now
            source->Materials.reserve(scene->mNumMaterials);
            for (const aiMaterial* material : Internal::MakeSpan(scene->mMaterials, scene->mNumMaterials))
            {
                MeshMaterial& meshMaterial = source->Materials.emplace_back();
                meshMaterial.Name          = material->GetName().C_Str();

                aiString path;
                if (material->GetTexture(aiTextureType_BASE_COLOR, 0, &path) == AI_SUCCESS ||
                    material->GetTexture(aiTextureType_DIFFUSE, 0, &path) == AI_SUCCESS)
                {
                    meshMaterial.AlbedoPath = path.C_Str();
                    ImportEmbeddedTexture(*source, scene, meshMaterial.AlbedoPath);
                }
                if (material->GetTexture(aiTextureType_NORMALS, 0, &path) == AI_SUCCESS)
                {
                    meshMaterial.NormalPath = path.C_Str();
                    ImportEmbeddedTexture(*source, scene, meshMaterial.NormalPath);
                }
            }

            ImportNode(*source, scene->mRootNode, MeshNode::NoParent);
            source->UpdateNodeTransforms();

            OE_CORE_LOG(Info,
                        "Imported \"{0}\": {1} vertices, {2} indices, {3} submeshes, {4} nodes",
                        name,
                        source->Vertices.size(),
                        source->Indices.size(),
                        source->Submeshes.size(),
                        source->Nodes.size());

            for (const MeshMaterial& material : source->Materials)
                OE_CORE_LOG(Info,
                            "Material \"{0}\": albedo=\"{1}\" normal=\"{2}\"",
                            material.Name,
                            material.AlbedoPath,
                            material.NormalPath);
            OE_CORE_LOG(Info,
                        "Directory=\"{0}\", embedded textures: {1}",
                        source->Directory,
                        source->EmbeddedTextures.size());

            return source;
        }
    } // namespace

    Ref<MeshSource> AssimpMeshImporter::Import(const std::string& virtualPath)
    {
        OE_PROFILE_FUNCTION();

        Assimp::Importer importer;
        // NOLINTNEXTLINE(cppcoreguidelines-owning-memory): the Importer takes ownership
        importer.SetIOHandler(new Internal::AssimpIOSystem());
        const aiScene* scene = importer.ReadFile(virtualPath, ImportFlags);
        if (!scene)
        {
            OE_CORE_LOG(Error, "Failed to import \"{0}\": {1}", virtualPath, importer.GetErrorString());
            return nullptr;
        }
        return ConvertScene(scene,
                            std::string(VirtualPath::Filename(virtualPath)),
                            std::string(VirtualPath::Parent(virtualPath)));
    }

    Ref<MeshSource> AssimpMeshImporter::ImportFromMemory(const void* data, size_t size, const char* formatHint)
    {
        OE_PROFILE_FUNCTION();

        Assimp::Importer importer;
        const aiScene* scene = importer.ReadFileFromMemory(data, size, ImportFlags, formatHint);
        if (!scene)
        {
            OE_CORE_LOG(Error, "Failed to import from memory ({0}): {1}", formatHint, importer.GetErrorString());
            return nullptr;
        }
        return ConvertScene(scene, "memory", {});
    }
} // namespace Orion
