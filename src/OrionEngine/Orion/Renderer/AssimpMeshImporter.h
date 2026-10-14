#pragma once

#include "Orion/Core/Core.h"
#include "Orion/Renderer/MeshSource.h"

#include <filesystem>

namespace Orion
{
    class AssimpMeshImporter
    {
    public:
        // Imports a mesh from a file
        // Returns nullptr if the file cannot be imported
        static Ref<MeshSource> Import(const std::filesystem::path& path);

        // Imports a mesh from a buffer in memory. Used for tests (no files or GL context).
        // formatHint is the file extension without the dot ("obj", "fbx")
        static Ref<MeshSource> ImportFromMemory(const void* data, size_t size, const char* formatHint);
    };
} // namespace Orion
