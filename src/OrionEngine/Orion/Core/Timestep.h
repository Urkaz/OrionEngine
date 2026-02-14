#pragma once

namespace Orion
{
    /**
     * @class Timestep
     * @brief Simple wrapper representing a time interval.
     *
     * Encapsulates a floating-point time value (in seconds) and provides
     * utilities to access the time in seconds or milliseconds.
     */
    class Timestep
    {
    public:
        /**
         * @brief Construct a new Timestep
         * @param time Time value in seconds (default 0.0f)
         */
        Timestep(float time = 0.0f) : m_Time(time) {}

        /**
         * @brief Get the time value in seconds.
         * @return float Time in seconds.
         */
        float GetSeconds() const
        {
            return m_Time;
        }

        /**
         * @brief Get the time value in milliseconds.
         * @return float Time in milliseconds.
         */
        float GetMilliseconds() const
        {
            return m_Time * 1000.0f;
        }

        /**
         * @brief Implicit conversion operator to `float`.
         * @return operator float Returns the time value in seconds.
         */
        operator float() const
        {
            return m_Time;
        }

    private:
        float m_Time; ///< Time stored in seconds
    };
} // namespace Orion
