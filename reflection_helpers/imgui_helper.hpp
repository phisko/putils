#pragma once

namespace putils::reflection {
	template<typename T>
	void imguiEdit(T && obj) noexcept;

	template<typename T>
	void imguiEdit(const char * label, T && obj) noexcept;

	template<typename E>
	bool imguiEnumCombo(const char * label, E & e) noexcept;
}

#include "imgui_helper.inl"