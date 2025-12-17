#pragma once

namespace OrionEngine
{
    class GraphicsContext
    {
    public:
        virtual ~GraphicsContext() = default;

        virtual void Init()        = 0;
        virtual void SwapBuffers() = 0;
    };
} // namespace OrionEngine
