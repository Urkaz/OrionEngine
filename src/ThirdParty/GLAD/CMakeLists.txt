set(LIBRARY_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/src/glad.c")
set(LIBRARY_HEADERS "${CMAKE_CURRENT_SOURCE_DIR}/include/glad/glad.h"
                    "${CMAKE_CURRENT_SOURCE_DIR}/include/KHR/khrplatform.h")

set(LIBRARY_INCLUDES "./include/")

add_library(GLAD STATIC ${LIBRARY_SOURCES} ${LIBRARY_HEADERS})
target_include_directories(GLAD PUBLIC ${LIBRARY_INCLUDES})
set_target_properties(GLAD PROPERTIES FOLDER "Dependencies")
