#pragma once

#include <string>
#include <unordered_map>
#include <iostream>

namespace putils {
    //
    // Interface for shared libraries
    //
    class Library {
    public:
        template<typename String>
        Library(String && name) noexcept;
        virtual ~Library() noexcept = default;

        Library(const Library &) = delete;
        Library & operator=(const Library &) = delete;

    protected:
        virtual void * loadSymbol(const std::string & name) noexcept = 0;

        // Return a pointer to the [name] function of the library, returning a T and taking P as parameters
        // Uses pseudo-flyweight to avoid reloading symbols
    public:
        template<typename T, typename ...P>
        using Func = T(*)(P...);

        template<typename T, typename ...P>
        Func<T, P...> loadMethod(const std::string & name) noexcept;

        // Executes the [name] function of the library, returning a T and taking P as parameters
    public:
        template<typename T, typename ...P>
        T execute(const std::string & name, P && ...args) noexcept;

        // Name getter
    public:
        std::string_view getName() const noexcept { return _name; }

        // Attributes
    private:
        std::string _name;
        std::unordered_map<std::string, void *> _symbols;
    };
}

#include "Library.inl"
