set(LIBRARY_SOURCES
    "${CMAKE_CURRENT_SOURCE_DIR}/Orion/ImGui/ImGuiLayer.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Orion/Renderer/Buffer.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Orion/Renderer/OrthographicCamera.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Orion/Renderer/RenderCommand.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Orion/Renderer/Renderer.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Orion/Renderer/RendererAPI.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Orion/Renderer/Shader.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Orion/Renderer/Texture.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Orion/Renderer/VertexArray.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Orion/Core/Application.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Orion/Core/Core.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Orion/Core/Layer.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Orion/Core/LayerStack.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Orion/Core/Log.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Orion/OrthographicCameraController.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Orion/Platform/OpenGL/OpenGLBuffer.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Orion/Platform/OpenGL/OpenGLContext.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Orion/Platform/OpenGL/OpenGLRendererAPI.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Orion/Platform/OpenGL/OpenGLShader.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Orion/Platform/OpenGL/OpenGLTexture.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Orion/Platform/OpenGL/OpenGLVertexArray.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Orion/Platform/Windows/WindowsInput.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Orion/Platform/Windows/WindowsWindow.cpp")
set(LIBRARY_HEADERS
    "${CMAKE_CURRENT_SOURCE_DIR}/Orion/Core/Events/ApplicationEvent.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/Orion/Core/Events/Event.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/Orion/Core/Events/KeyEvent.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/Orion/Core/Events/MouseEvent.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/Orion/ImGui/ImGuiLayer.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/Orion/Renderer/Buffer.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/Orion/Renderer/GraphicsContext.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/Orion/Renderer/OrthographicCamera.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/Orion/Renderer/RenderCommand.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/Orion/Renderer/Renderer.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/Orion/Renderer/RendererAPI.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/Orion/Renderer/Shader.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/Orion/Renderer/Texture.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/Orion/Renderer/VertexArray.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/Orion/Core/Application.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/Orion/Core/Core.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/Orion/EntryPoint.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/Orion/Core/Input.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/Orion/Core/KeyCodes.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/Orion/Core/Layer.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/Orion/Core/LayerStack.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/Orion/Core/Log.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/Orion/Core/MouseCodes.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/Orion/Core/Warnings.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/Orion/Core/Window.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/Orion/OrthographicCameraController.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/Orion.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/Orion/Platform/OpenGL/OpenGLBuffer.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/Orion/Platform/OpenGL/OpenGLContext.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/Orion/Platform/OpenGL/OpenGLRendererAPI.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/Orion/Platform/OpenGL/OpenGLShader.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/Orion/Platform/OpenGL/OpenGLTexture.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/Orion/Platform/OpenGL/OpenGLVertexArray.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/Orion/Platform/Windows/WindowsInput.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/Orion/Platform/Windows/WindowsWindow.h")

set(LIBRARY_INCLUDES "./")

add_library(${ENGINE_NAME} STATIC ${LIBRARY_SOURCES} ${LIBRARY_HEADERS})
target_include_directories(${ENGINE_NAME} PUBLIC ${LIBRARY_INCLUDES})

target_link_libraries(
    ${ENGINE_NAME}
    PUBLIC spdlog::spdlog ImGui_Core glm::glm
    PRIVATE glfw
            OpenGL::GL
            GLAD
            ImGui_OpenGL3
            ImGui_GLFW
            stb_image)

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
