#include "Orion/Core/Core.h"
#include "Orion/Core/FileSystem/Internal/OpenFileRegistry.h"
#include "Orion/Core/FileSystem/MemoryMountSource.h"

#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_vector.hpp>
#include <cstdint>
#include <string>
#include <vector>

using namespace Orion;

TEST_CASE("Core/FileSystem/OpenFileRegistry: removing the first of two open files keeps the other",
          "[Core][FileSystem]")
{
    Internal::OpenFileRegistry registry;

    MemoryMountSource source;
    source.AddFile("a.bin", std::vector<uint8_t>{10, 20, 30});
    source.AddFile("b.bin", std::vector<uint8_t>{40, 50, 60});

    UniquePtr<IFile> file1 = source.Open("a.bin", FileMode::Read);
    UniquePtr<IFile> file2 = source.Open("b.bin", FileMode::Read);
    REQUIRE(file1);
    REQUIRE(file2);

    registry.Add(file1.get(), "a.bin");
    registry.Add(file2.get(), "b.bin");

    CHECK_THAT(registry.GetOpenPaths(), Catch::Matchers::UnorderedEquals(std::vector<std::string>{"a.bin", "b.bin"}));

    registry.Remove(file1.get());

    CHECK(registry.GetOpenPaths() == std::vector<std::string>{"b.bin"});

    registry.Remove(file2.get());

    CHECK(registry.GetOpenPaths().empty());
}
