#pragma once

// stl
#include <unordered_map>

// meta
#include "meta/for_each.hpp"

// putils
#include "putils_profiling.hpp"

// mediator
#include "BaseModule.hpp"
#include "Mediator.hpp"

namespace putils {
    template<typename CRTP, typename ...DataPackets>
    class Module : public virtual BaseModule {
    public:
        Module(Mediator * m = nullptr) noexcept : BaseModule(m) {
			PUTILS_PROFILING_SCOPE;

            static_assert(std::is_base_of<Module, CRTP>(),
                          "Module's first template parameter should be its inheriting class");

            putils::for_each_type<DataPackets...>([this](auto && type) {
                using T = putils_wrapped_type(type);
                this->setHandler<T>([this](const auto & p) {
                    static_cast<CRTP *>(this)->handle(p);
                });
            });

			if (m != nullptr)
				m->addModule(*this);
        }

    private:
        const std::vector<putils::meta::type_index> & getSubscriptions() const noexcept final {
            static const std::vector<putils::meta::type_index> _subs = { putils::meta::type<DataPackets>::index... };
            return _subs;
        }

        void handle(std::nullptr_t) noexcept {} // gcc requires a `handle` function to compile the ctor
    };
}
