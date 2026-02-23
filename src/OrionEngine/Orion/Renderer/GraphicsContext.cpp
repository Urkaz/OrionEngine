#include "oepch.h"
#include "Orion/Renderer/GraphicsContext.h"

#include "Orion/Platform/OpenGL/OpenGLContext.h"
#include "Orion/Renderer/Renderer.h"

namespace Orion
{
    Scope<GraphicsContext> GraphicsContext::Create(void* window)
    {
        switch (Renderer::GetAPI())
        {
        case RendererAPI::API::None:
            OE_CORE_ASSERT(false, "RendererAPI::None is currently not supported!");
            return nullptr;
        case RendererAPI::API::OpenGL:
            return CreateScope<OpenGLContext>(static_cast<GLFWwindow*>(window));
        }

        OE_CORE_ASSERT(false, "Unknown RendererAPI!");
        return nullptr;
    }

} // namespace Orion
