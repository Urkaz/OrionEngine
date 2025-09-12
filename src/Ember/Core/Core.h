#pragma once

#ifdef EMBER_PLATFORM_WINDOWS
#ifdef EMBER_BUILD_DLL
#define EMBER_API __declspec(dllexport)
#else
#define EMBER_API __declspec(dllimport)
#endif
#else
#error Only Winmdows is supported for now!
#endif