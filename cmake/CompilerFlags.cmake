# Engine compiler flags. Third-party libraries keep their own settings.
#
# RTTI is disabled engine-wide: no dynamic_cast/typeid, type identification is
# done with explicit enums or EnTT. Any third-party library linked in must match this setting.
#
# Exceptions are disabled too: errors go through asserts, error codes or optional. Third-party
# libraries built with exceptions (Assimp) must not let them escape their API.
# On MSVC, no _HAS_EXCEPTIONS=0: it changes std::exception, and mixing that with libraries that
# use exceptions (Assimp) is an ODR violation. The STL headers keep their throw/try, hence /wd4530.

# CMake has no per-target setting for MSVC's exception model: its default /EHsc lives in CMAKE_CXX_FLAGS
# and a target's /EHs-c- would only override it (warning D9025). So /EHsc is moved to a per-target
# generator expression: every C++ target gets it unless ORION_ENABLE_EXCEPTIONS is OFF.
# Unset counts as enabled, so third-party targets, which never set it, keep their exceptions.
# This file must be included from the root CMakeLists.txt before any dependency is added.
if(MSVC)
    string(
        REPLACE "/EHsc"
                ""
                CMAKE_CXX_FLAGS
                "${CMAKE_CXX_FLAGS}")
    add_compile_options(
        "$<$<AND:$<COMPILE_LANGUAGE:CXX>,$<NOT:$<STREQUAL:$<TARGET_PROPERTY:ORION_ENABLE_EXCEPTIONS>,OFF>>>:/EHsc>"
    )
endif()

# ENABLE_EXCEPTIONS: keep C++ exceptions for this target (only OrionTests: Catch2 aborts a failed REQUIRE by throwing).
function(target_set_compiler_flags)
    set(options ENABLE_EXCEPTIONS)
    set(oneValueArgs TARGET)
    cmake_parse_arguments(
        TARGET_SET_COMPILER_FLAGS
        "${options}"
        "${oneValueArgs}"
        "${multiValueArgs}"
        ${ARGN})

    set(MSVC_FLAGS /Gy # Enable function-level linking
                   /GR- # Disable RTTI
    )
    set(CLANG_FLAGS -fno-rtti)

    if(NOT TARGET_SET_COMPILER_FLAGS_ENABLE_EXCEPTIONS)
        # MSVC: drops the /EHsc added above
        set_target_properties(${TARGET_SET_COMPILER_FLAGS_TARGET} PROPERTIES ORION_ENABLE_EXCEPTIONS
                                                                             OFF)
        list(
            APPEND
            MSVC_FLAGS
            /EHs-c- # Disable exceptions
            /wd4530 # C++ exception handler used, but unwind semantics are not enabled. (from the STL headers)
        )
        list(APPEND CLANG_FLAGS -fno-exceptions)
    endif()

    set(GCC_FLAGS ${CLANG_FLAGS})

    if(CMAKE_CXX_COMPILER_ID MATCHES "MSVC")
        set(COMPILER_FLAGS ${MSVC_FLAGS})
    elseif(CMAKE_CXX_COMPILER_ID MATCHES "Clang")
        set(COMPILER_FLAGS ${CLANG_FLAGS})
    elseif(CMAKE_CXX_COMPILER_ID MATCHES "GNU")
        set(COMPILER_FLAGS ${GCC_FLAGS})
    endif()

    target_compile_options(${TARGET_SET_COMPILER_FLAGS_TARGET} PRIVATE ${COMPILER_FLAGS})

endfunction(target_set_compiler_flags)
