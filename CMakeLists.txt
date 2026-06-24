cmake_minimum_required(VERSION 3.21)

project(
    "OrionEngine"
    VERSION 1.0.0
    LANGUAGES C CXX)

# Global CMake variables
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

if(MINGW)
    add_compile_definitions(_GLIBCXX_USE_CXX11_ABI=0)
endif()

# Use a single CPM download cache between this and any other projects that use CPM. Avoids redownloading dependancies.
if(NOT DEFINED ENV{CPM_SOURCE_CACHE})
    set(ENV{CPM_SOURCE_CACHE} "${CMAKE_SOURCE_DIR}/build-cpm-cache")
endif()

# Options
option(ENABLE_WARNINGS "Enable to add warnings to a target." ON)
option(ENABLE_WARNINGS_AS_ERRORS "Enable to treat warnings as errors." ON)

option(ENABLE_TESTING "Enable a Unit Testing build." OFF)
option(ENABLE_COVERAGE "Enable a Code Coverage build." OFF)

option(ENABLE_CLANG_TIDY "Enable to add clang tidy." ON)

option(ENABLE_DOCS "Enable to generate docs." ON)

option(ENABLE_PROFILING "Enable Tracy profiling instrumentation in Debug builds and the tracy-profiler GUI target." ON)

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
include(SetSourceGroup)

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

include(CPM)
cpmaddpackage("gh:gabime/spdlog#v1.16.0")
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
    cpmaddpackage("gh:catchorg/Catch2#v3.11.0")
    set_target_properties(Catch2 PROPERTIES FOLDER "Dependencies")
    list(APPEND CMAKE_MODULE_PATH ${Catch2_SOURCE_DIR}/extras)
endif()

find_package(OpenGL REQUIRED)

add_subdirectory(src)
