#include "Orion/Renderer/RenderCommand.h"

#include "Orion/Platform/OpenGL/OpenGLRendererAPI.h"

namespace Orion
{
    UniquePtr<RendererAPI> RenderCommand::s_RendererAPI = RendererAPI::Create();

} // namespace Orion
