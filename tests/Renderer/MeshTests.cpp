#include "Orion/Renderer/AssimpMeshImporter.h"
#include "Orion/Renderer/AssimpUtils.h"
#include "Orion/Renderer/MeshSource.h"

#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_floating_point.hpp>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <cstddef>
#include <cstring>

namespace
{
    constexpr double Epsilon = 1e-4;
} // namespace

TEST_CASE("Renderer/Mesh/Vertex: the buffer layout matches the memory layout of the struct", "[Renderer][Mesh]")
{
    const Orion::BufferLayout layout = Orion::Vertex::GetLayout();
    const auto& elements             = layout.GetElements();

    REQUIRE(elements.size() == 5);
    CHECK(layout.GetStride() == sizeof(Orion::Vertex));
    CHECK(elements[0].Offset == offsetof(Orion::Vertex, Position));
    CHECK(elements[1].Offset == offsetof(Orion::Vertex, Normal));
    CHECK(elements[2].Offset == offsetof(Orion::Vertex, Tangent));
    CHECK(elements[3].Offset == offsetof(Orion::Vertex, Bitangent));
    CHECK(elements[4].Offset == offsetof(Orion::Vertex, TexCoord));
}

TEST_CASE("Renderer/Mesh/MeshSource: node world transforms accumulate parent * local", "[Renderer][Mesh]")
{
    Orion::MeshSource source;
    source.Nodes.resize(3);

    source.Nodes[0].Parent         = Orion::MeshNode::NoParent;
    source.Nodes[0].LocalTransform = glm::translate(glm::mat4(1.0f), {1.0f, 0.0f, 0.0f});

    source.Nodes[1].Parent         = 0;
    source.Nodes[1].LocalTransform = glm::rotate(glm::mat4(1.0f), glm::radians(90.0f), {0.0f, 1.0f, 0.0f});

    source.Nodes[2].Parent         = 1;
    source.Nodes[2].LocalTransform = glm::translate(glm::mat4(1.0f), {0.0f, 0.0f, 1.0f});

    source.UpdateNodeTransforms();

    // Right to left: (0,0,0) -> (0,0,1) -> rotated 90 deg around Y -> (1,0,0) -> moved by the root -> (2,0,0)
    const glm::dvec3 origin(source.Nodes[2].WorldTransform * glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));

    CHECK_THAT(origin.x, Catch::Matchers::WithinAbs(2.0, Epsilon));
    CHECK_THAT(origin.y, Catch::Matchers::WithinAbs(0.0, Epsilon));
    CHECK_THAT(origin.z, Catch::Matchers::WithinAbs(0.0, Epsilon));
}

TEST_CASE("Renderer/Mesh/AssimpUtils: a row-major aiMatrix4x4 becomes a column-major glm::mat4", "[Renderer][Mesh]")
{
    aiMatrix4x4 m; // identity
    m.a4 = 5.0f;
    m.b4 = 6.0f;
    m.c4 = 7.0f;
    m.a2 = 3.0f; // row 0, column 1

    const glm::mat4 result = Orion::Mat4FromAssimp(m);

    CHECK(result[3] == glm::vec4(5.0f, 6.0f, 7.0f, 1.0f)); // translation = fourth COLUMN in glm
    CHECK(result[1][0] == 3.0f);                           // m[column][row]
}

TEST_CASE("Renderer/Mesh/AssimpMeshImporter: a textured quad becomes one submesh with a tangent space", "[Renderer][Mesh]")
{
    const char* obj = "v -1 -1 0\n"
                      "v  1 -1 0\n"
                      "v  1  1 0\n"
                      "v -1  1 0\n"
                      "vt 0 0\n"
                      "vt 1 0\n"
                      "vt 1 1\n"
                      "vt 0 1\n"
                      "f 1/1 2/2 3/3 4/4";

    Orion::Ref<Orion::MeshSource> source = Orion::AssimpMeshImporter::ImportFromMemory(obj, std::strlen(obj), "obj");
    REQUIRE(source);

    REQUIRE(source->Submeshes.size() == 1);
    CHECK(source->Vertices.size() == 4);
    CHECK(source->Indices.size() == 6);
    CHECK(source->Submeshes[0].IndexCount == 6);

    size_t references = 0;
    for (const Orion::MeshNode& node : source->Nodes)
        references += node.Submeshes.size();
    CHECK(references == 1);

    for (const Orion::Vertex& v : source->Vertices)
    {
        CHECK_THAT(static_cast<double>(v.Normal.z), Catch::Matchers::WithinAbs(1.0, Epsilon));
        CHECK_THAT(static_cast<double>(glm::length(v.Tangent)), Catch::Matchers::WithinAbs(1.0, Epsilon));
        CHECK_THAT(static_cast<double>(glm::dot(v.Normal, v.Tangent)), Catch::Matchers::WithinAbs(0.0, Epsilon));
        CHECK_THAT(static_cast<double>(glm::length(v.Bitangent)), Catch::Matchers::WithinAbs(1.0, Epsilon));
        CHECK_THAT(static_cast<double>(glm::dot(v.Tangent, v.Bitangent)), Catch::Matchers::WithinAbs(0.0, Epsilon));
    }
}
