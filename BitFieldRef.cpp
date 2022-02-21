#include "BitFieldRef.hpp"

namespace putils {
	ConstBitFieldRef::ConstBitFieldRef(const void * data) noexcept
	: data((const char *)data)
	{}

	bool ConstBitFieldRef::get(size_t index) const noexcept {
		return get(index / 8, index % 8);
	}

	bool ConstBitFieldRef::get(size_t byteIndex, size_t bitIndex) const noexcept {
		return (data[byteIndex] >> bitIndex) & 1;
	}

	BitFieldRef::BitFieldRef(void * data) noexcept
	: ConstBitFieldRef(data)
	, data((char *)data)
	{}

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
		data[byteIndex] ^= (1 << bitIndex);
	}
}
