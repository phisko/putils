#pragma once

// stl
#include <map>
#include <string>

namespace putils {
	struct PUTILS_CORE_EXPORT ini_file {
		struct section {
			std::map<std::string, std::string> values;
            std::map<std::string, section> sections;
		};

		std::map<std::string, section> sections;
	};

    PUTILS_CORE_EXPORT std::istream & operator>>(std::istream & s, ini_file & ini) noexcept;
	PUTILS_CORE_EXPORT std::ostream & operator<<(std::ostream & s, const ini_file & ini) noexcept;
}