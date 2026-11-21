#pragma once

#include "Orion/Core/FileSystem/MountSource.h"

#include <cstdint>
#include <functional>
#include <string>
#include <unordered_map>
#include <vector>

namespace Orion
{
    // Transparent hash: lets an unordered_map<std::string, ...> be searched with a string_view without a copy
    struct StringHash
    {
        using is_transparent = void;
        size_t operator()(std::string_view text) const noexcept
        {
            return std::hash<std::string_view>{}(text);
        }
    };

    // Read-only files kept in RAM. Files opened from it are VIEWS: they must not outlive the source
    class MemoryMountSource final : public IMountSource
    {
    public:
        void AddFile(std::string relativePath, std::vector<uint8_t> data);
        void AddFile(std::string relativePath, std::string_view text);

        [[nodiscard]] UniquePtr<IFile> Open(std::string_view relativePath, FileMode mode) override;
        [[nodiscard]] bool Exists(std::string_view relativePath) const override;
        [[nodiscard]] bool IsReadOnly() const override
        {
            return true;
        }

    private:
        std::unordered_map<std::string, std::vector<uint8_t>, StringHash, std::equal_to<>> m_Files;
    };

} // namespace Orion
