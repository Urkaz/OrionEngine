#pragma once

#include "Orion/Core.h"
#include "Orion/Events/Event.h"

DISABLE_WARNING_FILE(4100, "-Wunused-parameter")

namespace Orion
{
    class Layer
    {
    public:
        Layer(std::string name = "Layer");
        virtual ~Layer();

        virtual void OnAttach() {}
        virtual void OnDetach() {}
        virtual void OnUpdate() {}
        virtual void OnImguiRender() {}
        virtual void OnEvent(Event& event) {}

        inline const std::string& GetName() const
        {
            return m_DebugName;
        }

    protected:
        std::string m_DebugName;
    };
} // namespace Orion
