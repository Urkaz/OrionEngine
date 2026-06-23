#include "oepch.h"
#include "Orion/Platform/OpenGL/OpenGLTexture.h"

#include "OpenGLTexture.h"
#include <glad/glad.h>
#include <stb_image.h>

namespace Orion
{
    OpenGLTexture2D::OpenGLTexture2D(uint32_t width, uint32_t height) : m_Width(width), m_Height(height)
    {
        OE_PROFILE_FUNCTION();

        m_InternalFormat = GL_RGBA8;
        m_DataFormat     = GL_RGBA;

        glCreateTextures(GL_TEXTURE_2D, 1, &m_RendererID);
        glTextureStorage2D(m_RendererID,
                           1,
                           m_InternalFormat,
                           static_cast<GLsizei>(width),
                           static_cast<GLsizei>(height));

        glTextureParameteri(m_RendererID, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTextureParameteri(m_RendererID, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

        glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_T, GL_REPEAT);
    }

    OpenGLTexture2D::OpenGLTexture2D(const std::string& path)
    {
        OE_PROFILE_FUNCTION();

        int width, height, channels;
        stbi_set_flip_vertically_on_load(1);
        stbi_uc* data = nullptr;
        {
            OE_PROFILE_SCOPE("stbi_load - OpenGLTexture2D::OpenGLTexture2D(const std::string&)");
            data = stbi_load(path.c_str(), &width, &height, &channels, 0);
        }
        OE_CORE_ASSERT(data, "Failed to load image!");
        m_Width  = static_cast<uint32_t>(width);
        m_Height = static_cast<uint32_t>(height);

        m_InternalFormat = 0;
        m_DataFormat     = 0;
        if (channels == 4)
        {
            m_InternalFormat = GL_RGBA8;
            m_DataFormat     = GL_RGBA;
        }
        else if (channels == 3)
        {
            m_InternalFormat = GL_RGB8;
            m_DataFormat     = GL_RGB;
        }

        OE_CORE_ASSERT(m_InternalFormat && m_DataFormat, "Texture format not supported")

        glCreateTextures(GL_TEXTURE_2D, 1, &m_RendererID);
        glTextureStorage2D(m_RendererID, 1, m_InternalFormat, width, height);

        glTextureParameteri(m_RendererID, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTextureParameteri(m_RendererID, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

        glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_T, GL_REPEAT);

        glTextureSubImage2D(m_RendererID,
                            0,
                            0,
                            0,
                            static_cast<GLsizei>(width),
                            static_cast<GLsizei>(height),
                            m_DataFormat,
                            GL_UNSIGNED_BYTE,
                            data);

        stbi_image_free(data);
    }

    OpenGLTexture2D::~OpenGLTexture2D()
    {
        OE_PROFILE_FUNCTION();

        glDeleteTextures(1, &m_RendererID);
    }

    void OpenGLTexture2D::SetData(void* data, [[maybe_unused]] uint32_t size)
    {
        OE_PROFILE_FUNCTION();

        [[maybe_unused]] uint32_t bpp = m_DataFormat == GL_RGBA ? 4 : 3;
        OE_CORE_ASSERT(size == m_Width * m_Height * bpp, "Data must be entire texture!");
        glTextureSubImage2D(m_RendererID,
                            0,
                            0,
                            0,
                            static_cast<GLsizei>(m_Width),
                            static_cast<GLsizei>(m_Height),
                            m_DataFormat,
                            GL_UNSIGNED_BYTE,
                            data);
    }

    void OpenGLTexture2D::Bind(uint32_t slot) const
    {
        OE_PROFILE_FUNCTION();

        glBindTextureUnit(slot, m_RendererID);
    }
} // namespace Orion
