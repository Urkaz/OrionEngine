#pragma once

#include "oepch.h"

#include "Orion/Core.h"

#ifdef SPDLOG_FMT_EXTERNAL
    #include <fmt/core.h>
#else
    #include <spdlog/fmt/bundled/core.h>
#endif

namespace Orion
{
    /**
     * @brief Notes about event handling strategy.
     *
     * Events in OrionEngine are currently blocking: when an event occurs it
     * is dispatched immediately and handled synchronously. In the future,
     * consider queuing events in an event bus to process them during a
     * dedicated event phase of the update loop.
     */

    /**
     * @enum EventType
     * @brief All possible event types recognized by the engine.
     */
    enum class EventType
    {
        None = 0,
        WindowClose,
        WindowResize,
        WindowFocus,
        WindowLostFocus,
        WindowMoved,
        AppTick,
        AppUpdate,
        AppRender,
        KeyPressed,
        KeyReleased,
        KeyTyped,
        MouseButtonPressed,
        MouseButtonReleased,
        MouseMoved,
        MouseScrolled
    };

    /**
     * @enum EventCategory
     * @brief Bitmask categories that group related events together.
     *
     * Categories are used for quick filtering of events (e.g., input vs application events).
     */
    enum EventCategory
    {
        None                     = 0,
        EventCategoryApplication = BIT(0),
        EventCategoryInput       = BIT(1),
        EventCategoryKeyboard    = BIT(2),
        EventCategoryMouse       = BIT(3),
        EventCategoryMouseButton = BIT(4)
    };

    /**
     * @def EVENT_CLASS_TYPE(type)
     * @brief Macro to define type-related functions in event subclasses.
     *
     * Expands to: GetStaticType, GetEventType, and GetName implementations.
     */
#define EVENT_CLASS_TYPE(type)                                                                                         \
    static EventType GetStaticType()                                                                                   \
    {                                                                                                                  \
        return EventType::type;                                                                                        \
    }                                                                                                                  \
    virtual EventType GetEventType() const override                                                                    \
    {                                                                                                                  \
        return GetStaticType();                                                                                        \
    }                                                                                                                  \
    virtual const char* GetName() const override                                                                       \
    {                                                                                                                  \
        return #type;                                                                                                  \
    }

    /**
     * @def EVENT_CLASS_CATEGORY(category)
     * @brief Macro to define category flags accessor in event subclasses.
     */
#define EVENT_CLASS_CATEGORY(category)                                                                                 \
    virtual int GetCategoryFlags() const override                                                                      \
    {                                                                                                                  \
        return category;                                                                                               \
    }

    /**
     * @class Event
     * @brief Base class for all events in the engine.
     *
     * Provides an interface for event type identification, category flags,
     * string conversion and a handled flag that listeners can set to stop
     * further propagation.
     */
    class Event
    {
        friend class EventDispatcher;

    public:
        Event()          = default;
        virtual ~Event() = default;

        /**
         * @brief Get the concrete event type.
         * @return EventType
         */
        virtual EventType GetEventType() const = 0;

        /**
         * @brief Get the event's name as a C string.
         * @return const char*
         */
        virtual const char* GetName() const    = 0;

        /**
         * @brief Get the category bitmask for the event.
         * @return int Bitmask of EventCategory values.
         */
        virtual int GetCategoryFlags() const   = 0;

        /**
         * @brief String representation used for logging/debugging.
         * @return std::string
         */
        virtual std::string ToString() const
        {
            return GetName();
        }

        /**
         * @brief Check whether the event belongs to the given category.
         * @param category Category to test.
         * @return true if event is in category, false otherwise.
         */
        inline bool IsInCategory(EventCategory category)
        {
            return GetCategoryFlags() & category;
        }

        bool Handled = false; ///< Set to true by an event handler to stop propagation
    };

    /**
     * @class EventDispatcher
     * @brief Utility to dispatch an Event to a specific handler based on type.
     *
     * Usage:
     * EventDispatcher dispatcher(event);
     * dispatcher.Dispatch<WindowResizeEvent>([](WindowResizeEvent& e){ ... });
     */
    class EventDispatcher
    {
        template <typename T>
        using EventFn = std::function<bool(T&)>;

    public:
        /**
         * @brief Construct the dispatcher for a given event.
         * @param event Reference to the event to dispatch.
         */
        EventDispatcher(Event& event) : m_Event(event) {}

        /**
         * @brief Dispatch the stored event if it matches the type T.
         * @tparam T Expected event type to dispatch to.
         * @tparam F Callable taking T& and returning bool indicating handled state.
         * @param func Handler function/callable.
         * @return true if the event was dispatched (type matched), false otherwise.
         */
        template <typename T, typename F>
        bool Dispatch(const F& func)
        {
            if (m_Event.GetEventType() == T::GetStaticType())
            {
                m_Event.Handled = func(static_cast<T&>(m_Event));
                return true;
            }
            return false;
        }

    private:
        Event& m_Event;
    };

    /**
     * @brief Formatter helper allowing Events to be used with fmt::format or spdlog.
     * @param e Event reference
     * @return std::string The string representation of the event.
     */
    inline std::string format_as(const Event& e)
    {
        return e.ToString();
    }

} // namespace Orion
