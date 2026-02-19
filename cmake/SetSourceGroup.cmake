function(set_source_group file_in)
    # Get the directory of the source file
    get_filename_component(PARENT_DIR "${file_in}" DIRECTORY)

    # message(STATUS "PARENT_DIR: ${PARENT_DIR}")

    # Remove common directory prefix to make the group
    string(REPLACE "${CMAKE_CURRENT_SOURCE_DIR}" "" GROUP "${PARENT_DIR}")

    # Make sure we are using windows slashes
    # string(REPLACE "/" "\\" GROUP "${GROUP}")

    # Group
    set(GROUP "Source Files${GROUP}")

    # message(STATUS "GROUP: ${GROUP}")

    source_group("${GROUP}" FILES "${file_in}")
endfunction()
