#include "oepch.h"
#include "Orion/Renderer/RenderCommandQueue.h"

#include <cstdint>
#include <cstring>
#include <new>

namespace Orion
{
    namespace
    {
        // Rounds "value" up to the next multiple of "alignment" (a power of two)
        constexpr size_t AlignUp(size_t value, size_t alignment)
        {
            return (value + alignment - 1) & ~(alignment - 1);
        }
    } // namespace


    RenderCommandQueue::RenderCommandQueue(uint32_t capacity) : m_Capacity(capacity)
    {
        // Force alignment of the command buffer to PayloadAlignment
        m_CommandBuffer = static_cast<unsigned char*>(::operator new(m_Capacity, std::align_val_t{PayloadAlignment}));
        memset(m_CommandBuffer, 0, m_Capacity);
    }

    RenderCommandQueue::~RenderCommandQueue()
    {
        OE_CORE_ASSERT(m_CommandCount == 0, "RenderCommandQueue destroyed with pending commands (their captures leak)");
        ::operator delete(m_CommandBuffer, std::align_val_t{PayloadAlignment});
    }

    // NOLINTBEGIN(cppcoreguidelines-pro-bounds-pointer-arithmetic, cppcoreguidelines-pro-type-reinterpret-cast)

    void* RenderCommandQueue::Allocate(RenderCommandFn func, uint32_t size)
    {
        OE_PROFILE_FUNCTION();

        const size_t headerOffset  = AlignUp(m_Offset, alignof(CommandHeader));
        const size_t payloadOffset = AlignUp(headerOffset + sizeof(CommandHeader), PayloadAlignment);
        const size_t end           = payloadOffset + size;

        // Check again if we have enough space, but this time using the aligned ptr
        if (end > m_Capacity)
            return nullptr; // Out of buffer

        // The header is a trivial struct: construct it in place
        new (m_CommandBuffer + headerOffset) CommandHeader{.Function = func, .PayloadSize = size};

        // Advance the offset past the object
        m_Offset = static_cast<uint32_t>(payloadOffset + size);
        m_CommandCount++;
        return m_CommandBuffer + payloadOffset;
    }

    void RenderCommandQueue::Execute()
    {
        OE_PROFILE_FUNCTION();
        OE_CORE_ASSERT(!m_Executing, "RenderCommandQueue::Execute is not reentrant");

        m_Executing   = true;
        size_t offset = 0;

        for (unsigned int i = 0; i < m_CommandCount; i++)
        {
            const size_t headerOffset = AlignUp(offset, alignof(CommandHeader));
            // launder: tells the compiler a CommandHeader lives here (built with placement new in Allocate)
            const auto* header = std::launder(reinterpret_cast<CommandHeader*>(m_CommandBuffer + headerOffset));

            const size_t payloadOffset = AlignUp(headerOffset + sizeof(CommandHeader), PayloadAlignment);
            header->Function(m_CommandBuffer + payloadOffset);

            // Advance past the current payload
            offset = payloadOffset + header->PayloadSize;
        }

        m_Offset       = 0;
        m_CommandCount = 0;
        m_Executing    = false;
    }

    // NOLINTEND(cppcoreguidelines-pro-bounds-pointer-arithmetic, cppcoreguidelines-pro-type-reinterpret-cast)

} // namespace Orion
