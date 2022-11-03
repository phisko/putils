#pragma once

// stl
#include <map>
#include <string>

namespace putils {
	struct IniFile {
		struct Section {
			std::map<std::string, std::string> values;
            std::map<std::string, Section> sections;
		};

		std::map<std::string, Section> sections;
	};

    std::istream & operator>>(std::istream & s, IniFile & ini) noexcept;
	std::ostream & operator<<(std::ostream & s, const IniFile & ini) noexcept;
}