set(LIBRARY_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/Core/Application.cpp")
set(LIBRARY_HEADERS "${CMAKE_CURRENT_SOURCE_DIR}/Core/Application.h"
                    "${CMAKE_CURRENT_SOURCE_DIR}/Core/Core.h")
set(LIBRARY_INCLUDES "./")

add_library(${ENGINE_NAME} SHARED ${LIBRARY_SOURCES} ${LIBRARY_HEADERS})
target_include_directories(${ENGINE_NAME} PUBLIC ${LIBRARY_INCLUDES})

setup_output_dirs(${ENGINE_NAME})

if(${ENABLE_WARNINGS})
    target_set_warnings(
        TARGET
        ${ENGINE_NAME}
        ENABLE
        ${ENABLE_WARNINGS}
        AS_ERRORS
        ${ENABLE_WARNINGS_AS_ERRORS})
endif()

if(${ENABLE_LTO})
    target_enable_lto(
        TARGET
        ${ENGINE_NAME}
        ENABLE
        ON)
endif()

if(${ENABLE_CLANG_TIDY})
    add_clang_tidy_to_target(${ENGINE_NAME})
endif()

# Temp
add_compile_definitions("EMBER_PLATFORM_WINDOWS=1")
add_compile_definitions("EMBER_BUILD_DLL=1")
