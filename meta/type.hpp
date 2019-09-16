#pragma once

#include <typeindex>

#define pmeta_typeof(object) std::decay_t<decltype(object)>
#define pmeta_wrapped(typeObject) typename pmeta_typeof(typeObject)::wrapped

namespace pmeta {
    using type_index = std::size_t;

    template<typename T>
    struct type {
        using wrapped = T;
        static const type_index index;
    };
}

template<typename T>
const pmeta::type_index pmeta::type<T>::index = std::hash<std::type_index>()(std::type_index(typeid(T)));

template<typename T>
bool operator==(pmeta::type<T>, pmeta::type<T>) { return true; }

