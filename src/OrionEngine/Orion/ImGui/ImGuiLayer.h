#pragma once

#include "Orion/Events/ApplicationEvent.h"
#include "Orion/Events/KeyEvent.h"
#include "Orion/Events/MouseEvent.h"
#include "Orion/Layer.h"

namespace Orion
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
    };
} // namespace Orion
