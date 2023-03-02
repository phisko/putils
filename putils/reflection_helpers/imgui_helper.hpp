#pragma once

namespace putils::reflection {
	template<typename T>
	bool imgui_edit(T && obj) noexcept;

	template<typename T>
	bool imgui_edit(const char * label, T && obj) noexcept;

	template<typename E>
	bool imgui_enum_combo(const char * label, E & e) noexcept;
}

#include "imgui_helper.inl"