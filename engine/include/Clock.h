#pragma once

#include <chrono>

class Clock
{
public:
    using ClockType = std::chrono::steady_clock;

    Clock()
    {
        reset();
    }

    // Returns the time elapsed since the previous call, in seconds.
    float tick()
    {
        const auto currentTime = ClockType::now();

        const std::chrono::duration<float> delta =
            currentTime - m_previousTime;

        m_previousTime = currentTime;

        return delta.count();
    }

    // Returns the time elapsed since the clock was created or reset,
    // in seconds.
    float elapsed() const
    {
        const auto currentTime = ClockType::now();

        const std::chrono::duration<float> elapsed =
            currentTime - m_startTime;

        return elapsed.count();
    }

    // Resets the clock.
    void reset()
    {
        const auto currentTime = ClockType::now();

        m_startTime = currentTime;
        m_previousTime = currentTime;
    }

private:
    ClockType::time_point m_startTime;
    ClockType::time_point m_previousTime;
};

