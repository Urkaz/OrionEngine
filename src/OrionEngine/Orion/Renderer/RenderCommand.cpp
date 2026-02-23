#include "oepch.h"
#include "Orion/Renderer/RenderCommand.h"

#include "Orion/Platform/OpenGL/OpenGLRendererAPI.h"

namespace Orion
{
    Scope<RendererAPI> RenderCommand::s_RendererAPI = RendererAPI::Create();

} // namespace Orion
