#pragma once

#include "Orion/Core/Assert.h"

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <limits>

namespace Orion
{
    // Matrix that transforms NORMALS into world space. For a model matrix M it is transpose(inverse(mat3(M)))
    inline glm::mat3 NormalMatrix(const glm::mat4& model)
    {
        auto m = glm::mat3(model);
        OE_CORE_ASSERT(glm::determinant(m) >= std::numeric_limits<float>::min(), "Model matrix is not invertible!");
        return glm::transpose(glm::inverse(m));
    }

    // Unit direction from yaw/pitch: yaw 0 looks towards -Z, positive yaw turns towards -X, positive pitch looks up
    inline glm::vec3 DirectionFromYawPitch(float yawDegrees, float pitchDegrees)
    {
        const glm::quat rotation = glm::angleAxis(glm::radians(yawDegrees), glm::vec3(0.0f, 1.0f, 0.0f)) *
                                   glm::angleAxis(glm::radians(pitchDegrees), glm::vec3(1.0f, 0.0f, 0.0f));
        return rotation * glm::vec3(0.0f, 0.0f, -1.0f);
    }
} // namespace Orion
