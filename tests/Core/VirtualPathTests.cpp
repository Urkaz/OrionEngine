#include "Orion/Core/FileSystem/VirtualPath.h"

#include <catch2/catch_test_macros.hpp>

using namespace Orion;

TEST_CASE("Core/FileSystem/VirtualPath: Split separates the mount name from the relative path", "[Core][FileSystem]")
{
    const auto parts = VirtualPath::Split("assets://textures/a.png");
    REQUIRE(parts);
    CHECK(parts->Mount == "assets");
    CHECK(parts->Relative == "textures/a.png");

    const auto root = VirtualPath::Split("user_dir_1://");
    REQUIRE(root);
    CHECK(root->Mount == "user_dir_1");
    CHECK(root->Relative.empty());
}

TEST_CASE("Core/FileSystem/VirtualPath: Split rejects paths without a valid mount", "[Core][FileSystem]")
{
    CHECK_FALSE(VirtualPath::Split("textures/a.png"));
    CHECK_FALSE(VirtualPath::Split("/textures/a.png"));
    CHECK_FALSE(VirtualPath::Split("://a.png"));
    CHECK_FALSE(VirtualPath::Split("C:\\Users\\a.png"));
    CHECK_FALSE(VirtualPath::Split("Assets://textures/a.png")); // mount names must be lowercase
}

TEST_CASE("Core/FileSystem/VirtualPath: Normalize converts backslashes and removes empty and dot segments",
          "[Core][FileSystem]")
{
    CHECK(VirtualPath::Normalize("assets://textures\\a.png") == "assets://textures/a.png");
    CHECK(VirtualPath::Normalize("assets://textures/./Testing") == "assets://textures/Testing");
    CHECK(VirtualPath::Normalize("assets://textures/Testing/") == "assets://textures/Testing");
    CHECK(VirtualPath::Normalize("assets://textures//Testing") == "assets://textures/Testing");
    CHECK(VirtualPath::Normalize("assets://") == "assets://");
    CHECK(VirtualPath::Normalize("assets://./") == "assets://");
    CHECK(VirtualPath::Normalize("assets://textures/.") == "assets://textures");
    CHECK(VirtualPath::Normalize("assets://textures/a.png") == "assets://textures/a.png");
}

TEST_CASE("Core/FileSystem/VirtualPath: Normalize resolves parent segments inside the mount", "[Core][FileSystem]")
{
    CHECK(VirtualPath::Normalize("assets://textures/../Testing") == "assets://Testing");
    CHECK(VirtualPath::Normalize("assets://textures/..") == "assets://");
}

TEST_CASE("Core/FileSystem/VirtualPath: Normalize rejects paths that escape the mount root", "[Core][FileSystem]")
{
    CHECK_FALSE(VirtualPath::Normalize("assets://.."));
    CHECK_FALSE(VirtualPath::Normalize("assets://textures/../../testing"));
}

TEST_CASE("Core/FileSystem/VirtualPath: Normalize rejects absolute native paths", "[Core][FileSystem]")
{
    CHECK_FALSE(VirtualPath::Normalize("assets:///etc/passwd"));
    CHECK_FALSE(VirtualPath::Normalize("assets://C:/Users/Testing/Documents"));
    // What Assimp builds when it probes for a missing file
    CHECK_FALSE(VirtualPath::Normalize("assets://textures/assets://textures/a.png"));
}

TEST_CASE("Core/FileSystem/VirtualPath: Parent, Filename and Stem split a normalized path", "[Core][FileSystem]")
{
    CHECK(VirtualPath::Parent("assets://textures/Testing/a.png") == "assets://textures/Testing");
    CHECK(VirtualPath::Parent("assets://a.png") == "assets://");
    CHECK(VirtualPath::Parent("assets://") == "assets://");
    CHECK(VirtualPath::Parent("a.png").empty());
    CHECK(VirtualPath::Filename("assets://a.png") == "a.png");
    CHECK(VirtualPath::Filename("assets://textures/a.png") == "a.png");
    CHECK(VirtualPath::Filename("assets://.ignore") == ".ignore");
    CHECK(VirtualPath::Stem("assets://textures/a_nice_texture.png") == "a_nice_texture");
    CHECK(VirtualPath::Stem("assets://.ignore") == ".ignore");
    CHECK(VirtualPath::Stem("assets://file_without_extension") == "file_without_extension");
}

TEST_CASE("Core/FileSystem/VirtualPath: Join adds a single separator and does not normalize", "[Core][FileSystem]")
{

    CHECK(VirtualPath::Join("assets://textures", "a.png") == "assets://textures/a.png");
    CHECK(VirtualPath::Join("assets://textures/", "a.png") == "assets://textures/a.png");
    CHECK(VirtualPath::Join("assets://textures", "/a.png") == "assets://textures//a.png");           // not normalized
    CHECK(VirtualPath::Join("assets://textures", "../a\\b.png") == "assets://textures/../a\\b.png"); // not normalized
    CHECK(VirtualPath::Join("assets://textures", "") == "assets://textures");
    CHECK(VirtualPath::Join("assets://", "a.png") == "assets://a.png");
    CHECK(VirtualPath::Join("", "a.png") == "a.png");
}
