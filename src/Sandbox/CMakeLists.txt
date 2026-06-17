set(LIBRARY_SOURCES
    "${CMAKE_CURRENT_SOURCE_DIR}/SandboxApp.cpp" "${CMAKE_CURRENT_SOURCE_DIR}/ExampleLayer.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Sandbox2D.cpp")
set(LIBRARY_HEADERS "${CMAKE_CURRENT_SOURCE_DIR}/Sandbox2D.h"
                    "${CMAKE_CURRENT_SOURCE_DIR}/ExampleLayer.h")

add_executable(${SANDBOX_NAME} ${LIBRARY_SOURCES} ${LIBRARY_HEADERS})

# Set Source Groups
foreach(FILE ${LIBRARY_SOURCES})
    set_source_group(${FILE})
endforeach()
foreach(FILE ${LIBRARY_HEADERS})
    set_source_group(${FILE})
endforeach()

target_link_libraries(${SANDBOX_NAME} PUBLIC ${ENGINE_NAME})

setup_output_dirs(${SANDBOX_NAME})

if(ENABLE_WARNINGS)
    target_set_warnings(
        TARGET
        ${SANDBOX_NAME}
        ENABLE
        ${ENABLE_WARNINGS}
        AS_ERRORS
        ${ENABLE_WARNINGS_AS_ERRORS})
endif()
target_set_compiler_flags(TARGET ${SANDBOX_NAME})

if(ENABLE_LTO)
    target_enable_lto(
        TARGET
        ${SANDBOX_NAME}
        ENABLE
        ON)
endif()

if(ENABLE_CLANG_TIDY)
    add_clang_tidy_to_target(${SANDBOX_NAME})
endif()


# Copy assets
add_custom_command(
    TARGET ${SANDBOX_NAME}
    POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_SOURCE_DIR}/assets
            $<TARGET_FILE_DIR:${SANDBOX_NAME}>/assets)
