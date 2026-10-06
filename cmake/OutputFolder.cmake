function(setup_output_dirs target)
    # One output folder per preset, so presets sharing config and compiler (e.g. linux-clang-debug and
    # linux-clang-asan) never overwrite each other's binaries. Falls back to the build dir name when
    # configuring without presets.
    if(ORION_PRESET_NAME)
        set(OUTPUT_NAME "${ORION_PRESET_NAME}")
    else()
        get_filename_component(OUTPUT_NAME "${CMAKE_BINARY_DIR}" NAME)
    endif()
    set(OUTPUT_DIR "${CMAKE_SOURCE_DIR}/bin/${OUTPUT_NAME}/$<CONFIG>/${CMAKE_PROJECT_NAME}")

    set_target_properties(
        ${target}
        PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${OUTPUT_DIR}"
                   LIBRARY_OUTPUT_DIRECTORY "${OUTPUT_DIR}"
                   ARCHIVE_OUTPUT_DIRECTORY "${OUTPUT_DIR}")

endfunction(setup_output_dirs)
