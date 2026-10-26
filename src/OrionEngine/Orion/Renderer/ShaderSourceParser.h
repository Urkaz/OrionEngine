#pragma once

#include <string>
#include <string_view>
#include <unordered_map>

namespace Orion
{
    enum class ShaderStage : uint8_t
    {
        Vertex,
        Fragment
    };

    // The result of splitting a ".glsl" with "#type <stage>" sections into one source per stage.
    // On failure "Error" says what is wrong and "Stages" must not be used.
    struct ShaderParseResult
    {
        std::unordered_map<ShaderStage, std::string> Stages;
        std::string Error;

        [[nodiscard]] bool Ok() const
        {
            return Error.empty();
        }
    };

    ShaderParseResult ParseShaderSource(std::string_view source);
} // namespace Orion
