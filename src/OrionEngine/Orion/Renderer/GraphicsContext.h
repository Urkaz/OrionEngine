#pragma once

#include "Orion/Core/Core.h"

namespace Orion
{
    class GraphicsContext
    {
    public:
        GraphicsContext()          = default;
        virtual ~GraphicsContext() = default;
        OE_NON_COPYABLE(GraphicsContext);

        virtual void Init()        = 0;
        virtual void SwapBuffers() = 0;

        [[nodiscard]] static UniquePtr<GraphicsContext> Create(void* window);
    };
} // namespace Orion
