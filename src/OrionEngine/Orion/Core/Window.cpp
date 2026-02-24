#include "oepch.h"
#include "Orion/Core/Window.h"

#ifdef OE_PLATFORM_WINDOWS
    #include "Orion/Platform/Windows/WindowsWindow.h"
#endif

namespace Orion
{
    Scope<Window> Window::Create(const WindowProps& props)
    {
#ifdef OE_PLATFORM_WINDOWS
        return CreateScope<WindowsWindow>(props);
#else
        OE_CORE_ASSERT(false, "Unknown platform!");
        return nullptr;
#endif
    }

} // namespace Orion
