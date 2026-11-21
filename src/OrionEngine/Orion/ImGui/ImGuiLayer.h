#pragma once

#include "Orion/Core/Core.h"
#include "Orion/Core/Layer.h"

namespace Orion
{
    class ImGuiLayer : public Layer
    {
    public:
        ImGuiLayer();
        ~ImGuiLayer() override;
        OE_NON_COPYABLE(ImGuiLayer);

        void OnAttach() override;
        void OnDetach() override;

        void Begin();
        void End();
    };
} // namespace Orion
