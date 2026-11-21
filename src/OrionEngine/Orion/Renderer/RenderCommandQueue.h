#pragma once

#include "Orion/Core/Core.h"

#include <cstddef>
#include <cstdint>

namespace Orion
{
    class RenderCommandQueue
    {
    public:
        using RenderCommandFn = void (*)(void* payload);

        static constexpr uint32_t DefaultCapacity = 10 * 1024 * 1024; // 10 MB
        // Fixed 16 instead of alignof(std::max_align_t), which is 16 on GCC/Clang but 8 on MSVC: same layout on every platform
        static constexpr size_t PayloadAlignment = 16;
        static_assert(PayloadAlignment >= alignof(std::max_align_t), "Payloads must fit any fundamental type");

        explicit RenderCommandQueue(uint32_t capacity = DefaultCapacity);
        ~RenderCommandQueue();
        // A copy would delete the same buffer twice
        OE_NON_COPYABLE(RenderCommandQueue);

        void* Allocate(RenderCommandFn func, uint32_t size);

        // Runs every command in order and empties the queue.
        // Commands allocated while executing are appended and run in this same pass.
        void Execute();

        [[nodiscard]] bool IsExecuting() const
        {
            return m_Executing;
        }
        [[nodiscard]] uint32_t GetCommandCount() const
        {
            return m_CommandCount;
        }
        [[nodiscard]] uint32_t GetUsedBytes() const
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
