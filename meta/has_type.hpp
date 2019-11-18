#pragma once

#include <tuple>
#include <type_traits>

namespace putils {
    //
    // Boolean structure
    // inheriting from std::true_type if Tuple contains T
    //
    template<typename T, typename ...Types>
    struct has_type;

    // Empty tuple specialization: type wasn't found,
    // inherit from std::false_type
    template<typename T>
    struct has_type<T> : std::false_type {};

    // Bad element specialization: first type in tuple
    // isn't the type I'm looking for.
    // Pop the first type in Tuple and go recursive
    template<typename T, typename U, typename... Types>
    struct has_type<T, U, Types...> : has_type<T, Types...> {};

    // Good element specialization: first type in tuple
    // is the type I'm looking for.
    // Inherit from std::true_type
    template<typename T, typename ...Types>
    struct has_type<T, T, Types...> : std::true_type {};
}