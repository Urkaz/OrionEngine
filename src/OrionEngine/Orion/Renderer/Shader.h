#pragma once

#include "Orion/Core.h"

#include <string>

namespace Orion
{
    class Shader
    {
    public:
        virtual ~Shader() = default;

        virtual void Bind() const   = 0;
        virtual void Unbind() const = 0;

        static Ref<Shader> Create(const std::string& vertexSrc, const std::string& fragmentSrc);
        static Ref<Shader> Create(const std::string& filepath);
    };
} // namespace Orion
