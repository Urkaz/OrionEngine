add_subdirectory(GLAD)
add_subdirectory(ImGui)
add_subdirectory(stb_image)

if(ENABLE_PROFILING)
    add_subdirectory(Tracy)
endif()
