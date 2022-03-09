#pragma once

#include <string>

namespace putils {
    template<typename Obj, typename EnableIf = std::enable_if_t<!std::is_base_of_v<std::istream, std::decay_t<Obj>>>>
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