// -------------------------
// Macro to disable a specific warning for a single file.
// -------------------------
#if defined(_MSC_VER)
    #define DISABLE_WARNING_FILE(msvc_warning, gcc_warning) __pragma(warning(disable : msvc_warning))
#elif defined(__GNUC__) || defined(__clang__)
    #define DO_PRAGMA(x) _Pragma(#x)
    #define DISABLE_WARNING_FILE(msvc_warning, gcc_warning) DO_PRAGMA(GCC diagnostic ignored gcc_warning)
#else
    #define DISABLE_WARNING_FILE(msvc_warning, gcc_warning)
#endif
