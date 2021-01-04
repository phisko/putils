#pragma once

#if defined(__unix__) || defined(__APPLE__)

#include <string_view>
#include "Library.hpp"

namespace putils {
    class UnixLibrary final : public Library {
    public:
        UnixLibrary(std::string_view name) noexcept;
        ~UnixLibrary() noexcept;

        void * loadSymbol(const std::string & name) noexcept override;

    private:
        void *_handle = nullptr;
    };
}

#endif