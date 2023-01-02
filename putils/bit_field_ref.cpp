#include "bit_field_ref.hpp"

namespace putils {
	const_bit_field_ref::const_bit_field_ref(const void * data) noexcept
		: data((const char *)data) {}

	bool const_bit_field_ref::get(size_t index) const noexcept {
		return get(index / 8, index % 8);
	}

	bool const_bit_field_ref::get(size_t byte_index, size_t bit_index) const noexcept {
		return (data[byte_index] >> bit_index) & 1;
	}

	bit_field_ref::bit_field_ref(void * data) noexcept
		: const_bit_field_ref(data),
		  data((char *)data) {}

	void bit_field_ref::set(size_t index, bool value) noexcept {
		set(index / 8, index % 8, value);
	}

	void bit_field_ref::set(size_t byte_index, size_t bit_index, bool value) noexcept {
		if (value)
			data[byte_index] |= (1 << bit_index);
		else
			data[byte_index] &= ~(1 << bit_index);
	}

	void bit_field_ref::toggle(size_t index) noexcept {
		toggle(index / 8, index % 8);
	}

	void bit_field_ref::toggle(size_t byte_index, size_t bit_index) noexcept {
		data[byte_index] ^= (1 << bit_index);
	}
}
