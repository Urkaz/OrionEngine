#include "oepch.h"
#include "OpenGLShader.h"

#include <glad/glad.h>
#include <glm/gtc/type_ptr.hpp>

Orion::OpenGLShader::OpenGLShader(const std::string& vertexSrc, const std::string& fragmentSrc)
{
    // Read our shaders into the appropriate buffers
    // Create an empty vertex shader handle
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);

    // Send the vertex shader source code to GL
    // Note that std::string's .c_str is NULL character terminated.
    const GLchar* source = vertexSrc.c_str();
    glShaderSource(vertexShader, 1, &source, 0);

    // Compile the vertex shader
    glCompileShader(vertexShader);

    GLint isCompiled = 0;
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &isCompiled);
    if (isCompiled == GL_FALSE)
    {
        GLint maxLength = 0;
        glGetShaderiv(vertexShader, GL_INFO_LOG_LENGTH, &maxLength);

        // The maxLength includes the NULL character
        auto logLength = (maxLength > 0) ? static_cast<size_t>(maxLength) : static_cast<size_t>(1);
        std::vector<GLchar> infoLog(logLength);
        glGetShaderInfoLog(vertexShader, maxLength, &maxLength, &infoLog[0]);

        // We don't need the shader anymore.
        glDeleteShader(vertexShader);

        OE_CORE_LOG(error, "{0}", infoLog.data());
        OE_ASSERT(false, "Vertex shader compilarion error!");
        return;
    }

    // Create an empty fragment shader handle
    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

    // Send the fragment shader source code to GL
    // Note that std::string's .c_str is NULL character terminated.
    source = fragmentSrc.c_str();
    glShaderSource(fragmentShader, 1, &source, 0);

    // Compile the fragment shader
    glCompileShader(fragmentShader);

    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &isCompiled);
    if (isCompiled == GL_FALSE)
    {
        GLint maxLength = 0;
        glGetShaderiv(fragmentShader, GL_INFO_LOG_LENGTH, &maxLength);

        // The maxLength includes the NULL character
        auto logLength = (maxLength > 0) ? static_cast<size_t>(maxLength) : static_cast<size_t>(1);
        std::vector<GLchar> infoLog(logLength);
        glGetShaderInfoLog(fragmentShader, maxLength, &maxLength, &infoLog[0]);

        // We don't need the shader anymore.
        glDeleteShader(fragmentShader);
        // Either of them. Don't leak shaders.
        glDeleteShader(vertexShader);

        OE_CORE_LOG(error, "{0}", infoLog.data());
        OE_ASSERT(false, "Fragment shader compilarion error!");
        return;
    }

    // Vertex and fragment shaders are successfully compiled.
    // Now time to link them together into a program.
    // Get a program object.
    GLuint program = glCreateProgram();
    m_RendererID   = program;

    // Attach our shaders to our program
    glAttachShader(program, vertexShader);
    glAttachShader(program, fragmentShader);

    // Link our program
    glLinkProgram(program);

    // Note the different functions here: glGetProgram* instead of glGetShader*.
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
        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);

        OE_CORE_LOG(error, "{0}", infoLog.data());
        OE_ASSERT(false, "Shader link error!");
        return;
    }

    // Always detach shaders after a successful link.
    glDetachShader(program, vertexShader);
    glDetachShader(program, fragmentShader);
}

Orion::OpenGLShader::~OpenGLShader()
{
    glDeleteProgram(m_RendererID);
}

void Orion::OpenGLShader::Bind() const
{
    glUseProgram(m_RendererID);
}

void Orion::OpenGLShader::Unbind() const
{
    glUseProgram(0);
}

void Orion::OpenGLShader::UploadUniformInt(const std::string& name, int value)
{
    GLint location = glGetUniformLocation(m_RendererID, name.c_str());
    glUniform1i(location, value);
}

void Orion::OpenGLShader::UploadUniformFloat(const std::string& name, float value)
{
    GLint location = glGetUniformLocation(m_RendererID, name.c_str());
    glUniform1f(location, value);
}

void Orion::OpenGLShader::UploadUniformFloat2(const std::string& name, const glm::vec2 values)
{
    GLint location = glGetUniformLocation(m_RendererID, name.c_str());
    glUniform2f(location, values.x, values.y);
}

void Orion::OpenGLShader::UploadUniformFloat3(const std::string& name, const glm::vec3 values)
{
    GLint location = glGetUniformLocation(m_RendererID, name.c_str());
    glUniform3f(location, values.x, values.y, values.z);
}

void Orion::OpenGLShader::UploadUniformFloat4(const std::string& name, const glm::vec4 values)
{
    GLint location = glGetUniformLocation(m_RendererID, name.c_str());
    glUniform4f(location, values.x, values.y, values.z, values.w);
}

void Orion::OpenGLShader::UploadUniformMat3(const std::string& name, const glm::mat3& matrix)
{
    GLint location = glGetUniformLocation(m_RendererID, name.c_str());
    glUniformMatrix3fv(location, 1, GL_FALSE, glm::value_ptr(matrix));
}

void Orion::OpenGLShader::UploadUniformMat4(const std::string& name, const glm::mat4& matrix)
{
    GLint location = glGetUniformLocation(m_RendererID, name.c_str());
    glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(matrix));
}
