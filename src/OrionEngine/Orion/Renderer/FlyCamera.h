#pragma once

#include "Orion/Core/Events/ApplicationEvent.h"
#include "Orion/Core/Events/MouseEvent.h"
#include "Orion/Core/Timestep.h"
#include "Orion/Renderer/Camera.h"

namespace Orion
{
    // Free-flying perspective camera.
    // Hold right mouse button to look around, WASD to move, Q/E down/up, Shift to go faster,
    // mouse wheel to change the movement speed.
    class FlyCamera : public Camera
    {
    public:
        FlyCamera(float verticalFovDegrees, float aspectRatio, float nearClip, float farClip);

        void OnUpdate(Timestep ts);
        void OnEvent(Event& e);

        [[nodiscard]] const glm::mat4& GetViewMatrix() const
        {
            return m_ViewMatrix;
        }
        [[nodiscard]] glm::mat4 GetViewProjectionMatrix() const
        {
            return m_ProjectionMatrix * m_ViewMatrix;
        }

        [[nodiscard]] const glm::vec3& GetPosition() const
        {
            return m_Position;
        }
        void SetPosition(const glm::vec3& position);
        void SetRotation(float yawDegrees, float pitchDegrees);

        [[nodiscard]] glm::vec3 GetRightDirection() const;
        [[nodiscard]] glm::vec3 GetUpDirection() const;
        [[nodiscard]] glm::vec3 GetForwardDirection() const;

        [[nodiscard]] float GetYaw() const
        {
            return m_Yaw;
        }
        [[nodiscard]] float GetPitch() const
        {
            return m_Pitch;
        }

        [[nodiscard]] float GetFOV() const
        {
            return m_FOV;
        }
        [[nodiscard]] float GetNearClip() const
        {
            return m_NearClip;
        }
        [[nodiscard]] float GetFarClip() const
        {
            return m_FarClip;
        }
        void SetFOV(float fov);
        void SetClipPlanes(float nearClip, float farClip);

        [[nodiscard]] float GetMoveSpeed() const
        {
            return m_MoveSpeed;
        }

    private:
        void UpdateProjection();
        void UpdateView();
        [[nodiscard]] glm::mat3 GetRotation() const;

        bool OnMouseScrolled(MouseScrolledEvent& e);
        bool OnWindowResize(WindowResizeEvent& e);

    private:
        // Projection
        float m_FOV;
        float m_AspectRatio;
        float m_NearClip;
        float m_FarClip;

        // View
        glm::mat4 m_ViewMatrix = glm::mat4(1.0f);
        glm::vec3 m_Position   = {0.0f, 2.0f, 8.0f};
        float m_Yaw            = 0.0f; // degrees, rotation around world Y. 0 = looking towards -Z
        float m_Pitch          = 0.0f; // degrees, rotation around the camera's local X

        // Controls
        float m_MoveSpeed        = 5.0f;  // world units per second
        float m_MouseSensitivity = 0.15f; // degrees per pixel
        bool m_Looking           = false;
        glm::vec2 m_LastMousePosition{0.0f};
    };
} // namespace Orion
