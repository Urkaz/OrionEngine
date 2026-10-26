target_sources(
    ${ENGINE_NAME}
    PRIVATE AssimpMeshImporter.cpp
            Buffer.cpp
            Camera.cpp
            FlyCamera.cpp
            GraphicsContext.cpp
            MeshRenderData.cpp
            MeshSource.cpp
            RenderCommand.cpp
            RenderCommandQueue.cpp
            Renderer.cpp
            Renderer2D.cpp
            RendererAPI.cpp
            Shader.cpp
            ShaderSourceParser.cpp
            Texture.cpp
            VertexArray.cpp)

target_sources(
    ${ENGINE_NAME}
    PUBLIC AssimpMeshImporter.h
           AssimpUtils.h
           Buffer.h
           Camera.h
           FlyCamera.h
           GraphicsContext.h
           MeshRenderData.h
           MeshSource.h
           RenderCommand.h
           RenderCommandQueue.h
           Renderer.h
           Renderer2D.h
           RendererAPI.h
           Shader.h
           ShaderSourceParser.h
           Texture.h
           VertexArray.h)
