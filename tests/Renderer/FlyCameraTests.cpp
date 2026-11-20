#include "Orion/Core/Events/ApplicationEvent.h"
#include "Orion/Core/Events/MouseEvent.h"
#include "Orion/Renderer/FlyCamera.h"

#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_floating_point.hpp>
#include <cmath>
#include <glm/glm.hpp>
#include <glm/gtc/epsilon.hpp>

using Catch::Matchers::WithinAbs;

namespace
{
    constexpr double Epsilon = 1e-4;

    Orion::FlyCamera MakeCamera()
    {
        return {60.0f, 16.0f / 9.0f, 0.1f, 1000.0f};
    }
} // namespace

TEST_CASE("Renderer/Camera/FlyCamera: forward, right and up follow yaw and pitch", "[Renderer][FlyCamera]")
{
    Orion::FlyCamera camera = MakeCamera();

    const auto approxEqual = [](const glm::vec3& a, const glm::vec3& b) {
        return glm::all(glm::epsilonEqual(a, b, static_cast<float>(Epsilon)));
    };

    SECTION("yaw 0, pitch 0 looks down -Z")
    {
        camera.SetRotation(0.0f, 0.0f);
        CHECK(approxEqual(camera.GetForwardDirection(), {0.0f, 0.0f, -1.0f}));
        CHECK(approxEqual(camera.GetRightDirection(), {1.0f, 0.0f, 0.0f}));
        CHECK(approxEqual(camera.GetUpDirection(), {0.0f, 1.0f, 0.0f}));
    }

    SECTION("yaw 90 turns to the left, towards -X")
    {
        camera.SetRotation(90.0f, 0.0f);
        CHECK(approxEqual(camera.GetForwardDirection(), {-1.0f, 0.0f, 0.0f}));
        CHECK(approxEqual(camera.GetRightDirection(), {0.0f, 0.0f, -1.0f}));
        CHECK(approxEqual(camera.GetUpDirection(), {0.0f, 1.0f, 0.0f}));
    }

    SECTION("pitch 45 looks up and forward")
    {
        // Not 90: SetRotation clamps the pitch just below the pole
        const float s = std::sqrt(0.5f);
        camera.SetRotation(0.0f, 45.0f);
        CHECK(approxEqual(camera.GetForwardDirection(), {0.0f, s, -s}));
        CHECK(approxEqual(camera.GetRightDirection(), {1.0f, 0.0f, 0.0f}));
        CHECK(approxEqual(camera.GetUpDirection(), {0.0f, s, s}));
    }
}

TEST_CASE("Renderer/Camera/FlyCamera: SetRotation clamps the pitch and wraps the yaw", "[Renderer][FlyCamera]")
{
    Orion::FlyCamera camera = MakeCamera();

    SECTION("a pitch beyond the poles never flips the camera upside down")
    {
        // Unclamped, +-120 would look backwards (forward.z > 0) with up pointing down (up.y < 0)
        for (const float pitch : {120.0f, -120.0f})
        {
            camera.SetRotation(0.0f, pitch);
            CHECK(camera.GetForwardDirection().z < 0.0f);
            CHECK(camera.GetUpDirection().y > 0.0f);
        }
    }

    SECTION("the yaw is wrapped into [-180, 180] without changing the direction")
    {
        Orion::FlyCamera reference = MakeCamera();
        reference.SetRotation(10.0f, 30.0f);

        for (const float yaw : {370.0f, -350.0f, 730.0f})
        {
            camera.SetRotation(yaw, 30.0f);
            CHECK(glm::all(glm::epsilonEqual(camera.GetForwardDirection(), reference.GetForwardDirection(), 1e-4f)));
            CHECK_THAT(static_cast<double>(camera.GetYaw()), WithinAbs(10.0, Epsilon));
        }
    }
}

TEST_CASE("Renderer/Camera/FlyCamera: forward, right and up always form an orthonormal basis", "[Renderer][FlyCamera]")
{
    Orion::FlyCamera camera = MakeCamera();
    camera.SetRotation(37.0f, -25.0f);

    const glm::vec3 forward = camera.GetForwardDirection();
    const glm::vec3 right   = camera.GetRightDirection();
    const glm::vec3 up      = camera.GetUpDirection();

    CHECK_THAT(static_cast<double>(glm::length(forward)), WithinAbs(1.0, Epsilon));
    CHECK_THAT(static_cast<double>(glm::length(right)), WithinAbs(1.0, Epsilon));
    CHECK_THAT(static_cast<double>(glm::length(up)), WithinAbs(1.0, Epsilon));

    CHECK_THAT(static_cast<double>(glm::dot(forward, right)), WithinAbs(0.0, Epsilon));
    CHECK_THAT(static_cast<double>(glm::dot(forward, up)), WithinAbs(0.0, Epsilon));
    CHECK_THAT(static_cast<double>(glm::dot(right, up)), WithinAbs(0.0, Epsilon));

    // Right-handed: right = forward x up (a mirrored basis would give -right)
    CHECK(glm::all(glm::epsilonEqual(right, glm::cross(forward, up), 1e-4f)));
}

TEST_CASE("Renderer/Camera/FlyCamera: the mouse wheel scales the move speed and clamps it", "[Renderer][FlyCamera]")
{
    Orion::FlyCamera camera = MakeCamera();
    const float initial     = camera.GetMoveSpeed();

    SECTION("one step up or down")
    {
        Orion::MouseScrolledEvent up(0.0f, 1.0f);
        camera.OnEvent(up);
        CHECK_THAT(static_cast<double>(camera.GetMoveSpeed()), WithinAbs(static_cast<double>(initial) * 1.2, Epsilon));
        CHECK_FALSE(up.Handled); // the camera does not consume the wheel

        Orion::FlyCamera other = MakeCamera();
        Orion::MouseScrolledEvent down(0.0f, -1.0f);
        other.OnEvent(down);
        CHECK_THAT(static_cast<double>(other.GetMoveSpeed()), WithinAbs(static_cast<double>(initial) / 1.2, Epsilon));
    }

    SECTION("the speed is clamped to [0.1, 500]")
    {
        for (int i = 0; i < 100; ++i)
        {
            Orion::MouseScrolledEvent up(0.0f, 1.0f);
            camera.OnEvent(up);
        }
        CHECK(camera.GetMoveSpeed() == 500.0f);

        for (int i = 0; i < 100; ++i)
        {
            Orion::MouseScrolledEvent down(0.0f, -1.0f);
            camera.OnEvent(down);
        }
        CHECK_THAT(static_cast<double>(camera.GetMoveSpeed()), WithinAbs(0.1, Epsilon));
    }
}

TEST_CASE("Renderer/Camera/FlyCamera: a horizontal scroll does not change the move speed", "[Renderer][FlyCamera]")
{
    Orion::FlyCamera camera = MakeCamera();
    const float initial     = camera.GetMoveSpeed();

    Orion::MouseScrolledEvent horizontal(1.0f, 0.0f);
    camera.OnEvent(horizontal);

    CHECK(camera.GetMoveSpeed() == initial);
}

TEST_CASE("Renderer/Camera/FlyCamera: resizing the window updates the aspect ratio of the projection",
          "[Renderer][FlyCamera]")
{
    Orion::FlyCamera camera = MakeCamera();

    const auto aspectOf = [&camera]() {
        const glm::mat4& p = camera.GetProjectionMatrix();
        return static_cast<double>(p[1][1] / p[0][0]);
    };

    Orion::WindowResizeEvent wide(1000, 500);
    camera.OnEvent(wide);
    CHECK_THAT(aspectOf(), WithinAbs(2.0, Epsilon));

    Orion::WindowResizeEvent tall(500, 1000);
    camera.OnEvent(tall);
    CHECK_THAT(aspectOf(), WithinAbs(0.5, Epsilon));

    camera.OnEvent(wide);

    SECTION("zero width keeps the previous aspect")
    {
        Orion::WindowResizeEvent noWidth(0, 500);
        camera.OnEvent(noWidth);
        CHECK_THAT(aspectOf(), WithinAbs(2.0, Epsilon));
    }

    SECTION("zero height keeps the previous aspect")
    {
        Orion::WindowResizeEvent noHeight(1000, 0);
        camera.OnEvent(noHeight);
        CHECK_THAT(aspectOf(), WithinAbs(2.0, Epsilon));
    }
}
