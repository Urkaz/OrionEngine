set(LIBRARY_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/SandboxApp.cpp")
set(LIBRARY_HEADERS)
#set(LIBRARY_INCLUDES "./" "${CMAKE_BINARY_DIR}/configured_files/include")

add_executable(${SANDBOX_NAME} ${LIBRARY_SOURCES} ${LIBRARY_HEADERS})
target_include_directories(${SANDBOX_NAME} PUBLIC)

target_link_libraries(${SANDBOX_NAME} PRIVATE ${ENGINE_NAME})

setup_output_dirs(${SANDBOX_NAME})

if(${ENABLE_WARNINGS})
    target_set_warnings(
        TARGET
        ${SANDBOX_NAME}
        ENABLE
        ${ENABLE_WARNINGS}
        AS_ERRORS
        ${ENABLE_WARNINGS_AS_ERRORS})
endif()

if(${ENABLE_LTO})
    target_enable_lto(
        TARGET
        ${SANDBOX_NAME}
        ENABLE
        ON)
endif()

if(${ENABLE_CLANG_TIDY})
    add_clang_tidy_to_target(${SANDBOX_NAME})
endif()

# Temp
add_compile_definitions("EMBER_PLATFORM_WINDOWS=1")
