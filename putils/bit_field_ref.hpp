#pragma once

// stl
#include <cstddef>

namespace putils {
	class PUTILS_CORE_EXPORT const_bit_field_ref {
	public:
		const_bit_field_ref(const void * data) noexcept;

		bool get(size_t index) const noexcept;
		bool get(size_t byte_index, size_t bit_index) const noexcept;

	private:
		const char * data;
	};

	class PUTILS_CORE_EXPORT bit_field_ref : const_bit_field_ref {
	public:
		bit_field_ref(void * data) noexcept;

		void set(size_t index, bool value) noexcept;
		void set(size_t byte_index, size_t bit_index, bool value) noexcept;

		void toggle(size_t index) noexcept;
		void toggle(size_t byte_index, size_t bit_index) noexcept;

	private:
		char * data;
	};
}