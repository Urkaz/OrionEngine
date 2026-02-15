add_library(
    stb_image STATIC
    "${CMAKE_CURRENT_SOURCE_DIR}/source/stb_image.cpp")
target_include_directories(stb_image SYSTEM PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/source")
set_target_properties(stb_image PROPERTIES FOLDER "Dependencies/stb")
