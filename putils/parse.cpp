#include "parse.hpp"

// stl
#include <sstream>

// putils
#include "read_stream.hpp"
#include "profiling.hpp"

namespace putils {
	template<>
	void parse(bool & obj, std::string_view str) noexcept {
		obj = str == "true";
	};

	template<>
	void parse(std::string & obj, std::string_view str) noexcept {
		obj = str;
	}

	template<>
	bool parse(std::string_view str) noexcept {
		return str == "true";
	}

	template<>
	std::string parse(std::string_view str) noexcept {
		return std::string(str);
	}
}