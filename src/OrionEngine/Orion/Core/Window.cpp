#include "Orion/Core/Window.h"

#if defined(OE_PLATFORM_WINDOWS) || defined(OE_PLATFORM_LINUX)
    #include "Orion/Platform/GLFW/GlfwWindow.h"
#endif

namespace Orion
{
    UniquePtr<Window> Window::Create(const WindowProps& props)
    {
#if defined(OE_PLATFORM_WINDOWS) || defined(OE_PLATFORM_LINUX)
        return MakeUnique<GlfwWindow>(props);
#else
        OE_CORE_ASSERT(false, "Unknown platform!");
        return nullptr;
#endif
    }

} // namespace Orion
