#include "Orion/Renderer/ShaderSourceParser.h"

#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_string.hpp>

using Catch::Matchers::ContainsSubstring;

namespace
{
    constexpr const char* TwoStages = "#type vertex\n"
                                      "// VERTEX BODY\n"
                                      "void main() {}\n"
                                      "#type fragment\n"
                                      "// FRAGMENT BODY\n"
                                      "void main() {}\n";

    bool Contains(const std::string& text, const char* needle)
    {
        return text.find(needle) != std::string::npos;
    }
} // namespace

TEST_CASE("Renderer/Shader/ParseShaderSource: splits a vertex and a fragment section", "[Renderer][Shader]")
{
    const Orion::ShaderParseResult result = Orion::ParseShaderSource(TwoStages);

    REQUIRE(result.Ok());
    REQUIRE(result.Stages.size() == 2);

    const std::string& vertex   = result.Stages.at(Orion::ShaderStage::Vertex);
    const std::string& fragment = result.Stages.at(Orion::ShaderStage::Fragment);

    CHECK(Contains(vertex, "VERTEX BODY"));
    CHECK_FALSE(Contains(vertex, "FRAGMENT BODY"));
    CHECK(Contains(fragment, "FRAGMENT BODY"));
    CHECK_FALSE(Contains(fragment, "VERTEX BODY"));

    CHECK_FALSE(Contains(vertex, "#type"));
    CHECK_FALSE(Contains(fragment, "#type"));

    CHECK(vertex == "// VERTEX BODY\nvoid main() {}\n");
}

TEST_CASE("Renderer/Shader/ParseShaderSource: accepts CRLF line endings and extra whitespace in the directive",
          "[Renderer][Shader]")
{
    SECTION("CRLF")
    {
        const Orion::ShaderParseResult result = Orion::ParseShaderSource("#type vertex\r\nvoid main() {}\r\n"
                                                                         "#type fragment\r\nvoid main() {}\r\n");

        CHECK(result.Ok());
        CHECK(result.Stages.size() == 2);
    }

    SECTION("two spaces after #type")
    {
        const Orion::ShaderParseResult result = Orion::ParseShaderSource("#type  vertex\nvoid main() {}\n");

        CHECK(result.Ok());
        CHECK(result.Stages.contains(Orion::ShaderStage::Vertex));
    }

    SECTION("trailing spaces and tabs")
    {
        const Orion::ShaderParseResult result = Orion::ParseShaderSource("#type vertex \t\nvoid main() {}\n");

        CHECK(result.Ok());
        CHECK(result.Stages.contains(Orion::ShaderStage::Vertex));
    }
}

TEST_CASE("Renderer/Shader/ParseShaderSource: a '#type' that is not at the start of a line is plain code",
          "[Renderer][Shader]")
{
    const Orion::ShaderParseResult result = Orion::ParseShaderSource("#type vertex\n"
                                                                     "// this #type shouldn't split the vertex shader in 2\n"
                                                                     "void main() {}\n"
                                                                     "#type fragment\n"
                                                                     "void main() {}\n");

    REQUIRE(result.Ok());
    CHECK(result.Stages.size() == 2);
    CHECK(Contains(result.Stages.at(Orion::ShaderStage::Vertex), "this #type shouldn't split the vertex shader in 2"));
}

TEST_CASE("Renderer/Shader/ParseShaderSource: malformed files return an error instead of asserting",
          "[Renderer][Shader]")
{
    SECTION("unknown stage")
    {
        const auto result = Orion::ParseShaderSource("#type geometry\nvoid main() {}\n");
        CHECK_FALSE(result.Ok());
        CHECK_THAT(result.Error, ContainsSubstring("geometry"));
    }

    SECTION("duplicated stage")
    {
        const auto result = Orion::ParseShaderSource("#type vertex\nvoid main() {}\n#type vertex\nvoid main() {}\n");
        CHECK_FALSE(result.Ok());
        CHECK_THAT(result.Error, ContainsSubstring("Duplicate"));
        CHECK_THAT(result.Error, ContainsSubstring("vertex"));
    }

    SECTION("empty body")
    {
        const auto result = Orion::ParseShaderSource("#type vertex\n#type fragment\nvoid main() {}\n");
        CHECK_FALSE(result.Ok());
        CHECK_THAT(result.Error, ContainsSubstring("empty"));
    }

    SECTION("no directive at all")
    {
        const auto result = Orion::ParseShaderSource("void main() {}\n");
        CHECK_FALSE(result.Ok());
        CHECK_THAT(result.Error, ContainsSubstring("#type"));
    }

    SECTION("empty input")
    {
        CHECK_FALSE(Orion::ParseShaderSource("").Ok());
    }
}
