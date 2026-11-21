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

        [[nodiscard]] const glm::mat4& GetProjectionMatrix() const
        {
            return m_ProjectionMatrix;
        }

    protected:
        // Polymorphic base with data: only derived classes can copy or move (C++ Core Guidelines C.67)
        Camera(const Camera&)            = default;
        Camera& operator=(const Camera&) = default;
        Camera(Camera&&)                 = default;
        Camera& operator=(Camera&&)      = default;

    private:
        glm::mat4 m_ProjectionMatrix = glm::mat4(1.0f);
    };
} // namespace Orion
