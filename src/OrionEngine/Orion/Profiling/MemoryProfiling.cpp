#include "Orion/Profiling/Profiling.h"

#if defined(OE_ENABLE_PROFILING)

    #include <cstdlib>
    #include <new>

// Replaces the global allocator so every new/delete in the process (engine, sandbox and any
// statically-linked third-party code) shows up in Tracy's memory profiler. Uses the "secure"
// variants since static/global objects can free memory after the profiler has shut down.

void* operator new(std::size_t count)
{
    void* ptr = std::malloc(count);
    if (!ptr)
        throw std::bad_alloc();

    TracySecureAlloc(ptr, count);
    return ptr;
}

void* operator new[](std::size_t count)
{
    void* ptr = std::malloc(count);
    if (!ptr)
        throw std::bad_alloc();

    TracySecureAlloc(ptr, count);
    return ptr;
}

void operator delete(void* ptr) noexcept
{
    TracySecureFree(ptr);
    std::free(ptr);
}

void operator delete[](void* ptr) noexcept
{
    TracySecureFree(ptr);
    std::free(ptr);
}

void operator delete(void* ptr, std::size_t) noexcept
{
    TracySecureFree(ptr);
    std::free(ptr);
}

void operator delete[](void* ptr, std::size_t) noexcept
{
    TracySecureFree(ptr);
    std::free(ptr);
}

#endif