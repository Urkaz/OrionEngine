#pragma once

#include "Orion/Events/ApplicationEvent.h"
#include "Orion/Events/KeyEvent.h"
#include "Orion/Events/MouseEvent.h"
#include "Orion/Layer.h"

namespace OrionEngine
{
    class ImGuiLayer : public Layer
    {
    public:
        ImGuiLayer();
        ~ImGuiLayer();

        virtual void OnAttach() override;
        virtual void OnDetach() override;
        virtual void OnImguiRender() override;

        void Begin();
        void End();


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
