#pragma once

#include "Orion/Renderer/VertexArray.h"
#include <glm/glm.hpp>

namespace Orion
{
    class RendererAPI
    {
    public:
        enum class API
        {
            None   = 0,
            OpenGL = 1,
        };

        struct Capabilities
        {
            std::string Vendor;
            std::string Renderer;
            std::string Version;
        };

    public:
        virtual ~RendererAPI() = default;

        virtual void Init() = 0;

        virtual void SetClearColor(const glm::vec4 color) = 0;
        virtual void Clear()                              = 0;

        virtual void DrawIndexed(const Ref<VertexArray>& vertexArray) = 0;

        inline static API GetAPI()
        {
            return s_API;
        }

        static Capabilities& GetCapabilities()
        {
            static Capabilities capabilities;
            return capabilities;
        }

    private:
        static API s_API;
    };
} // namespace Orion
