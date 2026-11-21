#include "Orion/Core/FileSystem/DiskMountSource.h"

#include "Orion/Core/Base.h"
#include "Orion/Core/FileSystem/PlatformFile.h"
#include "Orion/Core/FileSystem/VirtualPath.h"

namespace Orion
{
    UniquePtr<IFile> DiskMountSource::Open(std::string_view relativePath, FileMode mode)
    {
        if (mode == FileMode::Write && m_ReadOnly)
            return nullptr;
        return PlatformFile::Open(VirtualPath::Join(m_NativeRoot, relativePath), mode);
    }

    bool DiskMountSource::Exists(std::string_view relativePath) const
    {
        return PlatformFile::Exists(VirtualPath::Join(m_NativeRoot, relativePath));
    }
} // namespace Orion
