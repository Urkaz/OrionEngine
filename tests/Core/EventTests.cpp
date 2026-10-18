#include "Orion/Core/Events/ApplicationEvent.h"
#include "Orion/Core/Events/Event.h"
#include "Orion/Core/Events/KeyEvent.h"
#include "Orion/Core/Events/MouseEvent.h"
#include "Orion/Core/Timestep.h"

#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_floating_point.hpp>

using Catch::Matchers::WithinAbs;

namespace
{
    constexpr double Epsilon = 1e-5;
} // namespace

TEST_CASE("Core/Events/EventDispatcher: a handler only runs for its own event type", "[Core][Events]")
{
    Orion::MouseMovedEvent event(10.0f, 20.0f);
    Orion::EventDispatcher dispatcher(event);

    bool resizeCalled = false;
    CHECK_FALSE(dispatcher.Dispatch<Orion::WindowResizeEvent>([&](Orion::WindowResizeEvent&) {
        resizeCalled = true;
        return true;
    }));
    CHECK_FALSE(resizeCalled);
    CHECK_FALSE(event.Handled);

    float seenX = 0.0f;
    CHECK(dispatcher.Dispatch<Orion::MouseMovedEvent>([&](Orion::MouseMovedEvent& e) {
        seenX = e.GetX();
        return true;
    }));
    CHECK(seenX == 10.0f);
    CHECK(event.Handled);
}

TEST_CASE("Core/Events/EventDispatcher: Handled follows the return value of the handler", "[Core][Events]")
{
    SECTION("a handler that does not consume the event")
    {
        Orion::MouseMovedEvent event(0.0f, 0.0f);
        Orion::EventDispatcher dispatcher(event);
        dispatcher.Dispatch<Orion::MouseMovedEvent>([](Orion::MouseMovedEvent&) { return false; });
        CHECK_FALSE(event.Handled);
    }

    SECTION("a later handler must not un-handle an event that an earlier one consumed")
    {
        Orion::MouseMovedEvent event(0.0f, 0.0f);
        Orion::EventDispatcher dispatcher(event);
        dispatcher.Dispatch<Orion::MouseMovedEvent>([](Orion::MouseMovedEvent&) { return true; });
        dispatcher.Dispatch<Orion::MouseMovedEvent>([](Orion::MouseMovedEvent&) { return false; });
        CHECK(event.Handled);
    }
}

TEST_CASE("Core/Events/Event: categories are a bitmask, and each event reports the right ones", "[Core][Events]")
{
    Orion::WindowResizeEvent resize(1280, 720);
    CHECK(resize.IsInCategory(Orion::EventCategoryApplication));
    CHECK_FALSE(resize.IsInCategory(Orion::EventCategoryInput));
    CHECK_FALSE(resize.IsInCategory(Orion::EventCategoryMouse));
    CHECK_FALSE(resize.IsInCategory(Orion::EventCategoryKeyboard));
    CHECK_FALSE(resize.IsInCategory(Orion::EventCategoryMouseButton));

    Orion::MouseMovedEvent moved(0.0f, 0.0f);
    CHECK_FALSE(moved.IsInCategory(Orion::EventCategoryApplication));
    CHECK(moved.IsInCategory(Orion::EventCategoryInput));
    CHECK(moved.IsInCategory(Orion::EventCategoryMouse));
    CHECK_FALSE(moved.IsInCategory(Orion::EventCategoryKeyboard));
    CHECK_FALSE(moved.IsInCategory(Orion::EventCategoryMouseButton));

    Orion::MouseButtonPressedEvent pressed(0);
    CHECK_FALSE(pressed.IsInCategory(Orion::EventCategoryApplication));
    CHECK(pressed.IsInCategory(Orion::EventCategoryInput));
    CHECK(pressed.IsInCategory(Orion::EventCategoryMouse));
    CHECK_FALSE(pressed.IsInCategory(Orion::EventCategoryKeyboard));
    CHECK(pressed.IsInCategory(Orion::EventCategoryMouseButton));

    Orion::KeyPressedEvent key(Orion::Key::A, 0);
    CHECK_FALSE(key.IsInCategory(Orion::EventCategoryApplication));
    CHECK(key.IsInCategory(Orion::EventCategoryInput));
    CHECK_FALSE(key.IsInCategory(Orion::EventCategoryMouse));
    CHECK(key.IsInCategory(Orion::EventCategoryKeyboard));
    CHECK_FALSE(key.IsInCategory(Orion::EventCategoryMouseButton));
}

TEST_CASE("Core/Events/Event: names and ToString are what the logs show", "[Core][Events]")
{
    Orion::WindowResizeEvent resize(1280, 720);
    CHECK(std::string(resize.GetName()) == "WindowResize");
    CHECK(resize.ToString() == "WindowResizeEvent: 1280, 720");
    CHECK(resize.GetEventType() == Orion::EventType::WindowResize);
    CHECK(resize.GetEventType() == Orion::WindowResizeEvent::GetStaticType());

    Orion::MouseScrolledEvent scrolled(1.5f, -2.0f);
    CHECK(scrolled.ToString() == "MouseScrolledEvent: 1.5, -2");

    Orion::WindowCloseEvent close;
    CHECK(close.ToString() == std::string(close.GetName()));
}

TEST_CASE("Core/Timestep/Timestep: seconds, milliseconds and the implicit conversion", "[Core][Timestep]")
{
    const Orion::Timestep ts(0.016f);
    CHECK_THAT(static_cast<double>(ts.GetSeconds()), Catch::Matchers::WithinRel(0.016, Epsilon));
    CHECK_THAT(static_cast<double>(ts.GetMilliseconds()), Catch::Matchers::WithinRel(16.0, Epsilon));
    CHECK_THAT(static_cast<double>(static_cast<float>(ts)), Catch::Matchers::WithinRel(0.016, Epsilon));

    const Orion::Timestep zero;
    CHECK(zero.GetSeconds() == 0.0f);
}
