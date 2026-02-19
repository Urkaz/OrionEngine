#pragma once

#include "Orion/Core/Events/ApplicationEvent.h"
#include "Orion/Core/Events/KeyEvent.h"
#include "Orion/Core/Events/MouseEvent.h"
#include "Orion/Core/Layer.h"

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
