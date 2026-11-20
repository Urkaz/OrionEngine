#include "oepch.h"

#include "Orion/Core/FileSystem/VirtualPath.h"

#include <algorithm>
#include <ranges>

namespace Orion::VirtualPath
{
    std::optional<Parts> Split(std::string_view path)
    {
        const size_t separator = path.find(MountSeparator);
        if (separator == std::string_view::npos || separator == 0)
            return std::nullopt;

        const std::string_view mount = path.substr(0, separator);

        // Only allow lowercase letters, digits and underscores in mount names.
        const bool is_valid_mount = std::ranges::all_of(mount, [](char c) {
            return (c >= 'a' && c <= 'z') || (c >= '0' && c <= '9') || c == '_';
        });
        if (!is_valid_mount)
            return std::nullopt;

        const std::string_view relative = path.substr(separator + MountSeparator.size());
        return Parts{.Mount = mount, .Relative = relative};
    }

    std::optional<std::string> Normalize(std::string_view path)
    {
        const std::optional<Parts> parts = Split(path);
        if (!parts)
            return std::nullopt;

        std::string relative(parts->Relative);
        std::ranges::replace(relative, '\\', '/');

        if (relative.starts_with('/') || relative.find(':') != std::string::npos)
            return std::nullopt;

        std::vector<std::string_view> segments;
        const std::string_view remaining(relative);
        size_t start = 0;
        while (start <= remaining.size())
        {
            const size_t end               = std::min(remaining.find('/', start), remaining.size());
            const std::string_view segment = remaining.substr(start, end - start);
            start                          = end + 1; // advance BEFORE any 'continue'

            if (segment.empty() || segment == ".")
                continue;

            if (segment == "..")
            {
                if (segments.empty())
                    return std::nullopt; // don't allow escaping the mount root
                segments.pop_back();
                continue;
            }
            segments.push_back(segment);
        }

        std::string normalized(parts->Mount);
        normalized += MountSeparator;
        for (size_t i = 0; i < segments.size(); ++i)
        {
            if (i > 0)
                normalized += '/';
            normalized += segments[i];
        }

        return normalized;
    }

    std::string Join(std::string_view directory, std::string_view relative)
    {
        std::string result(directory);
        if (!result.empty() && !result.ends_with('/') && !relative.empty())
            result += '/';
        result += relative;
        return result;
    }

    std::string_view Parent(std::string_view path)
    {
        const size_t slash = path.find_last_of('/');
        if (slash == std::string_view::npos)
            return {};

        // The last '/' belongs to "://": the parent is the mount root itself
        if (slash >= 2 && path.substr(slash - 2, MountSeparator.size()) == MountSeparator)
            return path.substr(0, slash + 1);
        return path.substr(0, slash);
    }

    std::string_view Filename(std::string_view path)
    {
        const size_t slash = path.find_last_of('/');
        return slash == std::string_view::npos ? path : path.substr(slash + 1);
    }

    std::string_view Stem(std::string_view path)
    {
        const std::string_view filename = Filename(path);
        const size_t dot                = filename.rfind('.');
        return dot == std::string_view::npos || dot == 0 ? filename : filename.substr(0, dot);
    }
} // namespace Orion::VirtualPath
