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

        // Constructors
    public:
        Timer() : _duration(0), _start(t_clock::now()) {}

        template<typename Duration>
        Timer(Duration && duration) noexcept
                : _duration(std::forward<Duration>(duration)), _start(t_clock::now()) {}

        // Coplien
    public:
        Timer(const Timer & other) noexcept = default;

        Timer & operator=(const Timer & other) noexcept = default;

        // Move
    public:
        Timer(Timer && other) noexcept = default;

        Timer & operator=(Timer && other) noexcept = default;

        // Is the timer ringing
    public:
        bool isDone() const noexcept { return (t_clock::now() - _start >= _duration && !_stopped); }

        float getTimesDone() const noexcept { return (t_clock::now() - _start) / _duration; }

        // How long ago did I start the timer
    public:
        t_duration getTimeSinceStart() const noexcept { return t_clock::now() - _start; }

        t_duration getTimeSinceDone() const noexcept {
            // return t_clock::now() - (_start + _duration);
            const auto elapsed = std::chrono::duration_cast<seconds>(t_clock::now() - _start);
            return elapsed - _duration;
        }

        // How long until it rings
    public:
        t_duration getTimeLeft() const noexcept { return _start + _duration - t_clock::now(); }

        // Restart the timer
    public:
        void restart() noexcept {
            _start = t_clock::now();
            _stopped = false;
        }

        void setStart(t_clock::time_point start) { _start = start; }

        const t_clock::time_point & getStart() const { return _start; }

        void stop() { _stopped = true; }

        // Duration getters and setters
    public:
        const t_duration & getDuration() const noexcept { return _duration; }

        template<typename Duration>
        void setDuration(Duration && duration) noexcept { _duration = duration; }

        // Attributes
    private:
        t_duration _duration;
        t_clock::time_point _start;
        bool _stopped = false;
    };
}
