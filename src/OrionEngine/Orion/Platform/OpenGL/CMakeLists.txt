target_sources(
    ${ENGINE_NAME}
    PRIVATE OpenGLBuffer.cpp
            OpenGLContext.cpp
            OpenGLRendererAPI.cpp
            OpenGLShader.cpp
            OpenGLTexture.cpp
            OpenGLVertexArray.cpp)

target_sources(
    ${ENGINE_NAME}
    PUBLIC OpenGLBuffer.h
           OpenGLContext.h
           OpenGLRendererAPI.h
           OpenGLShader.h
           OpenGLTexture.h
           OpenGLVertexArray.h)
