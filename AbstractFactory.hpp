#pragma once

#include <memory>
#include "meta/type.hpp"
#include "meta/GenLinearHierarchy.hpp"

namespace putils {
    // Abstract Factory inherits from this for each type in TList
    template<typename T>
    class AbstractFactoryUnit {
        // Create a T
    public:
        virtual std::unique_ptr<T> makeImpl(putils::meta::type<T>) noexcept = 0;
    };

    //
    // AbstractFactory able to build any type in TList
    // To implement a concrete factory, simply inherit from AbstractFactory
    // and overload std::unique_ptr<T>	make(putils::meta::type<T>)
    //
    // To build an object of type T, use factory.make<T>()
    //
    template<typename ...Types>
    class AbstractFactory : public putils::GenLinearHierarchy<AbstractFactoryUnit, Types...> {
        // Make an object of type T by casting myself to the right AbstractFactoryUnit
    public:
        template<typename T>
        std::unique_ptr<T> make() noexcept {
            AbstractFactoryUnit<T> & unit = static_cast<AbstractFactoryUnit<T> &>(*this);
            return unit.makeImpl(putils::meta::type<T>());
        }
    };
}

