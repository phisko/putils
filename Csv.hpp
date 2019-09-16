#pragma once

#include <vector>
#include <string>

namespace putils {
	std::vector<std::string> parseCSVLine(const std::string & s, char delim) {
		static size_t lines = 0;

		++lines;
		std::vector<std::string> ret;

		size_t startIndex = 0;
		bool stop = false;
		while (!stop) {
			size_t nextStartIndex;

			size_t endIndex;
			if (s[startIndex] == '"') {
				++startIndex; // Pop opening quote
				const auto closingQuote = s.find_first_of('"', startIndex);
				endIndex = s.find_first_of(delim, closingQuote);
				if (endIndex == std::string::npos) {
					endIndex = s.size() - 1;
					stop = true;
				}
				nextStartIndex = endIndex + 1;
				--endIndex; // Pop closing quote
			}
			else {
				endIndex = s.find_first_of(delim, startIndex);
				nextStartIndex = endIndex + 1;
			}

			ret.push_back(s.substr(startIndex, endIndex == std::string::npos ? endIndex : endIndex - startIndex));
			if (endIndex == std::string::npos)
				break;

			startIndex = nextStartIndex;
		}

		return ret;
	}
}
