#pragma once

#ifdef AE_PLATFORM_WINDOWS
    #ifdef AE_BUILD_DLL
        #define ARCENGINE_API __declspec(dllexport)
    #else
        #define ARCENGINE_API __declspec(dllimport)
    #endif
#else
    #error Only Windows is supported for now!
#endif

#ifdef AE_ENABLE_ASSERTS
    #define AE_ASSERT(x, ...) { if(!(x)) { AE_CORE_LOG(error, "Assertion Failed: {0}", __VA_ARGS__); __debugbreak(); } }
    #define AE_CORE_ASSERT(x, ...) { if(!(x)) { AE_CORE_LOG(error, "Assertion Failed: {0}", __VA_ARGS__); __debugbreak(); } }
#else
    #define AE_ASSERT(x, ...)
    #define AE_CORE_ASSERT(x, ...)
#endif


#define BIT(x) (1 << x)
