#pragma once

#include "Orion/Core/Core.h"
#include "Orion/Core/Events/Event.h"
#include "Orion/Core/Timestep.h"

namespace Orion
{
    class Layer
    {
    public:
        Layer(std::string name = "Layer");
        virtual ~Layer();

        virtual void OnAttach() {}
        virtual void OnDetach() {}
        virtual void OnUpdate([[maybe_unused]] Timestep ts) {}
        virtual void OnImguiRender() {}
        virtual void OnEvent([[maybe_unused]] Event& event) {}

        inline const std::string& GetName() const
        {
            return m_DebugName;
        }

    protected:
        std::string m_DebugName;
    };
} // namespace Orion
