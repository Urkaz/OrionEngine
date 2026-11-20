#include "oepch.h"

#include "Orion/Core/Core.h"

#include "Orion/Core/FileSystem/DiskMountSource.h"
#include "Orion/Core/FileSystem/FileSystem.h"
#include "Orion/Core/FileSystem/PlatformFile.h"

namespace Orion
{
    void InitializeCore()
    {
        const std::string executableDirectory = PlatformFile::GetExecutableDirectory();
        FileSystem::Mount("assets", CreateScope<DiskMountSource>(executableDirectory + "/assets", true));
        FileSystem::Mount("user", CreateScope<DiskMountSource>(executableDirectory + "/user", false));

#ifdef OE_ENABLE_LOGS
        Log::Init(); // The log goes to user://Orion.log. It needs the mounts first
#endif

        OE_CORE_LOG(Info, "--- Orion Engine ---");
        OE_CORE_LOG(Info, "Initializing...");
        OE_CORE_LOG(Info, "Executable directory: {0}", executableDirectory);
    }

    void ShutdownCore()
    {
        OE_CORE_LOG(Info, "Shutting down...");
#ifdef OE_ENABLE_LOGS
        Log::Shutdown();
#endif
        FileSystem::UnmountAll();
    }
} // namespace Orion
