cmake_minimum_required(VERSION 4.0.0)

project(
    "OrionEngine"
    VERSION 1.0.0
    LANGUAGES C CXX)

# Global CMake variables
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_compile_definitions(_GLIBCXX_USE_CXX11_ABI=0)

# Options
option(USE_VCPKG "Whether to use VCPKG." OFF)
option(USE_CPM "Whether to use CPM." ON)
option(USE_CONAN "Whether to use Conan." OFF)

option(ENABLE_WARNINGS "Enable to add warnings to a target." ON)
option(ENABLE_WARNINGS_AS_ERRORS "Enable to treat warnings as errors." ON)

option(ENABLE_TESTING "Enable a Unit Testing build." OFF)
option(ENABLE_COVERAGE "Enable a Code Coverage build." ON)

option(ENABLE_CLANG_TIDY "Enable to add clang tidy." ON)

option(ENABLE_DOCS "Enable to generate docs." OFF)

option(ENABLE_SANITIZE_ADDR "Enable address sanitize." OFF)
option(ENABLE_SANITIZE_UNDEF "Enable undefined sanitize." OFF)
option(ENABLE_SANITIZE_LEAK "Enable leak sanitize (Gcc/Clang only)." OFF)
option(ENABLE_SANITIZE_THREAD "Enable thread sanitize (Gcc/Clang only)." OFF)

option(ENABLE_CLANG_FORMAT "Enable to add clang-format." ON)
option(ENABLE_CMAKE_FORMAT "Enable to add cmake-format." ON)

option(ENABLE_LTO "Enable to add Link Time Optimization." ON)

# Project/Library Names
set(ENGINE_NAME "Orion")
set(SANDBOX_NAME "Sandbox")

# CMAKE Modules
list(APPEND CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake/")
include(ConfigSafeGuards)
include(AddGitSubmodule)
include(Tools)
include(OutputFolder)
include(CompilerFlags)

if(ENABLE_DOCS)
    include(Docs)
endif()

if(ENABLE_WARNINGS)
    include(Warnings)
endif()

set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")

add_cmake_format_target()
add_clang_format_target()

if(ENABLE_SANITIZE_ADDR
   OR ENABLE_SANITIZE_UNDEF
   OR ENABLE_SANITIZE_LEAK
   OR ENABLE_SANITIZE_THREAD)
    include(Sanitizer)
    add_sanitizer_flags()
endif()

if(ENABLE_TESTING AND ENABLE_COVERAGE)
    include(CodeCoverage)
    append_coverage_compiler_flags()
endif()

if(ENABLE_LTO)
    include(LTO)
endif()

# External libraries

if(USE_CPM)
    message(STATUS "Using CPM")
    include(CPM)
    cpmaddpackage("gh:gabime/spdlog#v1.15.3")
    set_target_properties(spdlog PROPERTIES FOLDER "Dependencies")
    cpmaddpackage(
        NAME
        glfw
        GITHUB_REPOSITORY
        glfw/glfw
        GIT_TAG
        3.4
        OPTIONS
        "GLFW_BUILD_DOCS OFF"
        "GLFW_BUILD_TESTS OFF"
        "GLFW_BUILD_EXAMPLES OFF"
        "GLFW_INSTALL OFF")
    target_compile_definitions(glfw PRIVATE _CRT_SECURE_NO_WARNINGS _GLFW_WIN32)
    set_target_properties(glfw PROPERTIES FOLDER "Dependencies/GLFW")
    set_target_properties(update_mappings PROPERTIES FOLDER "Dependencies/GLFW")

    cpmaddpackage("gh:g-truc/glm#1.0.2")
    set_target_properties(glm PROPERTIES FOLDER "Dependencies")

    if(ENABLE_TESTING)
        cpmaddpackage("gh:catchorg/Catch2#v3.10.0")
        set_target_properties(Catch2 PROPERTIES FOLDER "Dependencies")
        list(APPEND CMAKE_MODULE_PATH ${Catch2_SOURCE_DIR}/extras)
    endif()
elseif(USE_CONAN OR USE_VCPKG)
    if(USE_CONAN)
        message(STATUS "Using Conan")
        include(${CMAKE_BINARY_DIR}/conan_toolchain.cmake)
    elseif(USE_VCPKG)
        message(STATUS "Using VCPKG")
        include(${CMAKE_SOURCE_DIR}/external/vcpkg/scripts/buildsystems/vcpkg.cmake)
    endif()

    find_package(spdlog CONFIG REQUIRED)
    find_package(glfw3 CONFIG REQUIRED)
    find_package(glm CONFIG REQUIRED)

    if(ENABLE_TESTING)
        find_package(Catch2 CONFIG REQUIRED)
        list(APPEND CMAKE_MODULE_PATH ${Catch2_SOURCE_DIR}/extras)
    endif()
else()
    message(STATUS "Using FetchContent")

    FetchContent_Declare(
        spdlog
        GIT_REPOSITORY https://github.com/gabime/spdlog
        GIT_TAG v1.15.3
        GIT_SHALLOW TRUE)
    FetchContent_MakeAvailable(spdlog)
    set_target_properties(spdlog PROPERTIES FOLDER "Dependencies")

    FetchContent_Declare(
        glfw
        GIT_REPOSITORY https://github.com/glfw/glfw
        GIT_TAG 3.4
        GIT_SHALLOW TRUE)
    set(GLFW_BUILD_DOCS
        OFF
        CACHE BOOL "Disable building docs for GLFW" FORCE)
    set(GLFW_BUILD_TESTS
        OFF
        CACHE BOOL "Disable building tests for GLFW" FORCE)
    set(GLFW_BUILD_EXAMPLES
        OFF
        CACHE BOOL "Disable building examples for GLFW" FORCE)
    set(GLFW_INSTALL
        OFF
        CACHE BOOL "Disable installing docs for GLFW" FORCE)
    FetchContent_MakeAvailable(glfw)
    set_target_properties(glfw PROPERTIES FOLDER "Dependencies")

    FetchContent_Declare(
        glm
        GIT_REPOSITORY https://github.com/g-truc/glm
        GIT_TAG 1.0.2
        GIT_SHALLOW TRUE)
    FetchContent_MakeAvailable(glm)
    set_target_properties(glm PROPERTIES FOLDER "Dependencies")

    if(ENABLE_TESTING)
        FetchContent_Declare(
            Catch2
            GIT_REPOSITORY https://github.com/catchorg/Catch2
            GIT_TAG v3.10.0
            GIT_SHALLOW TRUE)
        FetchContent_MakeAvailable(Catch2)
        set_target_properties(Catch2 PROPERTIES FOLDER "Dependencies")
        list(APPEND CMAKE_MODULE_PATH ${catch2_SOURCE_DIR}/extras)
    endif()
endif()

find_package(OpenGL REQUIRED)

add_subdirectory(src)
