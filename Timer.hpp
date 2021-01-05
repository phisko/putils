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
        Timer(const seconds & duration) noexcept;

        Timer(const Timer & other) noexcept = default;
        Timer & operator=(const Timer & other) noexcept = default;

        // Is the timer ringing
        bool isDone() const noexcept;
        float getTimesDone() const noexcept;

        // How long ago did I start the timer
        seconds getTimeSinceStart() const noexcept;
        seconds getTimeSinceDone() const noexcept;

        // How long until it rings
        seconds getTimeLeft() const noexcept;

        // Restart the timer
        void restart() noexcept;
        void setStart(t_clock::time_point start) noexcept;

        const t_clock::time_point & getStart() const noexcept;
        const seconds & getDuration() const noexcept;
        void setDuration(const seconds & duration) noexcept;

        // Attributes
    private:
        seconds _duration{ 0 };
        t_clock::time_point _start = t_clock::now();
    };
}
