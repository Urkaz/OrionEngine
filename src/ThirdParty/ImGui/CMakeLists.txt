add_library(
    ImGui_Core STATIC
    "${CMAKE_CURRENT_SOURCE_DIR}/source/imgui.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/source/imgui_demo.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/source/imgui_draw.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/source/imgui_tables.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/source/imgui_widgets.cpp")
target_include_directories(ImGui_Core PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/source")
set_target_properties(ImGui_Core PROPERTIES FOLDER "Dependencies/ImGui")
target_compile_definitions(ImGui_Core PRIVATE _CRT_SECURE_NO_WARNINGS)

# GLFW Backend
add_library(ImGui_GLFW STATIC "source/backends/imgui_impl_glfw.cpp")
target_include_directories(ImGui_GLFW
                           PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/source/backends/imgui_impl_glfw.h")
target_link_libraries(ImGui_GLFW PUBLIC ImGui_Core glfw)
set_target_properties(ImGui_GLFW PROPERTIES FOLDER "Dependencies/ImGui")
target_compile_definitions(ImGui_GLFW PRIVATE _CRT_SECURE_NO_WARNINGS)

# OpenGL3 Backend
add_library(ImGui_OpenGL3 STATIC "source/backends/imgui_impl_opengl3.cpp")
target_include_directories(ImGui_OpenGL3
                           PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/source/backends/imgui_impl_opengl3.h")
target_link_libraries(ImGui_OpenGL3 PUBLIC ImGui_Core OpenGL::GL)
set_target_properties(ImGui_OpenGL3 PROPERTIES FOLDER "Dependencies/ImGui")
target_compile_definitions(ImGui_OpenGL3 PRIVATE _CRT_SECURE_NO_WARNINGS)
