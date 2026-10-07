target_sources(
    ${ENGINE_NAME}
    PRIVATE Buffer.cpp
            Camera.cpp
            FlyCamera.cpp
            GraphicsContext.cpp
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
           Camera.h
           FlyCamera.h
           GraphicsContext.h
           RenderCommand.h
           RenderCommandQueue.h
           Renderer.h
           Renderer2D.h
           RendererAPI.h
           Shader.h
           Texture.h
           VertexArray.h)
