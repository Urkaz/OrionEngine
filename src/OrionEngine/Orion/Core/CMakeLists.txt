add_subdirectory(Events)

target_sources(
    ${ENGINE_NAME}
    PRIVATE Application.cpp
            Core.cpp
            Layer.cpp
            LayerStack.cpp
            Log.cpp)

target_sources(
    ${ENGINE_NAME}
    PUBLIC Application.h
           Core.h
           Input.h
           KeyCodes.h
           Layer.h
           LayerStack.h
           Log.h
           MouseCodes.h
           Timestep.h
           Warnings.h
           Window.h)
