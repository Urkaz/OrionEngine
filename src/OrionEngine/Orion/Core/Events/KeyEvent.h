#pragma once

#include "Orion/Core/Events/Event.h"
#include "Orion/Core/KeyCodes.h"

#include <format>

namespace Orion
{
    class KeyEvent : public Event
    {
    public:
        inline KeyCode GetKeyCode() const
        {
            return m_KeyCode;
        }

        EVENT_CLASS_CATEGORY(EventCategoryKeyboard | EventCategoryInput)

    protected:
        KeyEvent(KeyCode keycode) : m_KeyCode(keycode) {}

        KeyCode m_KeyCode;
    };

    class KeyPressedEvent : public KeyEvent
    {
    public:
        KeyPressedEvent(KeyCode keycode, int repeatCount) : KeyEvent(keycode), m_RepeatCount(repeatCount) {}

        inline int GetRepeatCount() const
        {
            return m_RepeatCount;
        }

        std::string ToString() const override
        {
            return std::format("KeyPressedEvent: {} ({} repeats)", m_KeyCode, m_RepeatCount);
        }

        EVENT_CLASS_TYPE(KeyPressed)

    private:
        int m_RepeatCount;
    };

    class KeyReleasedEvent : public KeyEvent
    {
    public:
        KeyReleasedEvent(KeyCode keycode) : KeyEvent(keycode) {}

        std::string ToString() const override
        {
            return std::format("KeyReleasedEvent: {}", m_KeyCode);
        }

        EVENT_CLASS_TYPE(KeyReleased)
    };

    class KeyTypedEvent : public KeyEvent
    {
    public:
        KeyTypedEvent(KeyCode keycode) : KeyEvent(keycode) {}

        std::string ToString() const override
        {
            return std::format("KeyTypedEvent: {}", m_KeyCode);
        }

        EVENT_CLASS_TYPE(KeyTyped)
    };
} // namespace Orion
