#include "oepch.h"
#include "Orion/Core/Core.h"
#include "Orion/Platform/OpenGL/OpenGLShader.h"
#include "Orion/Renderer/ShaderSourceParser.h"

#include <fstream>
#include <glad/glad.h>
#include <glm/gtc/type_ptr.hpp>

namespace Orion
{
    OpenGLShader::OpenGLShader(const std::string& name, const std::string& vertexSrc, const std::string& fragmentSrc)
        : m_RendererID(0), m_Name(name)
    {
        OE_PROFILE_FUNCTION();

        std::unordered_map<GLenum, std::string> shaderSources;
        shaderSources[GL_VERTEX_SHADER]   = vertexSrc;
        shaderSources[GL_FRAGMENT_SHADER] = fragmentSrc;
        Compile(shaderSources);
    }

    OpenGLShader::OpenGLShader(const std::string& filepath) : m_RendererID(0)
    {
        OE_PROFILE_FUNCTION();

        const std::string source = ReadFile(filepath);
        auto shaderSources       = PreProcess(source);
        Compile(shaderSources);

        // Extract name from filepath
        auto lastSlash = filepath.find_last_of("/\\");
        lastSlash      = lastSlash == std::string::npos ? 0 : lastSlash + 1;
        auto lastDot   = filepath.rfind('.');
        auto count     = lastDot == std::string::npos ? filepath.size() - lastSlash : lastDot - lastSlash;
        m_Name         = filepath.substr(lastSlash, count);
    }

    OpenGLShader::~OpenGLShader()
    {
        OE_PROFILE_FUNCTION();

        glDeleteProgram(m_RendererID);
    }

    std::string OpenGLShader::ReadFile(const std::string& filepath)
    {
        OE_PROFILE_FUNCTION();

        std::ifstream in(filepath, std::ios::in | std::ios::binary);
        if (!in)
        {
            OE_CORE_LOG(Error, "Could not read shader file: '{0}'", filepath);
            return {};
        }

        in.seekg(0, std::ios::end);
        const std::streampos endPos = in.tellg();
        if (endPos <= 0)
        {
            OE_CORE_LOG(Error, "Could not read from file '{0}'", filepath);
            return {};
        }

        const std::size_t size = static_cast<std::size_t>(endPos);

        std::string result(size, '\0');
        in.seekg(0, std::ios::beg);
        in.read(result.data(), static_cast<std::streamsize>(size));

        return result;
    }

    std::unordered_map<GLenum, std::string> OpenGLShader::PreProcess(const std::string& source)
    {
        OE_PROFILE_FUNCTION();

        const ShaderParseResult parsed = ParseShaderSource(source);
        OE_CORE_ASSERT(parsed.Ok(), parsed.Error);

        std::unordered_map<GLenum, std::string> shaderSources;
        for (const auto& [stage, code] : parsed.Stages)
            shaderSources[stage == ShaderStage::Vertex ? GL_VERTEX_SHADER : GL_FRAGMENT_SHADER] = code;

        return shaderSources;
    }

    void OpenGLShader::Compile(const std::unordered_map<GLenum, std::string>& shaderSources)
    {
        OE_PROFILE_FUNCTION();

        const GLuint program = glCreateProgram();
        OE_CORE_ASSERT(shaderSources.size() <= 2, "Too many shader sources")
        std::array<GLenum, 2> glShaderIDs = {0, 0};

        size_t glShaderIDIndex = 0;
        for (auto& kv : shaderSources)
        {
            const GLenum shaderType   = kv.first;
            const std::string& source = kv.second;

            // Create an empty vertex shader handle
            const GLuint shader = glCreateShader(shaderType);

            // Send the shader source code to GL
            const GLchar* sourceCStr = source.c_str();
            glShaderSource(shader, 1, &sourceCStr, nullptr);

            // Compile the shader
            glCompileShader(shader);

            GLint isCompiled = 0;
            glGetShaderiv(shader, GL_COMPILE_STATUS, &isCompiled);
            if (isCompiled == GL_FALSE)
            {
                GLint maxLength = 0;
                glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &maxLength);

                // The maxLength includes the NULL character
                auto logLength = (maxLength > 0) ? static_cast<size_t>(maxLength) : static_cast<size_t>(1);
                std::vector<GLchar> infoLog(logLength);
                glGetShaderInfoLog(shader, maxLength, &maxLength, &infoLog[0]);

                // We don't need the shader anymore.
                glDeleteShader(shader);

                OE_CORE_LOG(Error, "{0}", infoLog.data());
                OE_CORE_ASSERT(false, "Shader compilarion failed!");
                return;
            }

            // Shader successfully compiled.
            // Attach shader to our program
            glAttachShader(program, shader);
            glShaderIDs[glShaderIDIndex++] = shader;
        }

        // Store program after a successfult compilation
        m_RendererID = program;

        // Link our program
        glLinkProgram(program);

        GLint isLinked = 0;
        glGetProgramiv(program, GL_LINK_STATUS, &isLinked);
        if (isLinked == GL_FALSE)
        {
            GLint maxLength = 0;
            glGetProgramiv(program, GL_INFO_LOG_LENGTH, &maxLength);

            // The maxLength includes the NULL character
            auto logLength = (maxLength > 0) ? static_cast<size_t>(maxLength) : static_cast<size_t>(1);
            std::vector<GLchar> infoLog(logLength);
            glGetProgramInfoLog(program, maxLength, &maxLength, &infoLog[0]);

            // We don't need the program anymore.
            glDeleteProgram(program);
            // Don't leak shaders either.
            for (auto id : glShaderIDs)
                glDeleteShader(id);

            OE_CORE_LOG(Error, "{0}", infoLog.data());
            OE_ASSERT(false, "Shader link error!");
            return;
        }

        // Always detach shaders after a successful link.
        for (auto id : glShaderIDs)
        {
            glDetachShader(program, id);
            glDeleteShader(id);
        }
    }

    void OpenGLShader::Bind() const
    {
        OE_PROFILE_FUNCTION();

        glUseProgram(m_RendererID);
    }

    void OpenGLShader::Unbind() const
    {
        OE_PROFILE_FUNCTION();

        glUseProgram(0);
    }

    std::string OpenGLShader::GetName()
    {
        return m_Name;
    }

    void OpenGLShader::SetInt(const std::string& name, int value)
    {
        OE_PROFILE_FUNCTION();

        UploadUniformInt(name, value);
    }

    void OpenGLShader::SetFloat(const std::string& name, float value)
    {
        OE_PROFILE_FUNCTION();

        UploadUniformFloat(name, value);
    }

    void OpenGLShader::SetFloat3(const std::string& name, const glm::vec3& value)
    {
        OE_PROFILE_FUNCTION();

        UploadUniformFloat3(name, value);
    }

    void OpenGLShader::SetFloat4(const std::string& name, const glm::vec4& value)
    {
        OE_PROFILE_FUNCTION();

        UploadUniformFloat4(name, value);
    }

    void OpenGLShader::SetMat4(const std::string& name, const glm::mat4& value)
    {
        OE_PROFILE_FUNCTION();

        UploadUniformMat4(name, value);
    }

    void OpenGLShader::UploadUniformInt(const std::string& name, int value)
    {
        const GLint location = glGetUniformLocation(m_RendererID, name.c_str());
        glUniform1i(location, value);
    }

    void OpenGLShader::UploadUniformFloat(const std::string& name, float value)
    {
        const GLint location = glGetUniformLocation(m_RendererID, name.c_str());
        glUniform1f(location, value);
    }

    void OpenGLShader::UploadUniformFloat2(const std::string& name, const glm::vec2 values)
    {
        const GLint location = glGetUniformLocation(m_RendererID, name.c_str());
        glUniform2f(location, values.x, values.y);
    }

    void OpenGLShader::UploadUniformFloat3(const std::string& name, const glm::vec3 values)
    {
        const GLint location = glGetUniformLocation(m_RendererID, name.c_str());
        glUniform3f(location, values.x, values.y, values.z);
    }

    void OpenGLShader::UploadUniformFloat4(const std::string& name, const glm::vec4 values)
    {
        const GLint location = glGetUniformLocation(m_RendererID, name.c_str());
        glUniform4f(location, values.x, values.y, values.z, values.w);
    }

    void OpenGLShader::UploadUniformMat3(const std::string& name, const glm::mat3& matrix)
    {
        const GLint location = glGetUniformLocation(m_RendererID, name.c_str());
        glUniformMatrix3fv(location, 1, GL_FALSE, glm::value_ptr(matrix));
    }

    void OpenGLShader::UploadUniformMat4(const std::string& name, const glm::mat4& matrix)
    {
        const GLint location = glGetUniformLocation(m_RendererID, name.c_str());
        glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(matrix));
    }
} // namespace Orion
