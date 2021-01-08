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

#include "MemoryPool.hpp"
#include <iostream>

namespace putils {
    template <typename T, size_t BlockSize>
    inline typename MemoryPool<T, BlockSize>::size_type
        MemoryPool<T, BlockSize>::padPointer(data_pointer_ p, size_type align)
        const noexcept
    {
        const auto result = (uintptr_t)p;
        return ((align - result) % align);
    }

    template <typename T, size_t BlockSize>
    void
        MemoryPool<T, BlockSize>::allocateBlock()
    {
        // Allocate space for the new block and store a pointer to the previous one
        const auto newBlock = (data_pointer_)operator new(BlockSize);
        reinterpret_cast<slot_pointer_>(newBlock)->next = currentBlock_;
        currentBlock_ = (slot_pointer_)newBlock;
        // Pad block body to staisfy the alignment requirements for elements
        data_pointer_ body = newBlock + sizeof(slot_pointer_);
        size_type bodyPadding = padPointer(body, alignof(slot_type_));
        currentSlot_ = (slot_pointer_)(body + bodyPadding);
        lastSlot_ = (slot_pointer_)(newBlock + BlockSize - sizeof(slot_type_) + 1);
    }



    template <typename T, size_t BlockSize>
    inline typename MemoryPool<T, BlockSize>::pointer
        MemoryPool<T, BlockSize>::allocate(size_type n, const_pointer hint)
    {
        if (freeSlots_ != nullptr) {
            const auto result = (pointer)freeSlots_;
            freeSlots_ = freeSlots_->next;
            return result;
        }
        else {
            if (currentSlot_ >= lastSlot_)
                allocateBlock();
            return (pointer)currentSlot_++;
        }
    }



    template <typename T, size_t BlockSize>
    inline void
        MemoryPool<T, BlockSize>::deallocate(pointer p, size_type n)
    {
        if (p != nullptr) {
            const auto slot = (slot_pointer_)p;
            slot->next = freeSlots_;
            freeSlots_ = slot;
        }
    }



    template <typename T, size_t BlockSize>
    inline typename MemoryPool<T, BlockSize>::size_type
        MemoryPool<T, BlockSize>::max_size()
        const noexcept
    {
        size_type maxBlocks = -1 / BlockSize;
        return (BlockSize - sizeof(data_pointer_)) / sizeof(slot_type_) * maxBlocks;
    }
}
