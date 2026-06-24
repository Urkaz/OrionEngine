#pragma once

#if defined(OE_ENABLE_PROFILING)
    #include <tracy/Tracy.hpp>

    #include <chrono>
    #include <cstdlib>
    #include <thread>

    #define OE_PROFILE_SCOPE(name) ZoneScopedN(name)
    #define OE_PROFILE_FUNCTION() ZoneScoped
    #define OE_PROFILE_FRAME() FrameMark

namespace Orion
{
    // With TRACY_ON_DEMAND, nothing is recorded until the profiler connects, so the startup
    // sequence is lost unless something waits for it. This is opt-in via env var so normal runs
    // never block: set OE_PROFILE_WAIT_FOR_CONNECTION, open the Tracy profiler, then launch.
    inline void ProfileWaitForConnection()
    {
        if (!std::getenv("OE_PROFILE_WAIT_FOR_CONNECTION"))
            return;

        while (!TracyIsConnected)
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
} // namespace Orion

    #define OE_PROFILE_WAIT_FOR_CONNECTION() ::Orion::ProfileWaitForConnection()
#else
    #define OE_PROFILE_SCOPE(name)
    #define OE_PROFILE_FUNCTION()
    #define OE_PROFILE_FRAME()
    #define OE_PROFILE_WAIT_FOR_CONNECTION()
#endif
