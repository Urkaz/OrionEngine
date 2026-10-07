#pragma once

#include <cstddef>
#include <cstdint>

namespace Orion
{
    class RenderCommandQueue
    {
    public:
        using RenderCommandFn = void (*)(void* payload);

        static constexpr uint32_t DefaultCapacity = 10 * 1024 * 1024; // 10 MB
        static constexpr size_t PayloadAlignment  = alignof(std::max_align_t);

        explicit RenderCommandQueue(uint32_t capacity = DefaultCapacity);
        ~RenderCommandQueue();
        // Disable copy and assignment: prevents accidental copying of the command queue, which could lead to double deletion of the buffer and other issues.
        RenderCommandQueue(const RenderCommandQueue&)            = delete;
        RenderCommandQueue& operator=(const RenderCommandQueue&) = delete;

        void* Allocate(RenderCommandFn func, uint32_t size);

        // Runs every command in order and empties the queue.
        // Commands allocated while executing are appended and run in this same pass.
        void Execute();

        bool IsExecuting() const
        {
            return m_Executing;
        }
        uint32_t GetCommandCount() const
        {
            return m_CommandCount;
        }
        uint32_t GetUsedBytes() const
        {
            return m_Offset;
        }

    private:
        struct CommandHeader
        {
            RenderCommandFn Function;
            uint32_t PayloadSize;
        };

        unsigned char* m_CommandBuffer;
        uint32_t m_Offset           = 0; // next free byte (bytes used so far)
        unsigned int m_CommandCount = 0;
        uint32_t m_Capacity         = 0;
        bool m_Executing            = false;
    };

} // namespace Orion
