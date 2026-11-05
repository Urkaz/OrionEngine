function(target_set_compiler_flags)
    set(oneValueArgs TARGET)
    cmake_parse_arguments(
        TARGET_SET_COMPILER_FLAGS
        "${options}"
        "${oneValueArgs}"
        "${multiValueArgs}"
        ${ARGN})

    # RTTI is disabled engine-wide: no dynamic_cast/typeid, type identification is
    # done with explicit enums or EnTT. Any third-party library linked in must match this setting.
    set(MSVC_FLAGS
        /Gy # Enable function-level linking
        /GR- # Disable RTTI
    )

    set(CLANG_FLAGS -fno-rtti)

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
