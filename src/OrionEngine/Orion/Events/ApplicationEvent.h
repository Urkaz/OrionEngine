#pragma once

#include "Orion/Events/Event.h"

namespace Orion
{
    /**
     * @class WindowResizeEvent
     * @brief Event fired when the application window is resized.
     *
     * Carries the new width and height of the window in pixels.
     */
    class WindowResizeEvent : public Event
    {
    public:
        /**
         * @brief Construct a new WindowResizeEvent
         * @param width New window width in pixels
         * @param height New window height in pixels
         */
        WindowResizeEvent(unsigned int width, unsigned int height) : m_Width(width), m_Height(height) {}

        /**
         * @brief Get the new window width.
         * @return unsigned int Width in pixels.
         */
        inline unsigned int GetWidth() const
        {
            return m_Width;
        }

        /**
         * @brief Get the new window height.
         * @return unsigned int Height in pixels.
         */
        inline unsigned int GetHeight() const
        {
            return m_Height;
        }

        /**
         * @brief String representation of the event for logging/debugging.
         * @return std::string "WindowResizeEvent: <width>, <height>"
         */
        std::string ToString() const override
        {
            std::stringstream ss;
            ss << "WindowResizeEvent: " << m_Width << ", " << m_Height;
            return ss.str();
        }

        EVENT_CLASS_TYPE(WindowResize)
        EVENT_CLASS_CATEGORY(EventCategoryApplication)
    private:
        unsigned int m_Width, m_Height;
    };

    /**
     * @class WindowCloseEvent
     * @brief Event fired when the application window receives a close request.
     */
    class WindowCloseEvent : public Event
    {
    public:
        WindowCloseEvent() {}

        EVENT_CLASS_TYPE(WindowClose)
        EVENT_CLASS_CATEGORY(EventCategoryApplication)
    };

    class AppTickEvent : public Event
    {
    public:
        AppTickEvent() {}

        EVENT_CLASS_TYPE(AppTick)
        EVENT_CLASS_CATEGORY(EventCategoryApplication)
    };

    class AppUpdateEvent : public Event
    {
    public:
        AppUpdateEvent() {}

        EVENT_CLASS_TYPE(AppUpdate)
        EVENT_CLASS_CATEGORY(EventCategoryApplication)
    };

    class AppRenderEvent : public Event
    {
    public:
        AppRenderEvent() {}

        EVENT_CLASS_TYPE(AppRender)
        EVENT_CLASS_CATEGORY(EventCategoryApplication)
    };

} // namespace Orion
