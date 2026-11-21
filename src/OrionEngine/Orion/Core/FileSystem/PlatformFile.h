#pragma once

#include "Orion/Core/Core.h"
#include "Orion/Core/FileSystem/File.h"

#include <string>

// Platform layer: the ONLY code that talks to the OS file APIs. Paths here are NATIVE and have UTF-8 encoding.
// Each platform provides one implementation
namespace Orion::PlatformFile
{
    // Write mode creates the missing parent folders
    [[nodiscard]] UniquePtr<IFile> Open(const std::string& nativePath, FileMode mode);
    [[nodiscard]] bool Exists(const std::string& nativePath);

    // UTF-8, "/" separators, and no trailing "/"
    [[nodiscard]] std::string GetExecutableDirectory();
} // namespace Orion::PlatformFile
