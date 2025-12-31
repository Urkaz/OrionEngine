#include "oepch.h"
#include "RenderCommand.h"

#include "Platform/OpenGL/OpenGLRendererAPI.h"

namespace Orion
{
    RendererAPI* RenderCommand::s_RendererAPI = new OpenGLRendererAPI;

} // namespace Orion
