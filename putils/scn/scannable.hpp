#pragma once

// scn
#include <scn/scn.h>

namespace putils {
	template<typename T>
	concept scannable = requires {
		scn::scanner<T>{};
	};
}