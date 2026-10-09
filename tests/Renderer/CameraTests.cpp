#include "Orion/Core/Events/ApplicationEvent.h"
#include "Orion/Renderer/Camera.h"
#include "Orion/Renderer/FlyCamera.h"

#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_floating_point.hpp>

#include <glm/fwd.hpp>
#include <glm/glm.hpp>

#include <cmath>

using Catch::Matchers::WithinAbs;

namespace
{
    constexpr double Epsilon = 1e-4;

    // Catch2 float matchers work in double, so return double to avoid implicit float -> double promotion.
    glm::dvec3 ToNDC(const glm::mat4& viewProjection, const glm::vec3& worldPoint)
    {
        glm::vec4 clip = viewProjection * glm::vec4(worldPoint, 1.0f);
        return glm::dvec3(glm::vec3(clip) / clip.w);
    }
} // namespace

TEST_CASE("Camera: perspective maps the near and far planes to NDC z -1 and +1", "[Renderer][Camera]")
{
    constexpr float nearClip = 0.1f;
    constexpr float farClip  = 100.0f;

    Orion::Camera camera;
    camera.SetPerspective(glm::radians(45.0f), 16.0f / 9.0f, nearClip, farClip);
    const glm::mat4& projection = camera.GetProjectionMatrix();

    CHECK_THAT(ToNDC(projection, {0.0f, 0.0f, -nearClip}).z, WithinAbs(-1.0, Epsilon));
    CHECK_THAT(ToNDC(projection, {0.0f, 0.0f, -farClip}).z, WithinAbs(1.0, Epsilon));
}

TEST_CASE("FlyCamera: the view matrix is the inverse of the camera transform", "[Renderer][Camera]")
{
    Orion::FlyCamera camera(45.0f, 16.0f / 9.0f, 0.1f, 1000.0f);
    const glm::vec3 position = {1.0f, 2.0f, 3.0f};
    camera.SetPosition(position);
    camera.SetRotation(90.0f, 20.0f);

    const glm::mat4& viewMatrix = camera.GetViewMatrix();

    // Multiplying the view matrix by the camera position should yield the origin in view space.
    const glm::dvec4 cameraInViewSpace(viewMatrix * glm::vec4(position, 1.0f));
    CHECK_THAT(cameraInViewSpace.x, WithinAbs(0.0, Epsilon));
    CHECK_THAT(cameraInViewSpace.y, WithinAbs(0.0, Epsilon));
    CHECK_THAT(cameraInViewSpace.z, WithinAbs(0.0, Epsilon));

    // Check if the camera looks to the correct direction
    const glm::vec3 inFront = position + camera.GetForwardDirection() * 5.0f;
    const glm::dvec3 ndc    = ToNDC(camera.GetViewProjectionMatrix(), inFront);
    CHECK_THAT(ndc.x, WithinAbs(0.0, Epsilon));
    CHECK_THAT(ndc.y, WithinAbs(0.0, Epsilon));
}

TEST_CASE("FlyCamera: a 0x0 resize keeps a valid projection", "[Renderer][Camera]")
{
    Orion::FlyCamera camera(45.0f, 16.0f / 9.0f, 0.1f, 1000.0f);
    Orion::WindowResizeEvent resize(0, 0);
    camera.OnEvent(resize);

    const glm::mat4& projectionMatrix = camera.GetProjectionMatrix();

    for (int column = 0; column < 4; ++column)
    {
        for (int row = 0; row < 4; ++row)
        {
            CHECK(std::isfinite(projectionMatrix[column][row]));
        }
    }
}
