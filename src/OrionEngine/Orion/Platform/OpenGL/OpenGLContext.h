#pragma once

#include "Orion/Renderer/GraphicsContext.h"

struct GLFWwindow;

namespace Orion
{
    class OpenGLContext final : public GraphicsContext
    {
    public:
        explicit OpenGLContext(GLFWwindow* windowHandle);

        void Init() override;
        void SwapBuffers() override;

    private:
        GLFWwindow* m_WindowHandle;
    };
} // namespace Orion
