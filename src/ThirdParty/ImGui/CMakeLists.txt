add_library(
    ImGui_Core STATIC
    "${CMAKE_CURRENT_SOURCE_DIR}/source/imgui.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/source/imgui_demo.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/source/imgui_draw.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/source/imgui_tables.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/source/imgui_widgets.cpp")
target_include_directories(ImGui_Core PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/source")

# GLFW Backend
add_library(ImGui_GLFW STATIC "source/backends/imgui_impl_glfw.cpp")
target_include_directories(ImGui_GLFW
                           PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/source/backends/imgui_impl_glfw.h")
target_link_libraries(ImGui_GLFW PUBLIC ImGui_Core glfw)

# OpenGL3 Backend
add_library(ImGui_OpenGL3 STATIC "source/backends/imgui_impl_opengl3.cpp")
target_include_directories(ImGui_OpenGL3
                           PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/source/backends/imgui_impl_opengl3.h")
target_link_libraries(ImGui_OpenGL3 PUBLIC ImGui_Core OpenGL::GL)
