#include "oepch.h"
#include "Orion/Core/Input.h"

#ifdef OE_PLATFORM_WINDOWS
    #include "Orion/Platform/Windows/WindowsInput.h"
#endif

namespace Orion
{
    Scope<Input> Input::s_Instance = Input::Create();

    Scope<Input> Input::Create()
    {
#ifdef OE_PLATFORM_WINDOWS
        return CreateScope<WindowsInput>();
#else
        OE_CORE_ASSERT(false, "Unknown platform!");
        return nullptr;
#endif
    }
} // namespace Orion
