#pragma once

// stl
#include <atomic>

// Taken from https://codereview.stackexchange.com/questions/167641/c-read-write-shared-spin_lock-implementation

namespace putils {
    class PUTILS_CORE_EXPORT spin_lock {
	public:
        void lock() noexcept;
        void unlock() noexcept;

        void lock_shared() noexcept;
        void unlock_shared() noexcept;

        void unlock_and_lock_shared() noexcept;

        struct PUTILS_CORE_EXPORT unique_lock {
            unique_lock(spin_lock & spin_lock) noexcept;
            ~unique_lock() noexcept;

            void lock() noexcept;
            void unlock() noexcept;

            spin_lock & _spin_lock;
            bool _locked;
        };

        struct PUTILS_CORE_EXPORT shared_lock {
            shared_lock(spin_lock & spin_lock) noexcept;
            ~shared_lock() noexcept;
            shared_lock(unique_lock && rhs) noexcept;

            void lock() noexcept;
            void unlock() noexcept;

            spin_lock & _spin_lock;
            bool _locked;
        };

	private:
		std::atomic<int> readers_count{ 0 };
		std::atomic<bool> write_now{ false };
    };
}