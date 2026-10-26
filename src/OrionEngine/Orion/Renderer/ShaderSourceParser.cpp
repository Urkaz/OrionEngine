#include "oepch.h"
#include "ShaderSourceParser.h"
#include <optional>
#include <string_view>

namespace Orion
{
    namespace
    {
        constexpr std::string_view DirectiveToken = "#type";

        std::string_view Trim(std::string_view text)
        {
            constexpr std::string_view whitespace = " \t\r\n";
            const size_t first                    = text.find_first_not_of(whitespace);
            if (first == std::string_view::npos)
                return {};
            const size_t last = text.find_last_not_of(whitespace);
            return text.substr(first, last - first + 1);
        }

        bool StageFromName(std::string_view name, ShaderStage& stage)
        {
            if (name == "vertex")
                stage = ShaderStage::Vertex;
            else if (name == "fragment")
                stage = ShaderStage::Fragment;
            else
                return false;
            return true;
        }

        const char* StageName(ShaderStage stage)
        {
            return stage == ShaderStage::Vertex ? "vertex" : "fragment";
        }
    } // namespace

    ShaderParseResult ParseShaderSource(std::string_view source)
    {
        ShaderParseResult result;
        std::optional<ShaderStage> current;
        size_t bodyBegin = 0;

        // Stores source[bodyBegin, bodyEnd) as the body of the stage being read.
        // returns true if everything goes well, false on an error (if the body is empty)
        // Even if the 3 variables above change, they are captured with a reference, so the value is always updated
        const auto finishStage = [&](size_t bodyEnd) {
            if (!current)
                return true;

            const std::string_view body = source.substr(bodyBegin, bodyEnd - bodyBegin);
            if (Trim(body).empty())
            {
                result.Error = std::string("Shader stage '") + StageName(*current) + "' is empty";
                return false;
            }
            result.Stages[*current] = std::string(body);
            return true;
        };

        size_t lineBegin = 0;
        while (lineBegin < source.size())
        {
            const size_t newLine        = source.find('\n', lineBegin);
            const size_t lineEnd        = newLine == std::string_view::npos ? source.size() : newLine;
            const size_t nextLine       = newLine == std::string_view::npos ? source.size() : newLine + 1;
            const std::string_view line = source.substr(lineBegin, lineEnd - lineBegin);

            if (line.starts_with(DirectiveToken))
            {
                // Try to close the current stage if there is any
                if (!finishStage(lineBegin))
                    return result;

                // Find the stage type and assign it
                const std::string_view name = Trim(line.substr(DirectiveToken.size()));
                ShaderStage stage{};
                if (!StageFromName(name, stage))
                {
                    result.Error = "Unknown shader type '" + std::string(name) + "'";
                    return result;
                }
                // but error if  the stage found was already stored
                if (result.Stages.contains(stage))
                {
                    result.Error = std::string("Duplicate '#type ") + StageName(stage) + "' section";
                    return result;
                }

                current   = stage;
                bodyBegin = nextLine;
            }
            lineBegin = nextLine;
        }

        // We reached the end of the file
        if (!finishStage(source.size()))
            return result;
        if (result.Stages.empty())
            result.Error = "No '#type' section found";
        return result;
    }
} // namespace Orion
