#include "oepch.h"
#include "GlfwInput.h"

#include "Orion/Core/Application.h"

#include <GLFW/glfw3.h>

namespace Orion
{
    bool GlfwInput::IsKeyPressedImpl(KeyCode key)
    {
        auto* window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());
        const int state    = glfwGetKey(window, static_cast<int32_t>(key));
        return state == GLFW_PRESS || state == GLFW_REPEAT;
    }

    bool GlfwInput::IsMouseButtonPressedImpl(MouseCode button)
    {
        auto* window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());
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
        auto* window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());
        double xpos = 0.0, ypos = 0.0;
        glfwGetCursorPos(window, &xpos, &ypos);
        return {static_cast<float>(xpos), static_cast<float>(ypos)};
    }

    void GlfwInput::SetCursorModeImpl(CursorMode mode)
    {
        auto* window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());

        int glfwMode = GLFW_CURSOR_NORMAL;
        switch (mode)
        {
        default:
        case CursorMode::Normal:
            glfwMode = GLFW_CURSOR_NORMAL;
            break;
        case CursorMode::Hidden:
            glfwMode = GLFW_CURSOR_HIDDEN;
            break;
        case CursorMode::Locked:
            glfwMode = GLFW_CURSOR_DISABLED;
            break;
        }
        glfwSetInputMode(window, GLFW_CURSOR, glfwMode);
    }

} // namespace Orion
