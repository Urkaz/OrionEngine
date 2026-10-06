#include "oepch.h"

#include "Orion/Core/Input.h"
#include "Orion/Core/KeyCodes.h"
#include "Orion/Renderer/FlyCamera.h"

#include <glm/fwd.hpp>
#include <glm/gtc/matrix_transform.hpp>


namespace Orion
{
    FlyCamera::FlyCamera(float verticalFovDegrees, float aspectRatio, float nearClip, float farClip)
        : m_FOV(verticalFovDegrees), m_AspectRatio(aspectRatio), m_NearClip(nearClip), m_FarClip(farClip)
    {
        UpdateProjection();
        UpdateView();
    }

    void FlyCamera::OnUpdate(Timestep ts)
    {
        OE_PROFILE_FUNCTION();

        // --- Mouse look ------------------------------------------------------
        const auto [mouseX, mouseY]   = Input::GetMousePosition();
        const glm::vec2 mousePosition = {mouseX, mouseY};

        if (Input::IsMouseButtonPressed(Mouse::ButtonRight))
        {
            if (!m_Looking)
            {
                //First frame of the drag: start from the current position to avoid a jump
                m_Looking           = true;
                m_LastMousePosition = mousePosition;
                Input::SetCursorMode(CursorMode::Locked);
            }

            // The mouse delta is already "distance moved this frame", so it is NOT scaled by the timestep
            const glm::vec2 mouseDelta = mousePosition - m_LastMousePosition;
            m_Yaw -= mouseDelta.x * m_MouseSensitivity;
            m_Pitch -= mouseDelta.y * m_MouseSensitivity;
        }
        else if (m_Looking)
        {
            m_Looking = false;
            Input::SetCursorMode(CursorMode::Normal);
        }
        m_LastMousePosition = mousePosition;

        // --- Movement (only while looking, like most editors) ---------------
        if (m_Looking)
        {
            float speed = m_MoveSpeed;
            if (Input::IsKeyPressed(Key::LeftShift))
                speed *= 5.0f;

            const glm::vec3 forward = GetForwardDirection();
            const glm::vec3 right   = GetRightDirection();
            const glm::vec3 worldUp = {0.0f, 1.0f, 0.0f};

            glm::vec3 direction{0.0f};
            if (Input::IsKeyPressed(Key::W))
                direction += forward;
            if (Input::IsKeyPressed(Key::S))
                direction -= forward;
            if (Input::IsKeyPressed(Key::D))
                direction += right;
            if (Input::IsKeyPressed(Key::A))
                direction -= right;
            if (Input::IsKeyPressed(Key::E))
                direction += worldUp;
            if (Input::IsKeyPressed(Key::Q))
                direction -= worldUp;

            // Normalize so moving diagonally is not faster than moving straight
            if (glm::dot(direction, direction) > 0.0f)
                m_Position += glm::normalize(direction) * speed * ts.GetSeconds();
        }
    }

    void FlyCamera::OnEvent(Event& e)
    {
        EventDispatcher dispatcher(e);
        dispatcher.Dispatch<MouseScrolledEvent>(OE_BIND_EVENT_FN(FlyCamera::OnMouseScrolled));
        dispatcher.Dispatch<WindowResizeEvent>(OE_BIND_EVENT_FN(FlyCamera::OnWindowResize));
    }

    void FlyCamera::SetPosition(const glm::vec3& position)
    {
        m_Position = position;
        UpdateView();
    }

    glm::vec3 FlyCamera::GetRightDirection() const
    {
        return GetRotation() * glm::vec3(1.0f, 0.0f, 0.0f);
    }

    glm::vec3 FlyCamera::GetUpDirection() const
    {
        return GetRotation() * glm::vec3(0.0f, 1.0f, 0.0f);
    }

    glm::vec3 FlyCamera::GetForwardDirection() const
    {
        return GetRotation() * glm::vec3(0.0f, 0.0f, -1.0f);
    }

    void FlyCamera::SetFOV(float verticalFovDegrees)
    {
        m_FOV = verticalFovDegrees;
        UpdateProjection();
    }

    void FlyCamera::SetClipPlanes(float nearClip, float farClip)
    {
        m_NearClip = nearClip;
        m_FarClip  = farClip;
        UpdateProjection();
    }

    void FlyCamera::UpdateProjection()
    {
        SetPerspective(glm::radians(m_FOV), m_AspectRatio, m_NearClip, m_FarClip);
    }

    void FlyCamera::UpdateView()
    {
        // The camera transform in the world
        const glm::mat4 transform = glm::translate(glm::mat4(1.0f), m_Position) * glm::mat4(GetRotation());
        // and the view matrix is the inverse of that
        m_ViewMatrix = glm::inverse(transform);
    }

    glm::mat3 FlyCamera::GetRotation() const
    {
        // First pitch around the local X, then yaw around the world Y (read right to left)
        glm::mat4 rotation = glm::rotate(glm::mat4(1.0f), glm::radians(m_Yaw), glm::vec3(0.0f, 1.0f, 0.0f));
        rotation           = glm::rotate(rotation, glm::radians(m_Pitch), glm::vec3(1.0f, 0.0f, 0.0f));
        return glm::mat3(rotation); // NOLINT(modernize-return-braced-init-list): explicit mat4 -> mat3 conversion
    }

    bool FlyCamera::OnMouseScrolled(MouseScrolledEvent& e)
    {
        // Multiplivative steps feel uniform at any speed (x1.2 per notch)
        m_MoveSpeed *= (e.GetYOffset() > 0.0f) ? 1.2f : 1.0f / 1.2f;
        m_MoveSpeed = std::clamp(m_MoveSpeed, 0.1f, 500.0f);
        return false;
    }

    bool FlyCamera::OnWindowResize(WindowResizeEvent& e)
    {
        if (e.GetWidth() == 0 || e.GetHeight() == 0)
            return false; // minimized: keep last valid aspect ratio

        m_AspectRatio = static_cast<float>(e.GetWidth()) / static_cast<float>(e.GetHeight());
        UpdateProjection();
        return false;
    }

} // namespace Orion
