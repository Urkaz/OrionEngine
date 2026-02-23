#include "oepch.h"
#include "RenderCommand.h"

#include "Orion/Platform/OpenGL/OpenGLRendererAPI.h"

namespace Orion
{
    Scope<RendererAPI> RenderCommand::s_RendererAPI = CreateScope<OpenGLRendererAPI>();

} // namespace Orion
