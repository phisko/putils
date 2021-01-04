#pragma once

#include <sstream>
#include "read_stream.hpp"
#include "fwd.hpp"

namespace putils {
    template<typename Obj>
    std::string toString(Obj && obj) noexcept;
    std::string toString(bool v) noexcept;
    std::string toString(std::istream & s) noexcept;

    template<typename Obj>
    void parse(Obj & obj, const char * str) noexcept;;

    template<>
    void parse(bool & obj, const char * str) noexcept;

	template<>
    void parse(std::string & obj, const char * str) noexcept;

    template<typename Obj>
    Obj parse(const char * str) noexcept;;

    template<>
    bool parse(const char * str) noexcept;

    template<>
    std::string parse(const char * str) noexcept;
}

#include "to_string.inl"