target_sources(
    ${ENGINE_NAME}
    PRIVATE Buffer.cpp
            GraphicsContext.cpp
            OrthographicCamera.cpp
            RenderCommand.cpp
            Renderer.cpp
            RendererAPI.cpp
            Shader.cpp
            Texture.cpp
            VertexArray.cpp)

target_sources(
    ${ENGINE_NAME}
    PUBLIC Buffer.h
           GraphicsContext.h
           OrthographicCamera.h
           RenderCommand.h
           Renderer.h
           RendererAPI.h
           Shader.h
           Texture.h
           VertexArray.h)
