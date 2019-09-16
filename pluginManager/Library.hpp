#pragma once

#include <string>
#include <unordered_map>
#include <iostream>

namespace putils {
    //
    // Interface for shared libraries
    //
    class Library {
        // Constructor
    public:
        template<typename String>
        Library(String && name) noexcept : _name(std::forward<String>(name)) {}

        // Destructor
    public:
        virtual ~Library() noexcept = default;

        // Load a symbol
    protected:
        virtual void * loadSymbol(const std::string & name) noexcept = 0;

        // Return a pointer to the [name] function of the library, returning a T and taking P as parameters
        // Uses pseudo-flyweight to avoid reloading symbols
    public:
        template<typename T, typename ...P>
        // T: return, P: param
        T (* loadMethod(const std::string & name))(P ...) {
            if (_symbols.find(name) == _symbols.end()) {
                void * symbol = loadSymbol(name);
                if (!symbol)
                    return NULL;
                _symbols[name] = symbol;
            }

            auto ret = reinterpret_cast<T(*)(P ...)>(_symbols[name]);
            if (ret == nullptr)
                std::cerr << _name << ": Failed to load method '" << name << "'" << std::endl;

            return ret;
        }

        // Executes the [name] function of the library, returning a T and taking P as parameters
    public:
        template<typename T, typename ...P>
        T execute(const std::string & name, P && ...args) {
            return (loadMethod<T, P...>(name))(std::forward<P>(args)...);
        }

        // Name getter
    public:
        std::string_view getName() const noexcept { return _name; }

        // Attributes
    private:
        std::string _name;
        std::unordered_map<std::string, void *> _symbols;

    public:
        Library(const Library &) = delete;

        Library & operator=(const Library &) = delete;
    };
}
