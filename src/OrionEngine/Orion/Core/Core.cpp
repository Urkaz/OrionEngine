#include "oepch.h"
#include "Core.h"

namespace Orion
{
    void InitializeCore()
    {
        Log::Init();

        OE_CORE_LOG(trace, "--- Orion Engine ---");
        OE_CORE_LOG(trace, "Initializing...");
    }
    void ShutdownCore()
    {
        OE_CORE_LOG(info, "Shutting down...");
    }
} // namespace Orion
