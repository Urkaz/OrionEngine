#pragma once

#include "Orion/Core/Events/Event.h"
#include "Orion/Core/MouseCodes.h"

#include <format>

namespace Orion
{
    class MouseMovedEvent : public Event
    {
    public:
        MouseMovedEvent(float x, float y) : m_MouseX(x), m_MouseY(y) {}

        [[nodiscard]] inline float GetX() const
        {
            return m_MouseX;
        }
        [[nodiscard]] inline float GetY() const
        {
            return m_MouseY;
        }

        [[nodiscard]] std::string ToString() const override
        {
            return std::format("MouseMovedEvent: {}, {}", m_MouseX, m_MouseY);
        }

        EVENT_CLASS_TYPE(MouseMoved)
        EVENT_CLASS_CATEGORY(EventCategoryMouse | EventCategoryInput)
    private:
        float m_MouseX, m_MouseY;
    };

    class MouseScrolledEvent : public Event
    {
    public:
        MouseScrolledEvent(float xOffset, float yOffset) : m_XOffset(xOffset), m_YOffset(yOffset) {}

        [[nodiscard]] inline float GetXOffset() const
        {
            return m_XOffset;
        }
        [[nodiscard]] inline float GetYOffset() const
        {
            return m_YOffset;
        }

        [[nodiscard]] std::string ToString() const override
        {
            return std::format("MouseScrolledEvent: {}, {}", GetXOffset(), GetYOffset());
        }

        EVENT_CLASS_TYPE(MouseScrolled)
        EVENT_CLASS_CATEGORY(EventCategoryMouse | EventCategoryInput)
    private:
        float m_XOffset, m_YOffset;
    };

    class MouseButtonEvent : public Event
    {
    public:
        [[nodiscard]] inline MouseCode GetMouseButton() const
        {
            return m_Button;
        }

        EVENT_CLASS_CATEGORY(EventCategoryMouse | EventCategoryInput | EventCategoryMouseButton)
    protected:
        explicit MouseButtonEvent(MouseCode button) : m_Button(button) {}

    private:
        MouseCode m_Button;
    };

    class MouseButtonPressedEvent : public MouseButtonEvent
    {
    public:
        explicit MouseButtonPressedEvent(MouseCode button) : MouseButtonEvent(button) {}

        [[nodiscard]] std::string ToString() const override
        {
            return std::format("MouseButtonPressedEvent: {}", GetMouseButton());
        }

        EVENT_CLASS_TYPE(MouseButtonPressed)
    };

    class MouseButtonReleasedEvent : public MouseButtonEvent
    {
    public:
        explicit MouseButtonReleasedEvent(MouseCode button) : MouseButtonEvent(button) {}

        [[nodiscard]] std::string ToString() const override
        {
            return std::format("MouseButtonReleasedEvent: {}", GetMouseButton());
        }

        EVENT_CLASS_TYPE(MouseButtonReleased)
    };

} // namespace Orion
