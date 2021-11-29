#include "BitFieldRef.hpp"

namespace putils {
	BitFieldRef::BitFieldRef(void * data) noexcept
	: data((char *)data)
	{}

	bool BitFieldRef::get(size_t index) const noexcept {
		return get(index / 8, index % 8);
	}

	bool BitFieldRef::get(size_t byteIndex, size_t bitIndex) const noexcept {
		return (data[byteIndex] >> bitIndex) & 1;
	}

	void BitFieldRef::set(size_t index, bool value) noexcept {
		set(index / 8, index % 8, value);
	}

	void BitFieldRef::set(size_t byteIndex, size_t bitIndex, bool value) noexcept {
		if (value)
			data[byteIndex] |= (1 << bitIndex);
		else
			data[byteIndex] &= ~(1 << bitIndex);
	}

	void BitFieldRef::toggle(size_t index) noexcept {
		toggle(index / 8, index % 8);
	}

	void BitFieldRef::toggle(size_t byteIndex, size_t bitIndex) noexcept {
		data[byteIndex] ^= get(byteIndex, bitIndex);
	}
}
