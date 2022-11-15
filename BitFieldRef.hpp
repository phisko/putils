#pragma once

// stl
#include <cstddef>

namespace putils {
	class PUTILS_CORE_EXPORT ConstBitFieldRef {
	public:
		ConstBitFieldRef(const void * data) noexcept;
		
        bool get(size_t index) const noexcept;
        bool get(size_t byteIndex, size_t bitIndex) const noexcept;

	private:
		const char * data;
	};

    class PUTILS_CORE_EXPORT BitFieldRef : ConstBitFieldRef {
	public:
        BitFieldRef(void * data) noexcept;

        void set(size_t index, bool value) noexcept;
        void set(size_t byteIndex, size_t bitIndex, bool value) noexcept;

        void toggle(size_t index) noexcept;
        void toggle(size_t byteIndex, size_t bitIndex) noexcept;
        
	private:
        char * data;
    };
}