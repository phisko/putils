#pragma once

// stl
#include <atomic>

// Taken from https://codereview.stackexchange.com/questions/167641/c-read-write-shared-spinlock-implementation

namespace putils {
    class PUTILS_CORE_EXPORT SpinLock {
	public:
        void lock() noexcept;
        void unlock() noexcept;

        void lock_shared() noexcept;
        void unlock_shared() noexcept;

        void unlock_and_lock_shared() noexcept;

        struct PUTILS_CORE_EXPORT Lock {
            Lock(SpinLock & spinLock) noexcept;
            ~Lock() noexcept;

            void lock() noexcept;
            void unlock() noexcept;

            SpinLock & _spinLock;
            bool _locked;
        };

        struct PUTILS_CORE_EXPORT SharedLock {
            SharedLock(SpinLock & spinLock) noexcept;
            ~SharedLock() noexcept;
            SharedLock(Lock && rhs) noexcept;

            void lock() noexcept;
            void unlock() noexcept;

            SpinLock & _spinLock;
            bool _locked;
        };

	private:
		std::atomic<int> readers_count{ 0 };
		std::atomic<bool> write_now{ false };
    };
}