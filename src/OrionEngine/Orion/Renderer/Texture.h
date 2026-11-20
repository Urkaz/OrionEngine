#pragma once

#include "Orion/Core/Core.h"

#include <string>

namespace Orion
{
    class Texture
    {
    public:
        virtual ~Texture() = default;

        [[nodiscard]] virtual uint32_t GetWidth() const  = 0;
        [[nodiscard]] virtual uint32_t GetHeight() const = 0;

        virtual void SetData(void* data, uint32_t size) = 0;

        virtual void Bind(uint32_t slot = 0) const = 0;
    };

    class Texture2D : public Texture
    {
    public:
        [[nodiscard]] static Ref<Texture2D> Create(uint32_t width, uint32_t height);
        [[nodiscard]] static Ref<Texture2D> Create(const std::string& path, bool srgb = false);
        // `data` is the contents of an image FILE (PNG, JPEG...), not raw pixels
        [[nodiscard]] static Ref<Texture2D> CreateFromMemory(const void* data, size_t size, bool srgb = false);
    };
} // namespace Orion
