#include "oepch.h"
#include "GlfwInput.h"

#include "Orion/Core/Application.h"

#include <GLFW/glfw3.h>

namespace Orion
{
    bool GlfwInput::IsKeyPressedImpl(KeyCode key)
    {
        GLFWwindow* window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());
        const int state    = glfwGetKey(window, static_cast<int32_t>(key));
        return state == GLFW_PRESS || state == GLFW_REPEAT;
    }

    bool GlfwInput::IsMouseButtonPressedImpl(MouseCode button)
    {
        GLFWwindow* window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());
        const int state    = glfwGetMouseButton(window, static_cast<int32_t>(button));
        return state == GLFW_PRESS;
    }

    float GlfwInput::GetMouseXImpl()
    {
        auto [x, y] = GetMousePositionImpl();
        return x;
    }

    float GlfwInput::GetMouseYImpl()
    {
        auto [x, y] = GetMousePositionImpl();
        return y;
    }

    std::pair<float, float> GlfwInput::GetMousePositionImpl()
    {
        GLFWwindow* window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());
        double xpos = 0.0, ypos = 0.0;
        glfwGetCursorPos(window, &xpos, &ypos);
        return {static_cast<float>(xpos), static_cast<float>(ypos)};
    }

} // namespace Orion
