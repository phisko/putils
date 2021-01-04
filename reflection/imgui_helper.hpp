#pragma once

#include "imgui.h"
#include "reflection.hpp"
#include "Color.hpp"
#include "magic_enum.hpp"
#include "lengthof.hpp"
#include "universal_functor.hpp"

namespace putils::reflection {
	template<typename T>
	void imguiEdit(T & obj);

	template<typename T>
	void imguiDisplay(const T & obj);

	template<typename E>
	void imguiEnumCombo(E & e);
}

#include "imgui_helper.inl"