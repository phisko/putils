#pragma once

namespace putils::reflection {
	template<typename T>
	void imguiEdit(T & obj);

	template<typename T>
	void imguiDisplay(const T & obj);

	template<typename E>
	void imguiEnumCombo(E & e);
}

#include "imgui_helper.inl"