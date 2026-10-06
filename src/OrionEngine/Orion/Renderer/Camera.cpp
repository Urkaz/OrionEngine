#include "oepch.h"
#include "Orion/Renderer/Camera.h"

#include <glm/gtc/matrix_transform.hpp>

namespace Orion
{
    void Camera::SetPerspective(float verticalFOVRadians, float aspectRatio, float nearPlane, float farPlane)
    {
        m_ProjectionMatrix = glm::perspective(verticalFOVRadians, aspectRatio, nearPlane, farPlane);
    }

    void Camera::SetOrthographic(float size, float aspectRatio, float nearPlane, float farPlane)
    {
        // 'size' is the visible height in world units.
        const float halfHeight = size * 0.5f;
        const float halfWidth = halfHeight * aspectRatio;
        m_ProjectionMatrix = glm::ortho(-halfWidth, halfWidth, -halfHeight, halfHeight, nearPlane, farPlane);
    }
} // namespace Orion
