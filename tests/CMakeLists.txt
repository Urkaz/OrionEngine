set(TEST_NAME "OrionTests")

set(TEST_SOURCES
    "${CMAKE_CURRENT_SOURCE_DIR}/TestSetup.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Renderer/CameraTests.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Renderer/MeshTests.cpp")

add_executable(${TEST_NAME} ${TEST_SOURCES})

foreach(FILE ${TEST_SOURCES})
    set_source_group(${FILE})
endforeach()

target_link_libraries(${TEST_NAME} PRIVATE ${ENGINE_NAME} Catch2::Catch2WithMain assimp::assimp)

setup_output_dirs(${TEST_NAME})

if(ENABLE_WARNINGS)
    target_set_warnings(
        TARGET
        ${TEST_NAME}
        ENABLE
        ${ENABLE_WARNINGS}
        AS_ERRORS
        ${ENABLE_WARNINGS_AS_ERRORS})
endif()
target_set_compiler_flags(TARGET ${TEST_NAME})

include(Catch)
catch_discover_tests(${TEST_NAME})

if(ENABLE_COVERAGE)
    setup_target_for_coverage_gcovr_html(
        NAME
        coverage
        EXECUTABLE
        ${TEST_NAME}
        DEPENDENCIES
        ${TEST_NAME}
        EXCLUDE
        "src/ThirdParty/*"
        "tests/*"
        "build-cpm-cache/*"
        "out/*")
endif()
