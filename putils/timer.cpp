#include "timer.hpp"

namespace putils {
	timer::timer(const t_duration & duration) noexcept
		: _duration(duration)
	{}

	bool timer::is_done() const noexcept {
		return (t_clock::now() - _start >= _duration);
	}

	float timer::get_times_done() const noexcept {
		return (t_clock::now() - _start) / _duration;
	}

	timer::t_duration timer::get_time_since_start() const noexcept {
		return t_clock::now() - _start;
	}

	timer::t_duration timer::get_time_since_done() const noexcept {
		const auto elapsed = std::chrono::duration_cast<seconds>(t_clock::now() - _start);
		return elapsed - _duration;
	}

	timer::t_duration timer::get_time_left() const noexcept {
		return _start + _duration - t_clock::now(); 
	}

	void timer::restart() noexcept {
		_start = t_clock::now();
	}

	void timer::set_start(t_clock::time_point start) noexcept {
		_start = start;
	}

	const timer::t_clock::time_point & timer::get_start() const noexcept {
		return _start; 
	}

	const timer::t_duration & timer::get_duration() const noexcept {
		return _duration; 
	}

	void timer::set_duration(const t_duration & duration) noexcept {
		_duration = duration; 
	}
}