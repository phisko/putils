#pragma once

// stl
#include <string>

// putils
#include "default_constructors.hpp"

namespace putils {
	PUTILS_CORE_EXPORT void set_thread_name(std::string_view name) noexcept;
	PUTILS_CORE_EXPORT std::string get_thread_name() noexcept;

	struct PUTILS_CORE_EXPORT scoped_thread_name {
		PUTILS_DELETE_COPY_AND_MOVE(scoped_thread_name)

		scoped_thread_name(std::string_view name) noexcept {
			set_thread_name(name);
		}

		~scoped_thread_name() noexcept {
			set_thread_name(old_name);
		}

		std::string old_name = get_thread_name();
	};
}