add_subdirectory(Events)
add_subdirectory(FileSystem)

target_sources(
    ${ENGINE_NAME}
    PRIVATE Application.cpp
            Core.cpp
            Input.cpp
            Layer.cpp
            LayerStack.cpp
            Log.cpp
            Window.cpp
            Internal/FileSystemSink.cpp
            Internal/FileSystemSink.h)

target_sources(
    ${ENGINE_NAME}
    PUBLIC Application.h
           Assert.h
           Base.h
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
