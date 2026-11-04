#pragma once

// Assert macros live apart from Core.h because they expand to OE_CORE_LOG, and Log.h already
// includes Core.h: defining them there would require a circular include.

#include "Orion/Core/Core.h"
#include "Orion/Core/Log.h"

#ifdef OE_PLATFORM_WINDOWS
    #define OE_DEBUGBREAK() __debugbreak()
#elif defined(OE_PLATFORM_LINUX)
    #include <csignal>
    #define OE_DEBUGBREAK() raise(SIGTRAP)
#endif

#ifdef OE_ENABLE_ASSERTS
    #define OE_ASSERT(x, ...)                                                                                          \
        do                                                                                                             \
        {                                                                                                              \
            if (!(x))                                                                                                  \
            {                                                                                                          \
                OE_LOG(Error, "Assertion Failed: " __VA_ARGS__);                                                       \
                OE_LOG(Error, "  at {}:{} -> {}", __FILE__, __LINE__, #x);                                             \
                OE_DEBUGBREAK();                                                                                       \
            }                                                                                                          \
        } while (false)
    #define OE_CORE_ASSERT(x, ...)                                                                                     \
        do                                                                                                             \
        {                                                                                                              \
            if (!(x))                                                                                                  \
            {                                                                                                          \
                OE_CORE_LOG(Error, "Assertion Failed: " __VA_ARGS__);                                                  \
                OE_CORE_LOG(Error, "  at {}:{} -> {}", __FILE__, __LINE__, #x);                                        \
                OE_DEBUGBREAK();                                                                                       \
            }                                                                                                          \
        } while (false)
#else
    #define OE_ASSERT(x, ...)
    #define OE_CORE_ASSERT(x, ...)
#endif // End of assert macros

// Like OE_CORE_ASSERT, but always checked (also in builds without asserts).
// For unrecoverable errors where continuing would be undefined behaviour.

#ifdef OE_ENABLE_LOGS
    #define OE_INTERNAL_VERIFY_REPORT(x, ...)                                                                          \
        OE_CORE_LOG(Critical, "Verify failed: " __VA_ARGS__);                                                          \
        OE_CORE_LOG(Critical, "  at {}:{} -> {}", __FILE__, __LINE__, #x)
#else
    #define OE_INTERNAL_VERIFY_REPORT(x, ...)                                                                          \
        spdlog::fmt_lib::print(stderr, "Verify failed: " __VA_ARGS__);                                                 \
        spdlog::fmt_lib::print(stderr, "\n  at {}:{} -> {}\n", __FILE__, __LINE__, #x)
#endif

#define OE_CORE_VERIFY(x, ...)                                                                                         \
    do                                                                                                                 \
    {                                                                                                                  \
        if (!(x))                                                                                                      \
        {                                                                                                              \
            OE_INTERNAL_VERIFY_REPORT(x, __VA_ARGS__);                                                                 \
            std::abort();                                                                                              \
        }                                                                                                              \
    } while (false)
