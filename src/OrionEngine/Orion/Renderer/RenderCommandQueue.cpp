#include "oepch.h"
#include "Orion/Renderer/RenderCommandQueue.h"

#include <cstdint>
#include <cstring>

namespace Orion
{
    static constexpr unsigned int DEFAULT_COMMAND_BUFFER_SIZE = 10 * 1024 * 1024; // 10 MB

    RenderCommandQueue::RenderCommandQueue()
    {
        m_CommandBufferSize = DEFAULT_COMMAND_BUFFER_SIZE;
        m_CommandBuffer     = new unsigned char[m_CommandBufferSize];
        m_CommandBufferPtr  = m_CommandBuffer;
        memset(m_CommandBuffer, 0, m_CommandBufferSize);
    }

    RenderCommandQueue::~RenderCommandQueue()
    {
        delete[] m_CommandBuffer;
    }

    void* RenderCommandQueue::Allocate(RenderCommandFn func, unsigned int size)
    {
        OE_PROFILE_FUNCTION();

        // We store: [RenderCommandFn][size:uint32][padding to alignment][object bytes]

        const uintptr_t base     = reinterpret_cast<uintptr_t>(m_CommandBuffer);
        uintptr_t writePtr = reinterpret_cast<uintptr_t>(m_CommandBufferPtr);

        // Check if we have enough buffer space to store everything
        if (writePtr + sizeof(RenderCommandFn) + sizeof(unsigned int) + size > base + m_CommandBufferSize)
            return nullptr; // Out of buffer

        // Write RenderCommandFn pointer and advance writePtr
        std::memcpy(reinterpret_cast<void*>(writePtr), &func, sizeof(RenderCommandFn));
        writePtr += sizeof(RenderCommandFn);

        // Write int representing the size and advance writePtr
        std::memcpy(reinterpret_cast<void*>(writePtr), &size, sizeof(unsigned int));
        writePtr += sizeof(unsigned int);

        // Align the payload to max alignment so any object can be safely created with placement-new here
        const size_t align = alignof(std::max_align_t);
        const uintptr_t aligned  = (writePtr + (align - 1)) & ~(align - 1);

        // Check again if we have enough space, but this time using the aligned ptr
        if (aligned + size > base + m_CommandBufferSize)
            return nullptr; // out of buffer

        void* memory = reinterpret_cast<void*>(aligned);

        // Advance the buffer pointer past the object
        m_CommandBufferPtr = reinterpret_cast<unsigned char*>(aligned + size);

        m_CommandCount++;
        return memory;
    }

    void RenderCommandQueue::Execute()
    {
        OE_PROFILE_FUNCTION();

        // OE_CORE_LOG(Trace, "RenderCommandQueue::Execute -- {0} commands, {1} bytes", m_CommandCount, (m_CommandBufferPtr - m_CommandBuffer));

        unsigned char* buffer = m_CommandBuffer;

        for (unsigned int i = 0; i < m_CommandCount; i++)
        {
            RenderCommandFn function = nullptr;
            std::memcpy(&function, buffer, sizeof(RenderCommandFn));
            buffer += sizeof(RenderCommandFn);

            unsigned int size = 0;
            std::memcpy(&size, buffer, sizeof(unsigned int));
            buffer += sizeof(unsigned int);

            // Align to the same alignment used in Allocate
            const size_t align = alignof(std::max_align_t);
            const uintptr_t writePtr = reinterpret_cast<uintptr_t>(buffer);
            const uintptr_t aligned  = (writePtr + (align - 1)) & ~(align - 1);

            unsigned char* payload = reinterpret_cast<unsigned char*>(aligned);
            function(payload);

            // Advance buffer pointer past the current object
            buffer = payload + size;
        }

        m_CommandBufferPtr = m_CommandBuffer;
        m_CommandCount     = 0;
    }

} // namespace Orion
