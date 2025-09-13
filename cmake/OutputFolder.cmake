function(setup_output_dirs target)
    set_target_properties(
        ${target}
        PROPERTIES RUNTIME_OUTPUT_DIRECTORY
                   "${CMAKE_SOURCE_DIR}/bin/$<CONFIG>-$<PLATFORM_ID>/${CMAKE_PROJECT_NAME}"
                   LIBRARY_OUTPUT_DIRECTORY
                   "${CMAKE_SOURCE_DIR}/bin/$<CONFIG>-$<PLATFORM_ID>/${CMAKE_PROJECT_NAME}"
                   ARCHIVE_OUTPUT_DIRECTORY
                   "${CMAKE_SOURCE_DIR}/bin/$<CONFIG>-$<PLATFORM_ID>/${CMAKE_PROJECT_NAME}")

endfunction(setup_output_dirs)
