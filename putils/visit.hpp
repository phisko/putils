#pragma once

// std::visit helper

namespace putils {
	template<typename ... Ts>
	struct overloaded : Ts... {
		using Ts::operator()...;
	};
	template<typename ... Ts>
	overloaded(Ts...) -> overloaded<Ts...>;
}
