#pragma once

// stl
#include <chrono>

namespace putils {
	//
	// timer class
	//
	class PUTILS_CORE_EXPORT timer {
	public:
		using t_clock = std::chrono::steady_clock;
		using t_duration = std::chrono::duration<float, std::ratio<1>>;
		using seconds = t_duration;

	public:
		timer(const seconds & duration) noexcept;

		// Is the timer ringing
		bool is_done() const noexcept;
		float get_times_done() const noexcept;

		// How long ago did I start the timer
		seconds get_time_since_start() const noexcept;
		seconds get_time_since_done() const noexcept;

		// How long until it rings
		seconds get_time_left() const noexcept;

		// Restart the timer
		void restart() noexcept;
		void set_start(t_clock::time_point start) noexcept;

		const t_clock::time_point & get_start() const noexcept;
		const seconds & get_duration() const noexcept;
		void set_duration(const seconds & duration) noexcept;

		// Attributes
	private:
		seconds _duration{ 0 };
		t_clock::time_point _start = t_clock::now();
	};
}
