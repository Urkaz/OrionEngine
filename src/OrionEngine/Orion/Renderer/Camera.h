#pragma once

#include <glm/glm.hpp>

namespace Orion
{
    class Camera
    {
    public:
        Camera() = default;
        explicit Camera(const glm::mat4& projection) : m_ProjectionMatrix(projection) {}
        virtual ~Camera() = default;

        void SetPerspective(float vericalFOVRadians, float aspectRatio, float nearClip, float farClip);
        void SetOrthographic(float size, float aspectRatio, float nearClip, float farClip);

        const glm::mat4& GetProjectionMatrix() const
        {
            return m_ProjectionMatrix;
        }

    protected:
        glm::mat4 m_ProjectionMatrix = glm::mat4(1.0f);
    };
} // namespace Orion
