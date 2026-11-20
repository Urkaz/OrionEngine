target_sources(
    ${ENGINE_NAME}
    PRIVATE VirtualPath.cpp
            MemoryMountSource.cpp
            FileSystem.cpp
            DiskMountSource.cpp
            Internal/OpenFileRegistry.cpp
            Internal/OpenFileRegistry.h)

target_sources(
    ${ENGINE_NAME}
    PUBLIC VirtualPath.h
           File.h
           MountSource.h
           MemoryMountSource.h
           FileSystem.h
           PlatformFile.h
           DiskMountSource.h)
