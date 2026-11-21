#pragma once

#include "Orion/Core/Core.h"

#include <glm/glm.hpp>
#include <string>
#include <unordered_map>

namespace Orion
{
    class Shader
    {
    public:
        Shader()          = default;
        virtual ~Shader() = default;
        OE_NON_COPYABLE(Shader);

        virtual void Bind() const   = 0;
        virtual void Unbind() const = 0;

        virtual void SetInt(const std::string& name, int value)                 = 0;
        virtual void SetFloat(const std::string& name, float value)             = 0;
        virtual void SetFloat3(const std::string& name, const glm::vec3& value) = 0;
        virtual void SetFloat4(const std::string& name, const glm::vec4& value) = 0;
        virtual void SetMat4(const std::string& name, const glm::mat4& value)   = 0;
        virtual void SetMat3(const std::string& name, const glm::mat3& value)   = 0;

        [[nodiscard]] virtual const std::string& GetName() const = 0;

        [[nodiscard]] static Ref<Shader> Create(std::string name, std::string vertexSrc, std::string fragmentSrc);
        [[nodiscard]] static Ref<Shader> Create(const std::string& filepath);
    };

    class ShaderLibrary
    {
    public:
        void Add(const Ref<Shader>& shader);
        void Add(const std::string& name, const Ref<Shader>& shader);
        Ref<Shader> Load(const std::string& filepath);
        Ref<Shader> Load(const std::string& name, const std::string& filepath);
        [[nodiscard]] Ref<Shader> Get(const std::string& name);

        [[nodiscard]] bool Exists(const std::string& name) const;

    private:
        std::unordered_map<std::string, Ref<Shader>> m_Shaders;
    };
} // namespace Orion
