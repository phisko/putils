#pragma once

namespace putils {
    class BitFieldRef {
	public:
        BitFieldRef(void * data) noexcept;

        bool get(size_t index) const noexcept;
        bool get(size_t byteIndex, size_t bitIndex) const noexcept;

        void set(size_t index, bool value) noexcept;
        void set(size_t byteIndex, size_t bitIndex, bool value) noexcept;

        void toggle(size_t index) noexcept;
        void toggle(size_t byteIndex, size_t bitIndex) noexcept;
        
	private:
        char * data;
    };
}