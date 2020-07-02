#pragma once

#include <unordered_map>
#include <string>

namespace putils {
	struct IniFile {
		struct Section {
			std::unordered_map<std::string, std::string> values;
		};

		std::unordered_map<std::string, Section> sections;
	};

	IniFile parseIniFile(const char * file) noexcept;
}