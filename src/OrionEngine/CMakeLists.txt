add_library(${ENGINE_NAME} STATIC)

add_subdirectory(Orion)

target_sources(${ENGINE_NAME} PUBLIC Orion.h)

target_include_directories(${ENGINE_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

# Set Source Groups
get_target_property(ALL_SOURCES ${ENGINE_NAME} SOURCES)
foreach(FILE ${ALL_SOURCES})
    set_source_group(${FILE})
endforeach()

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

target_compile_definitions(${ENGINE_NAME} PRIVATE $<$<NOT:$<CONFIG:Release>>:OE_DEBUG>)
target_compile_definitions(${ENGINE_NAME} PRIVATE $<$<NOT:$<CONFIG:Release>>:OE_ENABLE_LOGS>)
target_compile_definitions(${ENGINE_NAME} PRIVATE $<$<NOT:$<CONFIG:Release>>:OE_ENABLE_ASSERTS>)
target_compile_definitions(${ENGINE_NAME} PRIVATE GLFW_INCLUDE_NONE)
