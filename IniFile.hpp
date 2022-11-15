#pragma once

// stl
#include <map>
#include <string>

namespace putils {
	struct PUTILS_CORE_EXPORT IniFile {
		struct Section {
			std::map<std::string, std::string> values;
            std::map<std::string, Section> sections;
		};

		std::map<std::string, Section> sections;
	};

    PUTILS_CORE_EXPORT std::istream & operator>>(std::istream & s, IniFile & ini) noexcept;
	PUTILS_CORE_EXPORT std::ostream & operator<<(std::ostream & s, const IniFile & ini) noexcept;
}