#include "Timer.hpp"

namespace putils {
	Timer::Timer(const t_duration & duration) noexcept
		: _duration(duration)
	{}

	bool Timer::isDone() const noexcept {
		return (t_clock::now() - _start >= _duration);
	}

	float Timer::getTimesDone() const noexcept {
		return (t_clock::now() - _start) / _duration;
	}

	Timer::t_duration Timer::getTimeSinceStart() const noexcept {
		return t_clock::now() - _start;
	}

	Timer::t_duration Timer::getTimeSinceDone() const noexcept {
		const auto elapsed = std::chrono::duration_cast<seconds>(t_clock::now() - _start);
		return elapsed - _duration;
	}

	Timer::t_duration Timer::getTimeLeft() const noexcept {
		return _start + _duration - t_clock::now(); 
	}

	void Timer::restart() noexcept {
		_start = t_clock::now();
	}

	void Timer::setStart(t_clock::time_point start) noexcept {
		_start = start;
	}

	const Timer::t_clock::time_point & Timer::getStart() const noexcept {
		return _start; 
	}

	const Timer::t_duration & Timer::getDuration() const noexcept {
		return _duration; 
	}

	void Timer::setDuration(const t_duration & duration) noexcept {
		_duration = duration; 
	}
}