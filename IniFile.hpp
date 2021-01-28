#pragma once

#include <map>
#include <string>

namespace putils {
	struct IniFile {
		struct Section {
			std::map<std::string, std::string> values;
		};

		std::map<std::string, Section> sections;
	};

	IniFile parseIniFile(const char * file) noexcept;
	std::ostream & operator<<(std::ostream & s, const IniFile & ini) noexcept;
}