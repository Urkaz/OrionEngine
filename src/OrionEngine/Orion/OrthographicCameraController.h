#pragma once

#include "Orion/Core/Timestep.h"
#include "Orion/Renderer/OrthographicCamera.h"

#include "Orion/Core/Events/ApplicationEvent.h"
#include "Orion/Core/Events/MouseEvent.h"

namespace Orion
{
    class OrthographicCameraController
    {
    public:
        OrthographicCameraController(float aspectRatio, bool rotationEnabled = true);

        void OnUpdate(Timestep ts);
        void OnEvent(Event& event);

        OrthographicCamera& GetCamera()
        {
            return m_Camera;
        };
        const OrthographicCamera& GetCamera() const
        {
            return m_Camera;
        };

        void SetZoomLevel(float level)
        {
            m_ZoomLevel = level;
        }
        float GetZoomLevel() const
        {
            return m_ZoomLevel;
        }

    private:
        bool OnMouseScrolled(MouseScrolledEvent& e);
        bool OnWindowResize(WindowResizeEvent& e);

    private:
        float m_AspectRatio;
        float m_ZoomLevel = 1.0f;
        OrthographicCamera m_Camera;

        bool m_RotationEnabled;

        glm::vec3 m_CameraPosition = {0.0f, 0.0f, 0.0f};
        float m_CameraRotation     = 0.0f; //In degrees, in the anti-clockwise direction

        float m_CameraTranslationSpeed = 20.0f;
        float m_CameraRotationSpeed    = 45.0f;
    };
} // namespace Orion
