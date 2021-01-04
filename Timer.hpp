#pragma once

#include <chrono>

namespace putils {
    //
    // Timer class
    //
    class Timer {
    public:
        using t_clock = std::chrono::steady_clock;
        using t_duration = std::chrono::duration<float, std::ratio<1>>;
        using seconds = t_duration;

    public:
        Timer() noexcept = default;
        Timer(const t_duration & duration) noexcept;

        Timer(const Timer & other) noexcept = default;
        Timer & operator=(const Timer & other) noexcept = default;

        // Is the timer ringing
        bool isDone() const noexcept;
        float getTimesDone() const noexcept;

        // How long ago did I start the timer
        t_duration getTimeSinceStart() const noexcept;
        t_duration getTimeSinceDone() const noexcept;

        // How long until it rings
        t_duration getTimeLeft() const noexcept;

        // Restart the timer
        void restart() noexcept;
        void setStart(t_clock::time_point start) noexcept;

        const t_clock::time_point & getStart() const noexcept;
        const t_duration & getDuration() const noexcept;
        void setDuration(const t_duration & duration) noexcept;

        // Attributes
    private:
        t_duration _duration{ 0 };
        t_clock::time_point _start = t_clock::now();
    };
}
