#pragma once

#include <optional>
#include <string>
#include <string_view>

namespace Orion::VirtualPath
{
    inline constexpr std::string_view MountSeparator = "://";

    // "assets://models/a.gltf" -> Mount "assets", Relative "models/a.gltf".
    struct Parts
    {
        std::string_view Mount;
        std::string_view Relative;
    };

    // Splits without validating the relative part. nullopt if there is no valid mount name
    [[nodiscard]] std::optional<Parts> Split(std::string_view path);

    // The canonical form of a path, or nullopt if it is not a valid virtual path
    [[nodiscard]] std::optional<std::string> Normalize(std::string_view path);

    // Plain concatenation with a single '/'. Does not normalize: call Normalize on the result
    [[nodiscard]] std::string Join(std::string_view directory, std::string_view relative);

    // On normalized paths: "assets://a/b.png" -> "assets://a", "b.png", "b". The parent of "assets://x" is "assets://"
    [[nodiscard]] std::string_view Parent(std::string_view path);
    [[nodiscard]] std::string_view Filename(std::string_view path);
    [[nodiscard]] std::string_view Stem(std::string_view path);
} // namespace Orion::VirtualPath
