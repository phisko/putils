#pragma once

#include <atomic>

// Taken from https://codereview.stackexchange.com/questions/167641/c-read-write-shared-spinlock-implementation

namespace putils {
    class SpinLock {
        std::atomic<int> readers_count{ 0 };
        std::atomic<bool> write_now{ false };

    public:
        void lock() noexcept {
            while (write_now.exchange(true, std::memory_order_acquire))
                ;

            // wait for readers to exit
            while (readers_count != 0)
                ;
        }

        void unlock() noexcept {
            write_now.store(false, std::memory_order_release);
        }

        void lock_shared() noexcept {
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

        void unlock_shared() noexcept {
            readers_count.fetch_sub(1, std::memory_order_release);
        }

        void unlock_and_lock_shared() {
            readers_count.fetch_add(1, std::memory_order_acquire);
            write_now.store(false, std::memory_order_release);
        }

        struct Lock {
            Lock(SpinLock & spinLock) noexcept : _spinLock(spinLock) {
                lock();
            }

            ~Lock() noexcept {
                if (_locked)
					unlock();
            }

            void lock() noexcept {
                _spinLock.lock();
                _locked = true;
            }

            void unlock() noexcept {
                _spinLock.unlock();
                _locked = false;
            }

            SpinLock & _spinLock;
            bool _locked;
        };

        struct SharedLock {
            SharedLock(SpinLock & spinLock) noexcept : _spinLock(spinLock) {
                lock();
            }

            ~SharedLock() noexcept {
                if (_locked)
					unlock();
            }

            SharedLock(Lock && rhs) noexcept : _spinLock(rhs._spinLock) {
                rhs._spinLock.unlock_and_lock_shared();
            }

            void lock() noexcept {
                _spinLock.lock_shared();
                _locked = true;
            }

            void unlock() noexcept {
                _spinLock.unlock_shared();
                _locked = false;
            }

            SpinLock & _spinLock;
            bool _locked;
        };
    };
}