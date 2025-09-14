set(LIBRARY_SOURCES
    "${CMAKE_CURRENT_SOURCE_DIR}/Core/Application.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Core/Log.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Platform/Windows/WindowsWindow.cpp")
set(LIBRARY_HEADERS
    "${CMAKE_CURRENT_SOURCE_DIR}/Core/Application.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/Core/Core.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/Core/EntryPoint.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/Core/Log.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/Core/Events/ApplicationEvent.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/Core/Events/Event.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/Core/Events/KeyEvent.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/Core/Events/MouseEvent.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/Core/Window.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/ArcEngine.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/Platform/Windows/WindowsWindow.h")

set(LIBRARY_INCLUDES "./")

add_library(${ENGINE_NAME} SHARED ${LIBRARY_SOURCES} ${LIBRARY_HEADERS})
target_include_directories(${ENGINE_NAME} PUBLIC ${LIBRARY_INCLUDES})

target_link_libraries(${ENGINE_NAME} PUBLIC spdlog::spdlog glfw OpenGL::GL)

if(WIN32)
    target_link_libraries(${ENGINE_NAME} PRIVATE gdi32 user32 kernel32)
endif()

# Precompiled header
target_precompile_headers(${ENGINE_NAME} PUBLIC "aepch.h")

# Misc settings
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
add_compile_definitions("AE_PLATFORM_WINDOWS=1")
add_compile_definitions("AE_BUILD_DLL=1")
add_compile_definitions("AE_ENABLE_LOGS=1")
add_compile_definitions("AE_ENABLE_ASSERTS=1")
