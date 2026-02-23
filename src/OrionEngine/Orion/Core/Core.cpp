#include "oepch.h"
#include "Orion/Core/Core.h"

namespace Orion
{
    void InitializeCore()
    {
#ifdef OE_ENABLE_LOGS
        Log::Init();
#endif

        OE_CORE_LOG(trace, "--- Orion Engine ---");
        OE_CORE_LOG(trace, "Initializing...");
    }
    void ShutdownCore()
    {
        OE_CORE_LOG(info, "Shutting down...");
    }
} // namespace Orion
