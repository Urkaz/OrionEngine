#include "Orion/Core/FileSystem/DiskMountsource.h"
#include "Orion/Core/FileSystem/FileSystem.h"
#include "Orion/Core/FileSystem/MemoryMountSource.h"

#include <array>
#include <catch2/catch_test_macros.hpp>
#include <filesystem>
#include <vector>

using namespace Orion;

namespace
{
    // Reset FileSystem global state even when a REQUIRE aborts a test
    struct FileSystemGuard
    {
        FileSystemGuard()                                  = default;
        FileSystemGuard(const FileSystemGuard&)            = delete;
        FileSystemGuard& operator=(const FileSystemGuard&) = delete;
        FileSystemGuard(FileSystemGuard&&)                 = delete;
        FileSystemGuard& operator=(FileSystemGuard&&)      = delete;
        ~FileSystemGuard()
        {
            Orion::FileSystem::UnmountAll();
        }
    };

    void MountTestAssets()
    {
        auto source = Orion::CreateScope<Orion::MemoryMountSource>();
        source->AddFile("shaders/Test.glsl", std::string_view("#type vertex"));
        source->AddFile("models/a.bin", std::vector<uint8_t>{1, 2, 3, 4});
        Orion::FileSystem::Mount("assets", std::move(source));
    }
} // namespace

TEST_CASE("Core/FileSystem/MemoryMountSource: Read stops at the end and Seek checks bounds", "[Core][FileSystem]")
{
    MemoryMountSource source;
    source.AddFile("a.bin", std::vector<uint8_t>{10, 20, 30});

    Scope<IFile> file = source.Open("a.bin", FileMode::Read);
    REQUIRE(file);

    std::array<uint8_t, 8> buffer{};
    CHECK(file->Read(buffer.data(), buffer.size()) == 3);
    CHECK(file->Tell() == 3);
    CHECK(file->Read(buffer.data(), buffer.size()) == 0);

    CHECK(file->Seek(1));
    CHECK(file->Read(buffer.data(), 1) == 1);
    CHECK(buffer[0] == 20);
    CHECK_FALSE(file->Seek(4));
}

TEST_CASE("Core/FileSystem/FileSystem: reads files from a mounted source", "[Core][FileSystem]")
{
    FileSystemGuard guard;
    MountTestAssets();

    CHECK(FileSystem::ReadText("assets://shaders/Test.glsl") == "#type vertex");
    CHECK(FileSystem::ReadBinary("assets://models/a.bin") == std::vector<uint8_t>{1, 2, 3, 4});
    CHECK(FileSystem::Exists("assets://models/a.bin"));
}

TEST_CASE("Core/FileSystem/FileSystem: unknown mounts and missing files return nothing", "[Core][FileSystem]")
{
    FileSystemGuard guard;
    MountTestAssets();

    CHECK_FALSE(FileSystem::Exists("textures://a.png"));
    CHECK_FALSE(FileSystem::ReadBinary("assets://models/missing.bin"));
    CHECK(FileSystem::Open("assets://models/missing.bin") == nullptr);
    CHECK_FALSE(FileSystem::ReadText("models/a.bin")); // no mount: not a virtual path
}

TEST_CASE("Core/FileSystem/FileSystem: paths are normalized before reaching the source", "[Core][FileSystem]")
{
    FileSystemGuard guard;
    MountTestAssets();

    CHECK(FileSystem::Exists("assets://shaders\\..\\models\\.\\a.bin"));
    CHECK_FALSE(FileSystem::Exists("assets://../assets/models/a.bin"));
}

TEST_CASE("Core/FileSystem/FileSystem: writing to a read-only mount fails", "[Core][FileSystem]")
{
    FileSystemGuard guard;
    MountTestAssets();

    // MemoryMountSource is always read-only
    CHECK(FileSystem::Open("assets://new.txt", FileMode::Write) == nullptr);
    CHECK_FALSE(FileSystem::WriteText("assets://new.txt", "data"));
}

TEST_CASE("Core/FileSystem/DiskMountSource: writes a file and reads it back", "[Core][FileSystem]")
{
    FileSystemGuard guard;
    // System temp folder so leftovers never end up in the repo
    const std::filesystem::path root = std::filesystem::temp_directory_path() / "OrionTests-FileSystem";
    std::filesystem::remove_all(root); // Remove all files from a previous failed test

    // UTF-8 explicitly: generic_string() would go through the Windows ANSI code page
    const std::u8string rootUtf8 = root.generic_u8string();
    FileSystem::Mount("temp", CreateScope<DiskMountSource>(std::string(rootUtf8.begin(), rootUtf8.end()), false));

    REQUIRE(FileSystem::WriteText("temp://logs/test.txt", "hello")); // also creates the missing folder
    CHECK(FileSystem::ReadText("temp://logs/test.txt") == "hello");
    CHECK(std::filesystem::is_regular_file(root / "logs" / "test.txt")); // the mount maps to the expected folder

    std::filesystem::remove_all(root);
}

TEST_CASE("Core/FileSystem/DiskMountSource: consecutive writes grow the size to the end position", "[Core][FileSystem]")
{
    FileSystemGuard guard;
    const std::filesystem::path root = std::filesystem::temp_directory_path() / "OrionTests-FileSystem-Size";
    std::filesystem::remove_all(root);

    const std::u8string rootUtf8 = root.generic_u8string();
    FileSystem::Mount("temp", CreateScope<DiskMountSource>(std::string(rootUtf8.begin(), rootUtf8.end()), false));

    {
        Scope<IFile> file = FileSystem::Open("temp://size.bin", FileMode::Write);
        REQUIRE(file);

        const std::array<uint8_t, 3> data{1, 2, 3};
        file->Write(data.data(), 3);
        CHECK(file->Size() == 3);

        file->Write("abcde", 5);
        CHECK(file->Size() == 8);
    }

    std::filesystem::remove_all(root);
}
