#pragma once

#include "Orion/Core/Core.h"
#include "Orion/Core/FileSystem/File.h"
#include "Orion/Core/FileSystem/MountSource.h"

#include <optional>
#include <span>
#include <string>
#include <string_view>
#include <vector>

namespace Orion
{
    // Virtual file system. Every engine file access goes through here
    // Mounts are configured at startup and removed at shutdown.
    class FileSystem
    {
    public:
        // Replaces the source if the name is already mounted
        static void Mount(std::string_view name, UniquePtr<IMountSource> source);
        static void Unmount(std::string_view name);
        static void UnmountAll();

        // nullptr / nullopt / false when the path is invalid, the mount does not exist or the file is missing
        [[nodiscard]] static UniquePtr<IFile> Open(std::string_view virtualPath, FileMode mode = FileMode::Read);
        [[nodiscard]] static bool Exists(std::string_view virtualPath);

        [[nodiscard]] static std::optional<std::vector<uint8_t>> ReadBinary(std::string_view virtualPath);
        [[nodiscard]] static std::optional<std::string> ReadText(std::string_view virtualPath);
        static bool WriteBinary(std::string_view virtualPath, std::span<const uint8_t> data);
        static bool WriteText(std::string_view virtualPath, std::string_view text);
    };
} // namespace Orion
