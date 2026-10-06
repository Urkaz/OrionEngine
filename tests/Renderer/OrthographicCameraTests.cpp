#include "Orion/Renderer/OrthographicCamera.h"

#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_floating_point.hpp>

#include <glm/glm.hpp>

using Catch::Matchers::WithinAbs;

namespace
{
    constexpr double Epsilon = 1e-5;

    // Catch2 float matchers work in double, so return double to avoid implicit float -> double promotion.
    glm::dvec3 ToNDC(const glm::mat4& viewProjection, const glm::vec3& worldPoint)
    {
        glm::vec4 clip = viewProjection * glm::vec4(worldPoint, 1.0f);
        return glm::dvec3(glm::vec3(clip) / clip.w);
    }
} // namespace

TEST_CASE("OrthographicCamera: volume corners map to NDC corners", "[Renderer][Camera]")
{
    Orion::OrthographicCamera camera(-1.6f, 1.6f, -0.9f, 0.9f);

    glm::dvec3 bottomLeft = ToNDC(camera.GetViewProjectionMatrix(), {-1.6f, -0.9f, 0.0f});
    glm::dvec3 topRight   = ToNDC(camera.GetViewProjectionMatrix(), {1.6f, 0.9f, 0.0f});

    CHECK_THAT(bottomLeft.x, WithinAbs(-1.0, Epsilon));
    CHECK_THAT(bottomLeft.y, WithinAbs(-1.0, Epsilon));
    CHECK_THAT(topRight.x, WithinAbs(1.0, Epsilon));
    CHECK_THAT(topRight.y, WithinAbs(1.0, Epsilon));
}
