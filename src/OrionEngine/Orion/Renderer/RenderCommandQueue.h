#pragma once

#include <functional>
#include <memory>
#include <mutex>
#include <vector>

namespace Orion
{
    class RenderCommandQueue
    {
    public:
        typedef void (*RenderCommandFn)(void*);

        RenderCommandQueue();
        ~RenderCommandQueue();

        void* Allocate(RenderCommandFn func, unsigned int size);

        void Execute();

    private:
        unsigned char* m_CommandBuffer;
        unsigned char* m_CommandBufferPtr;
        unsigned int m_CommandCount      = 0;
        unsigned int m_CommandBufferSize = 0;
    };

} // namespace Orion
