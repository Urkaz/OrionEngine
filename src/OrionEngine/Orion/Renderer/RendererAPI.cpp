#include "Orion/Renderer/RendererAPI.h"

#include "Orion/Platform/OpenGL/OpenGLRendererAPI.h"

namespace Orion
{
    RendererAPI::API RendererAPI::s_API = RendererAPI::API::OpenGL;

    Scope<RendererAPI> RendererAPI::Create()
    {
        switch (s_API)
        {
        case RendererAPI::API::None:
            OE_CORE_ASSERT(false, "RendererAPI::None is currently not supported!");
            return nullptr;
        case RendererAPI::API::OpenGL:
            return CreateScope<OpenGLRendererAPI>();
        }

        OE_CORE_ASSERT(false, "Unknown RendererAPI!");
        return nullptr;
    }

} // namespace Orion
