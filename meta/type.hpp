#pragma once

#include <typeindex>
#include <type_traits>

#define putils_typeof(object) std::decay_t<decltype(object)>
#define putils_wrapped_type(typeObject) typename putils_typeof(typeObject)::wrapped

namespace putils::meta {
    using type_index = std::size_t;

    template<typename T>
    struct type {
        using wrapped = T;
        static const type_index index;
    };
}

template<typename T>
const putils::meta::type_index putils::meta::type<T>::index = std::hash<std::type_index>()(std::type_index(typeid(T)));

template<typename T>
bool operator==(putils::meta::type<T>, putils::meta::type<T>) noexcept { return true; }

