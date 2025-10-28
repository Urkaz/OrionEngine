#pragma once

#include "Orion/Events/ApplicationEvent.h"
#include "Orion/Events/KeyEvent.h"
#include "Orion/Events/MouseEvent.h"
#include "Orion/Layer.h"

namespace OrionEngine
{
    class ORIONENGINE_API ImGuiLayer : public Layer
    {
    public:
        ImGuiLayer();
        ~ImGuiLayer();

        virtual void OnAttach() override;
        virtual void OnDetach() override;
        virtual void OnUpdate() override;
        virtual void OnEvent(Event& event) override;

    private:
        bool OnMouseButtonPressedEvent(MouseButtonPressedEvent& e);
        bool OnMouseButtonReleasedEvent(MouseButtonReleasedEvent& e);
        bool OnMouseMovedEvent(MouseMovedEvent& e);
        bool OnMouseScrolledEvent(MouseScrolledEvent& e);
        bool OnKeyPressedEvent(KeyPressedEvent& e);
        bool OnKeyReleasedEvent(KeyReleasedEvent& e);
        bool OnKeyTypedEvent(KeyTypedEvent& e);
        bool OnWindowResizeEvent(WindowResizeEvent& e);

    private:
        float m_Time = 0.0f;

        bool m_LeftCtrlDown   = false;
        bool m_RightCtrlDown  = false;
        bool m_LeftShiftDown  = false;
        bool m_RightShiftDown = false;
        bool m_LeftAltDown    = false;
        bool m_RightAltDown   = false;
        bool m_LeftSuperDown  = false;
        bool m_RightSuperDown = false;
    };
} // namespace OrionEngine
