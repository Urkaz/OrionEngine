#pragma once

#include "Orion/Core/Assert.h"
#include "Orion/Profiling/Profiling.h"

#include "Orion/Renderer/Camera.h"
#include "Orion/Renderer/MeshRenderData.h"
#include "Orion/Renderer/RendererAPI.h"
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

        static void BeginScene(const Camera& camera, const glm::mat4& viewMatrix);
        static void EndScene();

        static void Submit(const Ref<Shader>& shader,
                           const Ref<VertexArray>& vertexArray,
                           const glm::mat4 transform = glm::mat4(1.0f));

        static void SubmitMesh(const Ref<Shader>& shader,
                               const Ref<MeshRenderData>& renderData,
                               const glm::mat4& transform = glm::mat4(1.0f));

        inline static RendererAPI::API GetAPI()
        {
            return RendererAPI::GetAPI();
        }

        template <typename FuncT>
        static void Submit(FuncT&& func)
        {
            OE_PROFILE_FUNCTION();

            // The "clean" type of the lambda: if an lvalue is passed, FuncT is deduced as Lambda&,
            // and we need the type itself to allocate, construct, call and destroy it.
            using Fn = std::decay_t<FuncT>;
            static_assert(alignof(Fn) <= RenderCommandQueue::PayloadAlignment,
                          "Render command captures a type with an alignment the queue does not support");

            // Create a wrapper (lambda) with the same signature as RenderCommandFn that knows
            // how to execute and destroy the original object.
            //    - Receives a void* (pointer to the memory where the real object lives)
            //    - This is necessary because the real type of func is only known at compile time,
            //      but the CommandQueue stores function pointers with a fixed signature (RenderCommandFn).
            auto renderCmd = [](void* ptr) {
                auto* pFunc = static_cast<Fn*>(ptr);
                (*pFunc)(); // Executes the real command

                // We explicitly call the destructor because the object was constructed with placement-new.
                // If Fn were trivially destructible, we could omit this, but many lambdas
                // may capture std::string or other resources that require destruction.
                pFunc->~Fn();
            };

            // Reserve space in the command buffer for the object and register the wrapper.
            void* storage = GetRenderCommandQueue().Allocate(renderCmd, static_cast<uint32_t>(sizeof(Fn)));
            OE_CORE_VERIFY(storage, "RenderCommandQueue is full");

            // Construct the func object in the reserved buffer using placement-new.
            new (storage) Fn(std::forward<FuncT>(func));
        }

        // Executes every pending render command. Called once per frame by Application.
        static void WaitAndRender()
        {
            GetRenderCommandQueue().Execute();
        }

        // True while the queued commands are being executed: the only moment RT_ functions may be called
        static bool IsExecutingCommands()
        {
            return GetRenderCommandQueue().IsExecuting();
        }

    private:
        static RenderCommandQueue& GetRenderCommandQueue();

    private:
        struct SceneData
        {
            glm::mat4 ViewProjectionMatrix;
        };
        static Scope<SceneData> s_SceneData;
    };

} // namespace Orion
