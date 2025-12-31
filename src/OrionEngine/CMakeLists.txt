set(LIBRARY_SOURCES
    "${CMAKE_CURRENT_SOURCE_DIR}/Orion/ImGui/ImGuiLayer.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Orion/Renderer/Buffer.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Orion/Renderer/Renderer.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Orion/Renderer/Shader.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Orion/Renderer/VertexArray.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Orion/Application.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Orion/Layer.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Orion/LayerStack.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Orion/Log.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Platform/OpenGL/OpenGLBuffer.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Platform/OpenGL/OpenGLContext.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Platform/OpenGL/OpenGLVertexArray.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Platform/Windows/WindowsInput.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Platform/Windows/WindowsWindow.cpp")
set(LIBRARY_HEADERS
    "${CMAKE_CURRENT_SOURCE_DIR}/Orion/Events/ApplicationEvent.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/Orion/Events/Event.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/Orion/Events/KeyEvent.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/Orion/Events/MouseEvent.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/Orion/ImGui/ImGuiLayer.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/Orion/Renderer/Buffer.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/Orion/Renderer/GraphicsContext.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/Orion/Renderer/Renderer.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/Orion/Renderer/Shader.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/Orion/Renderer/VertexArray.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/Orion/Application.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/Orion/Core.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/Orion/EntryPoint.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/Orion/Input.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/Orion/KeyCodes.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/Orion/Layer.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/Orion/LayerStack.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/Orion/Log.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/Orion/MouseCodes.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/Orion/Warnings.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/Orion/Window.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/Orion.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/Platform/OpenGL/OpenGLBuffer.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/Platform/OpenGL/OpenGLContext.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/Platform/OpenGL/OpenGLVertexArray.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/Platform/Windows/WindowsInput.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/Platform/Windows/WindowsWindow.h")

set(LIBRARY_INCLUDES "./")

add_library(${ENGINE_NAME} STATIC ${LIBRARY_SOURCES} ${LIBRARY_HEADERS})
target_include_directories(${ENGINE_NAME} PUBLIC ${LIBRARY_INCLUDES})

target_link_libraries(
    ${ENGINE_NAME}
    PUBLIC spdlog::spdlog ImGui_Core
    PRIVATE glfw
            OpenGL::GL
            GLAD
            ImGui_OpenGL3
            ImGui_GLFW
            glm::glm)

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
        ${ENABLE_WARNINGS_AS_ERRORS})
endif()
target_set_compiler_flags(TARGET ${ENGINE_NAME})

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

if(WIN32)
    target_compile_definitions(${ENGINE_NAME} PRIVATE OE_PLATFORM_WINDOWS)
endif()
target_compile_definitions(${ENGINE_NAME} PRIVATE $<$<NOT:$<CONFIG:Release>>:OE_ENABLE_LOGS>)
target_compile_definitions(${ENGINE_NAME} PRIVATE $<$<NOT:$<CONFIG:Release>>:OE_ENABLE_ASSERTS>)
target_compile_definitions(${ENGINE_NAME} PRIVATE GLFW_INCLUDE_NONE)
