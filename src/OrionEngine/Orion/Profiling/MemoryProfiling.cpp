#if defined(OE_ENABLE_PROFILING)
    #include <cstdlib>
    #include <new>
    #include <tracy/Tracy.hpp>

    #if defined(_WIN32)
        #include <malloc.h>
    #endif

// Replaces the global allocator so every new/delete in the process (engine, sandbox and any
// statically-linked third-party code) shows up in Tracy's memory profiler. Uses the "secure"
// variants since static/global objects can free memory after the profiler has shut down.

// NOLINTBEGIN(cppcoreguidelines-no-malloc, cppcoreguidelines-owning-memory)
namespace
{
    // Frames captured per allocation. This makes allocations more expensive.
    constexpr int AllocCallstackDepth = 12;
} // namespace

// Base operator new. The default array and nothrow variants of operator new forward here.
void* operator new(std::size_t count)
{
    // new must return a unique non-null pointer even for zero bytes; malloc(0) may return null.
    void* ptr = std::malloc(count != 0 ? count : 1);
    if (!ptr)
        std::abort();

    TracySecureAllocS(ptr, count, AllocCallstackDepth);
    return ptr;
}

// Required: the default aligned new bypasses operator new(size) and would be invisible to Tracy.
void* operator new(std::size_t count, std::align_val_t alignment)
{
    const auto align       = static_cast<std::size_t>(alignment);
    const std::size_t size = count != 0 ? count : 1;
    #if defined(_WIN32)
    // MSVC has no std::aligned_alloc; must be paired with _aligned_free, never free().
    void* ptr = _aligned_malloc(size, align);
    #else
    // aligned_alloc requires the size to be a multiple of the alignment.
    void* ptr = std::aligned_alloc(align, (size + align - 1) / align * align);
    #endif
    if (!ptr)
        std::abort();

    TracySecureAllocS(ptr, count, AllocCallstackDepth);
    return ptr;
}

// Base operator delete. The default array, nothrow and sized variants of operator delete forward here.
void operator delete(void* ptr) noexcept
{
    if (!ptr)
        return;

    TracySecureFree(ptr);
    std::free(ptr);
}

// Required: the default aligned delete bypasses operator delete(size) and would be invisible to Tracy.
void operator delete(void* ptr, std::align_val_t) noexcept
{
    if (!ptr)
        return;

    TracySecureFree(ptr);
    #if defined(_WIN32)
    _aligned_free(ptr);
    #else
    std::free(ptr);
    #endif
}
// NOLINTEND(cppcoreguidelines-no-malloc, cppcoreguidelines-owning-memory)

#endif
