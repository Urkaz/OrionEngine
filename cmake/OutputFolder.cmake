function(setup_output_dirs target)
    # One output folder per build dir, so presets sharing config and compiler (e.g. linux-clang-debug and
    # linux-clang-asan) never overwrite each other's binaries. Presets use out/build/<preset>, so the
    # folder is named after the preset; a custom build dir (-B) for the same preset gets its own folder.
    get_filename_component(OUTPUT_NAME "${CMAKE_BINARY_DIR}" NAME)
    set(OUTPUT_DIR "${CMAKE_SOURCE_DIR}/bin/${OUTPUT_NAME}/$<CONFIG>/${CMAKE_PROJECT_NAME}")

    set_target_properties(
        ${target}
        PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${OUTPUT_DIR}"
                   LIBRARY_OUTPUT_DIRECTORY "${OUTPUT_DIR}"
                   ARCHIVE_OUTPUT_DIRECTORY "${OUTPUT_DIR}")

endfunction(setup_output_dirs)
