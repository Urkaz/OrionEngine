#pragma once

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

        void Begin();
        void End();
    };
} // namespace Orion
