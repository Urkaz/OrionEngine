#pragma once

#include "Orion/Renderer/RendererAPI.h"

#include "Orion/Renderer/OrthographicCamera.h"
#include "Orion/Renderer/RenderCommandQueue.h"
#include "Orion/Renderer/Shader.h"

namespace Orion
{
    class Renderer
    {
    public:
        static void Init();
        static void Shutdown();

        static void OnWindowResize(uint32_t width, uint32_t height);

        static void BeginScene(OrthographicCamera& camera);
        static void EndScene();

        static void Submit(const Ref<Shader>& shader,
                           const Ref<VertexArray>& vertexArray,
                           const glm::mat4 transform = glm::mat4(1.0f));

        inline static RendererAPI::API GetAPI()
        {
            return RendererAPI::GetAPI();
        }

        inline static Renderer& Get()
        {
            return *s_Instance;
        }

        template <typename FuncT>
        static void Submit(FuncT&& func)
        {
            OE_PROFILE_FUNCTION();

            // Create a wrapper (lambda) with the same signature as RenderCommandFn that knows
            // how to execute and destroy the original object.
            //    - Receives a void* (pointer to the memory where the real object lives)
            //    - This is necessary because the real type of func is only known at compile time,
            //      but the CommandQueue stores function pointers with a fixed signature (RenderCommandFn).
            auto renderCmd = [](void* ptr) {
                auto pFunc = (FuncT*)ptr;
                (*pFunc)(); // Executes the real command

                // We explicitly call the destructor because the object was constructed with placement-new.
                // If FuncT were trivially destructible, we could omit this, but many lambdas
                // may capture std::string or other resources that require destruction.
                pFunc->~FuncT();
            };

            // Reserve space in the command buffer for the object and register the wrapper.
            // std::decay_t<FuncT> removes references and qualifiers to get the real type.
            auto storageBuffer = GetRenderCommandQueue().Allocate(renderCmd, sizeof(std::decay_t<FuncT>));

            OE_CORE_ASSERT(storageBuffer, "RenderCommandQueue overflow");

            // Construct the func object in the reserved buffer using placement-new.
            new (storageBuffer) FuncT(std::forward<FuncT>(func));
        }

        static void WaitAndRender()
        {
            s_Instance->GetRenderCommandQueue().Execute();
        }

    private:
        static RenderCommandQueue& GetRenderCommandQueue();

    private:
        struct SceneData
        {
            glm::mat4 ViewProjectionMatrix;
        };
        static Scope<SceneData> s_SceneData;

        static Scope<Renderer> s_Instance;
    };

} // namespace Orion
