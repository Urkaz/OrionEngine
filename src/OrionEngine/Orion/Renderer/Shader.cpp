#include "oepch.h"
#include "Shader.h"

#include "Platform/OpenGL/OpenGLShader.h"
#include "Renderer.h"

namespace Orion
{
    Ref<Shader> Shader::Create(const std::string& vertexSrc, const std::string& fragmentSrc)
    {
        switch (Renderer::GetAPI())
        {
        case RendererAPI::API::None:
            OE_ASSERT(false, "RendererAPI::None is not supported!");
            return nullptr;
        case RendererAPI::API::OpenGL:
            return std::make_shared<OpenGLShader>(vertexSrc, fragmentSrc);
        default:
            break;
        }

        OE_ASSERT(false, "Unknown RendererAPI!");
        return nullptr;
    }
} // namespace Orion
