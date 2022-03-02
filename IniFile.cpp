#include <assert.h>
#include <string>
#include <fstream>
#include <iostream>
#include "IniFile.hpp"
#include "regex.hpp"
#include "chop.hpp"

namespace putils {
    static IniFile::Section * getSection(IniFile & ini, std::string fullSection) noexcept {
        IniFile::Section * ret = nullptr;

        size_t slashPos;
        do {
            slashPos = fullSection.find('/');
            const auto subsection = fullSection.substr(0, slashPos);
            if (ret == nullptr)
                ret = &ini.sections[subsection];
            else
                ret = &ret->sections[subsection];

            if (slashPos != std::string::npos)
                fullSection = fullSection.substr(slashPos + 1);
        } while (slashPos != std::string::npos);

        return ret;
    }

    std::istream & operator>>(std::istream & s, IniFile & ini) noexcept {
		IniFile::Section * currentSection = nullptr;
		for (std::string line; std::getline(s, line);) {
			using namespace putils::regex;
			static const auto pattern = "^\\[(.*)\\]$"_m;
			const auto match = (line.c_str() == pattern);
			if (!match.empty()) {
                currentSection = getSection(ini, match[1].str());
                if (currentSection == nullptr)
                    std::cerr << "Invalid section name '" << match[1].str() << "'\n";
				continue;
			}
			else if (currentSection == nullptr) {
				std::cerr << "Invalid INI file, should start with a section\n";
				continue;
			}

			if (putils::chop(line).empty())
				continue;

			const auto index = line.find('=');
			if (index == std::string::npos) {
				std::cerr << "Invalid INI file, should match 'key = value' format\n";
				continue;
			}

			auto key = std::string(putils::chop(line.substr(0, index)));
			auto value = std::string(putils::chop(line.substr(index + 1)));
			currentSection->values[std::move(key)] = std::move(value);
		}

		return s;
	}

    static void serializeSection(std::ostream & s, const std::string & name, const IniFile::Section & section) noexcept {
        s << '[' << name << "]\n";
        for (const auto & [key, value] : section.values)
            s << key << '=' << value << '\n';

        for (const auto & [key, subsection] : section.sections) {
            s << '\n';
            serializeSection(s, name + '/' + key, subsection);
        }
    }

	std::ostream & operator<<(std::ostream & s, const IniFile & ini) noexcept {
        bool first = true;
		for (const auto & [name, section] : ini.sections) {
            if (!first)
                s << '\n';
            first = false;

            serializeSection(s, name, section);
        }
		return s;
	}
}