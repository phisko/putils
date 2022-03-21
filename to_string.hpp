#pragma once

#include <string>
#include <string_view>

namespace putils {
    template<typename Obj, typename EnableIf = std::enable_if_t<!std::is_base_of_v<std::istream, std::decay_t<Obj>>>>
    std::string toString(Obj && obj) noexcept;
    std::string toString(bool v) noexcept;
    std::string toString(std::istream & s) noexcept;

    template<typename Obj>
    void parse(Obj & obj, std::string_view str) noexcept;;

    template<>
    void parse(bool & obj, std::string_view str) noexcept;

	template<>
    void parse(std::string & obj, std::string_view str) noexcept;

    template<typename Obj>
    Obj parse(std::string_view str) noexcept;;

    template<>
    bool parse(std::string_view str) noexcept;

    template<>
    std::string parse(std::string_view str) noexcept;
}

#include "to_string.inl"