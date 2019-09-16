#pragma once

#if defined(__unix__) || defined(__APPLE__)

#include <dlfcn.h>
#include "Library.hpp"

namespace putils
{
    class UnixLibrary final : public Library
    {
        // Constructor
    public:
        template<typename Str>
        UnixLibrary(Str &&name)
                :
                Library(name),
                _handle(dlopen(name.c_str(), RTLD_NOW))
        {
            if (_handle == nullptr)
            {
                std::stringstream s;
                s << "Failed to load library '" << name << "': " << dlerror();
                throw std::runtime_error(s.str());
            }
        }


        // Destructor
    public:
        ~UnixLibrary() noexcept
        {
            if (_handle != nullptr)
                dlclose(_handle);
        }

        // Load a symbol
    public:
        void *loadSymbol(const std::string & name) noexcept override { return dlsym(_handle, name.c_str()); }

        // Attributes
    private:
        void *_handle;

        // Coplien
    public:
        UnixLibrary(const UnixLibrary &) = delete;

        UnixLibrary &operator=(const UnixLibrary &) = delete;
    };
}

#endif