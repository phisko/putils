#include "SpinLock.hpp"

namespace putils {
	void SpinLock::lock() noexcept {
		while (write_now.exchange(true, std::memory_order_acquire))
			;

		// wait for readers to exit
		while (readers_count != 0)
			;
	}

	void SpinLock::unlock() noexcept {
		write_now.store(false, std::memory_order_release);
	}

	void SpinLock::lock_shared() noexcept {
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

	void SpinLock::unlock_shared() noexcept {
            readers_count.fetch_sub(1, std::memory_order_release);
        }

	void SpinLock::unlock_and_lock_shared() noexcept {
		readers_count.fetch_add(1, std::memory_order_acquire);
		write_now.store(false, std::memory_order_release);
	}

	SpinLock::Lock::Lock(SpinLock & spinLock) noexcept
		: _spinLock(spinLock)
	{
		lock();
	}

	SpinLock::Lock::~Lock() noexcept {
		if (_locked)
			unlock();
	}

	void SpinLock::Lock::lock() noexcept {
		_spinLock.lock();
		_locked = true;
	}

	void SpinLock::Lock::unlock() noexcept {
		_spinLock.unlock();
		_locked = false;
	}

	SpinLock::SharedLock::SharedLock(SpinLock & spinLock) noexcept
		: _spinLock(spinLock)
	{
		lock();
	}

	SpinLock::SharedLock::~SharedLock() noexcept {
		if (_locked)
			unlock();
	}

	SpinLock::SharedLock::SharedLock(Lock && rhs) noexcept
		: _spinLock(rhs._spinLock)
	{
		rhs._spinLock.unlock_and_lock_shared();
	}

	void SpinLock::SharedLock::lock() noexcept {
		_spinLock.lock_shared();
		_locked = true;
	}

	void SpinLock::SharedLock::unlock() noexcept {
		_spinLock.unlock_shared();
		_locked = false;
	}
}