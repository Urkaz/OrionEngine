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

#define BIT(x) (1 << x)
