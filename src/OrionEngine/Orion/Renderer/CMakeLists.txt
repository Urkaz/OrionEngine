target_sources(
    ${ENGINE_NAME}
    PRIVATE Buffer.cpp
            GraphicsContext.cpp
            OrthographicCamera.cpp
            RenderCommand.cpp
            RenderCommandQueue.cpp
            Renderer.cpp
            Renderer2D.cpp
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
           RenderCommandQueue.h
           Renderer.h
           Renderer2D.h
           RendererAPI.h
           Shader.h
           Texture.h
           VertexArray.h)
