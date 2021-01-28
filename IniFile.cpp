#include <assert.h>
#include <string>
#include <fstream>
#include <iostream>
#include "IniFile.hpp"
#include "regex.hpp"
#include "chop.hpp"

namespace putils {
	IniFile parseIniFile(const char * file) noexcept {
		IniFile ret;

		std::ifstream f(file);
		if (!f)
			return ret;

		IniFile::Section * currentSection = nullptr;
		for (std::string line; std::getline(f, line);) {
			using namespace putils::regex;
			static const auto pattern = "^\\[(.*)\\]$"_m;
			const auto match = (line.c_str() == pattern);
			if (!match.empty()) {
				currentSection = &ret.sections[match[1].str()];
				continue;
			}
			else if (currentSection == nullptr) {
				std::cerr << "[" << file << "] Invalid INI file, should start with a section\n";
				continue;
			}

			if (putils::chop(line).empty())
				continue;

			const auto index = line.find('=');
			if (index == std::string::npos) {
				std::cerr << "[" << file << "] Invalid INI file, should match 'key = value' format\n";
				continue;
			}

			const auto key = putils::chop(line.substr(0, index));
			const auto value = putils::chop(line.substr(index + 1));
			currentSection->values[key] = value;
		}

		return ret;
	}

	std::ostream & operator<<(std::ostream & s, const IniFile & ini) noexcept {
		for (const auto & [name, section] : ini.sections) {
			s << '[' << name << "]\n";
			for (const auto & [key, value] : section.values)
				s << key << '=' << value << '\n';
			s << '\n';
		}

		return s;
	}
}