#include "casts.hpp"

namespace putils {
    template<typename Derived, typename Base>
    std::unique_ptr<Derived> static_unique_ptr_cast(std::unique_ptr<Base> && p) noexcept {
        auto d = static_cast<Derived *>(p.release());
        return std::unique_ptr<Derived>(d);
    }

    template<typename Derived, typename Base>
    std::unique_ptr<Derived> dynamic_unique_ptr_cast(std::unique_ptr<Base> && p) noexcept {
        auto result = dynamic_cast<Derived *>(p.get());
        if (result != nullptr) {
            p.release();
            return std::unique_ptr<Derived>(result);
        }
        return std::unique_ptr<Derived> (nullptr);
    }
}