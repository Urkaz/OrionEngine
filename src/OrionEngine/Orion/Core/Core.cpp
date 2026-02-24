#include "oepch.h"
#include "Orion/Core/Core.h"

namespace Orion
{
    void InitializeCore()
    {
#ifdef OE_ENABLE_LOGS
        Log::Init();
#endif

        OE_CORE_LOG(Info, "--- Orion Engine ---");
        OE_CORE_LOG(Info, "Initializing...");
    }
    void ShutdownCore()
    {
        OE_CORE_LOG(Info, "Shutting down...");
    }
} // namespace Orion
