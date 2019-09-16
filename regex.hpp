#pragma once

#include <string>
#include <regex>

bool operator==(const std::string & str, const std::regex & reg) {
    return std::regex_match(str.c_str(), reg);
}

std::cmatch operator==(const std::string & str, const std::pair<std::regex, bool> & reg) {
    std::cmatch m;
    std::regex_match(str.c_str(), m, reg.first);
    return m;
}

std::pair<std::regex, bool> operator ""_m(const char * str, size_t) {
    return std::make_pair(std::regex(str), true);
}

std::regex operator ""_r(const char * str, size_t) {
    return std::regex(str);
}
