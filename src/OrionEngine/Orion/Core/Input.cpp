#include "oepch.h"
#include "Orion/Core/Input.h"

#if defined(OE_PLATFORM_WINDOWS) || defined(OE_PLATFORM_LINUX)
    #include "Orion/Platform/GLFW/GlfwInput.h"
#endif

namespace Orion
{
    Scope<Input> Input::s_Instance = Input::Create();

    Scope<Input> Input::Create()
    {
#if defined(OE_PLATFORM_WINDOWS) || defined(OE_PLATFORM_LINUX)
        return CreateScope<GlfwInput>();
#else
        OE_CORE_ASSERT(false, "Unknown platform!");
        return nullptr;
#endif
    }
} // namespace Orion
