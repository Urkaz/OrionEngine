#pragma once

#include "Orion/Core/FileSystem/MountSource.h"

#include <string>

namespace Orion
{
    class DiskMountSource final : public IMountSource
    {
    public:
        // `nativeRoot`: UTF-8, '/' separators, no trailing '/'
        DiskMountSource(std::string nativeRoot, bool readOnly)
            : m_NativeRoot(std::move(nativeRoot)), m_ReadOnly(readOnly)
        {}

        [[nodiscard]] UniquePtr<IFile> Open(std::string_view relativePath, FileMode mode) override;
        [[nodiscard]] bool Exists(std::string_view relativePath) const override;
        [[nodiscard]] bool IsReadOnly() const override
        {
            return m_ReadOnly;
        }

    private:
        std::string m_NativeRoot;
        bool m_ReadOnly;
    };
} // namespace Orion
