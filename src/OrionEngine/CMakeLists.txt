set(LIBRARY_SOURCES
    "${CMAKE_CURRENT_SOURCE_DIR}/Orion/Application.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Orion/Layer.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Orion/LayerStack.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Orion/Log.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Platform/Windows/WindowsWindow.cpp")
set(LIBRARY_HEADERS
    "${CMAKE_CURRENT_SOURCE_DIR}/Orion/Application.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/Orion/Core.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/Orion/EntryPoint.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/Orion/Layer.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/Orion/LayerStack.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/Orion/Log.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/Orion/Events/ApplicationEvent.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/Orion/Events/Event.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/Orion/Events/KeyEvent.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/Orion/Events/MouseEvent.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/Orion/Window.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/Orion.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/Platform/Windows/WindowsWindow.h")

set(LIBRARY_INCLUDES "./")

add_library(${ENGINE_NAME} SHARED ${LIBRARY_SOURCES} ${LIBRARY_HEADERS})
target_include_directories(${ENGINE_NAME} PUBLIC ${LIBRARY_INCLUDES})

target_link_libraries(${ENGINE_NAME} PUBLIC spdlog::spdlog glfw OpenGL::GL)

if(WIN32)
    target_link_libraries(${ENGINE_NAME} PRIVATE gdi32 user32 kernel32)
endif()

# Precompiled header
target_precompile_headers(${ENGINE_NAME} PUBLIC "oepch.h")

# Misc settings
setup_output_dirs(${ENGINE_NAME})

if(${ENABLE_WARNINGS})
    target_set_warnings(
        TARGET
        ${ENGINE_NAME}
        ENABLE
        ${ENABLE_WARNINGS}
        AS_ERRORS
        ${ENABLE_WARNINGS_AS_ERRORS}
        DISABLE_EXTRA
        ON)
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
add_compile_definitions("OE_PLATFORM_WINDOWS=1")
add_compile_definitions("OE_BUILD_DLL=1")
add_compile_definitions("OE_ENABLE_LOGS=1")
add_compile_definitions("OE_ENABLE_ASSERTS=1")
