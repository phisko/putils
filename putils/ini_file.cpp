#include "ini_file.hpp"

// stl
#include <cassert>
#include <string>
#include <fstream>
#include <iostream>

// putils
#include "regex.hpp"
#include "chop.hpp"
#include "putils/profiling.hpp"

namespace putils {
	static ini_file::section * get_section(ini_file & ini, std::string full_section) noexcept {
		PUTILS_PROFILING_SCOPE;

		ini_file::section * ret = nullptr;

		size_t slash_pos;
		do {
			slash_pos = full_section.find('/');
			const auto subsection = full_section.substr(0, slash_pos);
			if (ret == nullptr)
				ret = &ini.sections[subsection];
			else
				ret = &ret->sections[subsection];

			if (slash_pos != std::string::npos)
				full_section = full_section.substr(slash_pos + 1);
		} while (slash_pos != std::string::npos);

		return ret;
	}

	std::istream & operator>>(std::istream & s, ini_file & ini) noexcept {
		PUTILS_PROFILING_SCOPE;

		ini_file::section * current_section = nullptr;
		for (std::string line; std::getline(s, line);) {
			using namespace putils::regex;
			static const auto pattern = "^\\[(.*)\\]$"_m;
			const auto match = (line.c_str() == pattern);
			if (!match.empty()) {
				current_section = get_section(ini, match[1].str());
				if (current_section == nullptr)
					std::cerr << "Invalid section name '" << match[1].str() << "'" << std::endl;
				continue;
			} else if (current_section == nullptr) {
				std::cerr << "Invalid INI file, should start with a section" << std::endl;
				continue;
			}

			if (putils::chop(line).empty())
				continue;

			const auto index = line.find('=');
			if (index == std::string::npos) {
				std::cerr << "Invalid INI file, should match 'key = value' format" << std::endl;
				continue;
			}

			auto key = std::string(putils::chop(line.substr(0, index)));
			auto value = std::string(putils::chop(line.substr(index + 1)));
			current_section->values[std::move(key)] = std::move(value);
		}

		return s;
	}

	static void serialize_section(std::ostream & s, const std::string & name, const ini_file::section & section) noexcept {
		PUTILS_PROFILING_SCOPE;

		s << '[' << name << ']' << std::endl;
		for (const auto & [key, value] : section.values)
			s << key << '=' << value << std::endl;

		for (const auto & [key, subsection] : section.sections) {
			s << std::endl;
			serialize_section(s, name + '/' + key, subsection);
		}
	}

	std::ostream & operator<<(std::ostream & s, const ini_file & ini) noexcept {
		PUTILS_PROFILING_SCOPE;

		bool first = true;
		for (const auto & [name, section] : ini.sections) {
			if (!first)
				s << std::endl;
			first = false;

			serialize_section(s, name, section);
		}
		return s;
	}
}