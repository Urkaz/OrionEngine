#pragma once

#ifdef OE_PLATFORM_WINDOWS
    #ifdef OE_BUILD_DLL
        #define ORIONENGINE_API __declspec(dllexport)
    #else
        #define ORIONENGINE_API __declspec(dllimport)
    #endif
#else
    #error Only Windows is supported for now!
#endif

#ifdef OE_ENABLE_ASSERTS
    #define OE_ASSERT(x, ...)                                                                                          \
        {                                                                                                              \
            if (!(x))                                                                                                  \
            {                                                                                                          \
                OE_CORE_LOG(error, "Assertion Failed: {0}", __VA_ARGS__);                                              \
                __debugbreak();                                                                                        \
            }                                                                                                          \
        }
    #define OE_CORE_ASSERT(x, ...)                                                                                     \
        {                                                                                                              \
            if (!(x))                                                                                                  \
            {                                                                                                          \
                OE_CORE_LOG(error, "Assertion Failed: {0}", __VA_ARGS__);                                              \
                __debugbreak();                                                                                        \
            }                                                                                                          \
        }
#else
    #define OE_ASSERT(x, ...)
    #define OE_CORE_ASSERT(x, ...)
#endif

#define BIT(x) (1 << x)

#define OE_BIND_EVENT_FN(x) std::bind(&x, this, std::placeholders::_1)

