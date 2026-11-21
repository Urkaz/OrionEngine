#include "Orion/Core/Input.h"

#if defined(OE_PLATFORM_WINDOWS) || defined(OE_PLATFORM_LINUX)
    #include "Orion/Platform/GLFW/GlfwInput.h"
#endif

namespace Orion
{
    UniquePtr<Input> Input::s_Instance = Input::Create();

    UniquePtr<Input> Input::Create()
    {
#if defined(OE_PLATFORM_WINDOWS) || defined(OE_PLATFORM_LINUX)
        return MakeUnique<GlfwInput>();
#else
        OE_CORE_ASSERT(false, "Unknown platform!");
        return nullptr;
#endif
    }
} // namespace Orion
