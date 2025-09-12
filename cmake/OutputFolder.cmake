function(setup_output_dirs target)
    set_target_properties(
        ${target}
        PROPERTIES RUNTIME_OUTPUT_DIRECTORY
                   "${CMAKE_SOURCE_DIR}/bin/$<CONFIG>-$<PLATFORM_ID>/${target}"
                   LIBRARY_OUTPUT_DIRECTORY
                   "${CMAKE_SOURCE_DIR}/bin/$<CONFIG>-$<PLATFORM_ID>/${target}"
                   ARCHIVE_OUTPUT_DIRECTORY
                   "${CMAKE_SOURCE_DIR}/bin/$<CONFIG>-$<PLATFORM_ID>/${target}")

endfunction(setup_output_dirs)
