#pragma once

namespace putils {
	template<typename Container, typename Val>
	void erase(Container & container, Val && val) noexcept;
}

#include "erase.inl"