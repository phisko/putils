#include "spin_lock.hpp"

// putils
#include "putils/profiling.hpp"

namespace putils {
	void spin_lock::lock() noexcept {
		PUTILS_PROFILING_SCOPE;

		while (write_now.exchange(true, std::memory_order_acquire))
			;

		// wait for readers to exit
		while (readers_count != 0)
			;
	}

	void spin_lock::unlock() noexcept {
		PUTILS_PROFILING_SCOPE;
		write_now.store(false, std::memory_order_release);
	}

	void spin_lock::lock_shared() noexcept {
		PUTILS_PROFILING_SCOPE;

		// unique_lock have priority
		while (true) {
			// wait for unlock
			while (write_now)
				;

			readers_count.fetch_add(1, std::memory_order_acquire);

			if (write_now) {
				// locked while transaction? Fallback. Go another round
				readers_count.fetch_sub(1, std::memory_order_release);
			}
			else {
				// all ok
				return;
			}
		}
	}

	void spin_lock::unlock_shared() noexcept {
		PUTILS_PROFILING_SCOPE;
		readers_count.fetch_sub(1, std::memory_order_release);
	}

	void spin_lock::unlock_and_lock_shared() noexcept {
		PUTILS_PROFILING_SCOPE;
		readers_count.fetch_add(1, std::memory_order_acquire);
		write_now.store(false, std::memory_order_release);
	}

	spin_lock::unique_lock::unique_lock(spin_lock & spin_lock) noexcept
		: _spin_lock(spin_lock)
	{
		lock();
	}

	spin_lock::unique_lock::~unique_lock() noexcept {
		if (_locked)
			unlock();
	}

	void spin_lock::unique_lock::lock() noexcept {
		_spin_lock.lock();
		_locked = true;
	}

	void spin_lock::unique_lock::unlock() noexcept {
		_spin_lock.unlock();
		_locked = false;
	}

	spin_lock::shared_lock::shared_lock(spin_lock & spin_lock) noexcept
		: _spin_lock(spin_lock)
	{
		lock();
	}

	spin_lock::shared_lock::~shared_lock() noexcept {
		if (_locked)
			unlock();
	}

	spin_lock::shared_lock::shared_lock(unique_lock && rhs) noexcept
		: _spin_lock(rhs._spin_lock)
	{
		rhs._spin_lock.unlock_and_lock_shared();
	}

	void spin_lock::shared_lock::lock() noexcept {
		_spin_lock.lock_shared();
		_locked = true;
	}

	void spin_lock::shared_lock::unlock() noexcept {
		_spin_lock.unlock_shared();
		_locked = false;
	}
}