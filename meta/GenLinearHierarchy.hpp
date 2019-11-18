#pragma once

namespace putils {
    //
    // Class which, for each type X in Tuple,
    // inherits from T<X>
    //
    template<template<class> class T, typename ...Types>
    class GenLinearHierarchy;

    // Empty tuple specialization (called when Tuple is empty)
    template<template<class> class T>
    class GenLinearHierarchy<T> {};

    //
    // Typical specialization: inherits from T<U>
    // (with U being the first item in Tuple),
    // pops U from Tuple and calls itself again
    //
    template<template<class> class T, typename U, typename ...Types>
    class GenLinearHierarchy<T, U, Types...>
            : public T<U>, public GenLinearHierarchy<T, Types...> {
    };
}
