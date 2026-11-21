#pragma once

#include "Orion/Core/Events/Event.h"
#include "Orion/Core/Timestep.h"

namespace Orion
{
    class Layer
    {
    public:
        explicit Layer(std::string name = "Layer");
        virtual ~Layer();

        virtual void OnAttach() {}
        virtual void OnDetach() {}
        virtual void OnUpdate([[maybe_unused]] Timestep ts) {}
        virtual void OnImGuiRender() {}
        virtual void OnEvent([[maybe_unused]] Event& event) {}

        [[nodiscard]] inline const std::string& GetName() const
        {
            return m_DebugName;
        }

    protected:
        // Polymorphic base with data: only derived classes can copy or move (C++ Core Guidelines C.67)
        Layer(const Layer&)            = default;
        Layer& operator=(const Layer&) = default;
        Layer(Layer&&)                 = default;
        Layer& operator=(Layer&&)      = default;

    private:
        std::string m_DebugName;
    };
} // namespace Orion
