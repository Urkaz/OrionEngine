#pragma once

#include "Orion/Core/Events/Event.h"
#include "Orion/Core/KeyCodes.h"

#include <format>

namespace Orion
{
    class KeyEvent : public Event
    {
    public:
        [[nodiscard]] inline KeyCode GetKeyCode() const
        {
            return m_KeyCode;
        }

        EVENT_CLASS_CATEGORY(EventCategoryKeyboard | EventCategoryInput)

    protected:
        explicit KeyEvent(KeyCode keycode) : m_KeyCode(keycode) {}

    private:
        KeyCode m_KeyCode;
    };

    class KeyPressedEvent : public KeyEvent
    {
    public:
        KeyPressedEvent(KeyCode keycode, int repeatCount) : KeyEvent(keycode), m_RepeatCount(repeatCount) {}

        [[nodiscard]] inline int GetRepeatCount() const
        {
            return m_RepeatCount;
        }

        [[nodiscard]] std::string ToString() const override
        {
            return std::format("KeyPressedEvent: {} ({} repeats)", GetKeyCode(), m_RepeatCount);
        }

        EVENT_CLASS_TYPE(KeyPressed)

    private:
        int m_RepeatCount;
    };

    class KeyReleasedEvent : public KeyEvent
    {
    public:
        explicit KeyReleasedEvent(KeyCode keycode) : KeyEvent(keycode) {}

        [[nodiscard]] std::string ToString() const override
        {
            return std::format("KeyReleasedEvent: {}", GetKeyCode());
        }

        EVENT_CLASS_TYPE(KeyReleased)
    };

    class KeyTypedEvent : public KeyEvent
    {
    public:
        explicit KeyTypedEvent(KeyCode keycode) : KeyEvent(keycode) {}

        [[nodiscard]] std::string ToString() const override
        {
            return std::format("KeyTypedEvent: {}", GetKeyCode());
        }

        EVENT_CLASS_TYPE(KeyTyped)
    };
} // namespace Orion
