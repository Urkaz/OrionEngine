#pragma once

#include "Orion/Core/Core.h"
#include "Orion/Core/FileSystem/File.h"

#include <string_view>

namespace Orion
{

    // Something that can be mounted under a name ("assets", "user"...). Paths are RELATIVE to the mount and should already be normalized
    class IMountSource
    {
    public:
        IMountSource()                               = default;
        IMountSource(const IMountSource&)            = delete;
        IMountSource& operator=(const IMountSource&) = delete;
        IMountSource(IMountSource&&)                 = delete;
        IMountSource& operator=(IMountSource&&)      = delete;
        virtual ~IMountSource()                      = default;

        // nullptr if the file does not exist or cannot be opened. Never asserts.
        [[nodiscard]] virtual UniquePtr<IFile> Open(std::string_view relativePath, FileMode mode) = 0;
        [[nodiscard]] virtual bool Exists(std::string_view relativePath) const                    = 0;
        [[nodiscard]] virtual bool IsReadOnly() const                                             = 0;
    };

} // namespace Orion
