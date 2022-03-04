/*-
 * Copyright (c) 2013 Cosku Acay, http://www.coskuacay.com
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
 * IN THE SOFTWARE.
 */

#pragma once

#include <climits>
#include <cstddef>

namespace putils {
    template <typename T, size_t BlockSize = 4096>
    class MemoryPool
    {
    public:
        /* Member types */
        typedef T               value_type;
        typedef T * pointer;
        typedef T & reference;
        typedef const T * const_pointer;
        typedef const T & const_reference;
        typedef size_t          size_type;
        typedef ptrdiff_t       difference_type;
        typedef std::false_type propagate_on_container_copy_assignment;
        typedef std::true_type  propagate_on_container_move_assignment;
        typedef std::true_type  propagate_on_container_swap;

        template <typename U> struct rebind {
            typedef MemoryPool<U, BlockSize> other;
        };

        /* Member functions */
        MemoryPool() noexcept = default;
        ~MemoryPool() noexcept = default;

        MemoryPool(const MemoryPool & memoryPool) noexcept = default;
        MemoryPool & operator=(const MemoryPool & memoryPool) = default;
        MemoryPool(MemoryPool && memoryPool) noexcept = default;
        MemoryPool & operator=(MemoryPool && memoryPool) noexcept = default;

        template <class U, size_t B>
        MemoryPool(const MemoryPool<U, B> & memoryPool) noexcept {}

        // Can only allocate one object at a time. n and hint are ignored
        pointer allocate(size_type n = 1, const_pointer hint = 0);
        void deallocate(pointer p, size_type n = 1);

        size_type max_size() const noexcept;

    private:
        union Slot_ {
            value_type element;
            Slot_ * next;

            ~Slot_() noexcept = delete;
        };

        typedef char * data_pointer_;
        typedef Slot_ slot_type_;
        typedef Slot_ * slot_pointer_;

        static inline slot_pointer_ currentBlock_ = nullptr;
        static inline slot_pointer_ currentSlot_ = nullptr;
        static inline slot_pointer_ lastSlot_ = nullptr;
        static inline slot_pointer_ freeSlots_ = nullptr;

        size_type padPointer(data_pointer_ p, size_type align) const noexcept;
        void allocateBlock();

        static_assert(BlockSize >= 2 * sizeof(slot_type_), "BlockSize too small.");
    };
}

#include "MemoryPool.inl"