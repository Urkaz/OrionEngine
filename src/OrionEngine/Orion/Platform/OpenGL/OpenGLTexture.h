#pragma once

#include "Orion/Renderer/Texture.h"

#include <glad/glad.h>
#include <stb_image.h>

namespace Orion
{
    class OpenGLTexture2D : public Texture2D
    {
    public:
        OpenGLTexture2D(uint32_t width, uint32_t height);
        OpenGLTexture2D(const void* data, size_t size, bool srgb = false);
        virtual ~OpenGLTexture2D();

        // Takes ownership of nothing: `pixels` is only read. `channels` is 3 or 4.
        void Upload(const stbi_uc* pixels, int width, int height, int channels, bool srgb);

        virtual uint32_t GetWidth() const override
        {
            return m_Width;
        }
        virtual uint32_t GetHeight() const override
        {
            return m_Height;
        }

        virtual void SetData(void* data, uint32_t size) override;

        virtual void Bind(uint32_t slot) const override;

    private:
        std::string m_Path;
        uint32_t m_Width        = 0;
        uint32_t m_Height       = 0;
        uint32_t m_RendererID   = 0;
        GLenum m_InternalFormat = 0;
        GLenum m_DataFormat     = 0;
    };
} // namespace Orion
