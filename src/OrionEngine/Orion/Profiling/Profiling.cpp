#include "oepch.h"

#include "Orion/Profiling/Profiling.h"

#if defined(OE_ENABLE_PROFILING)

#include <chrono>
#include <cstdlib>
#include <thread>

DISABLE_WARNING_FILE(4996, "-Wdeprecated-declarations")

namespace Orion
{
    void ProfileWaitForConnection()
    {
        if (!std::getenv("OE_PROFILE_WAIT_FOR_CONNECTION"))
            return;

        while (!TracyIsConnected)
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
} // namespace Orion

#endif
