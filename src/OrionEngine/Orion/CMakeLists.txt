add_subdirectory(Core)
add_subdirectory(ImGui)
add_subdirectory(Platform)
add_subdirectory(Renderer)
add_subdirectory(Profiling)

target_sources(${ENGINE_NAME} PRIVATE OrthographicCameraController.cpp EntryPoint.h)

target_sources(${ENGINE_NAME} PUBLIC OrthographicCameraController.h)
