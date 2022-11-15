#pragma once

// stl
#include <string>
#include <string_view>

namespace putils {
    template<typename Obj, typename EnableIf = std::enable_if_t<!std::is_base_of_v<std::istream, std::decay_t<Obj>>>>
    std::string toString(Obj && obj) noexcept;
    PUTILS_CORE_EXPORT std::string toString(bool v) noexcept;
    PUTILS_CORE_EXPORT std::string toString(std::istream & s) noexcept;

    template<typename Obj>
    void parse(Obj & obj, std::string_view str) noexcept;;

    template<>
    PUTILS_CORE_EXPORT void parse(bool & obj, std::string_view str) noexcept;

	template<>
    PUTILS_CORE_EXPORT void parse(std::string & obj, std::string_view str) noexcept;

    template<typename Obj>
    Obj parse(std::string_view str) noexcept;;

    template<>
    PUTILS_CORE_EXPORT bool parse(std::string_view str) noexcept;

    template<>
    PUTILS_CORE_EXPORT std::string parse(std::string_view str) noexcept;
}

#include "to_string.inl"