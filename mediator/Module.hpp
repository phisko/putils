#pragma once

#include <unordered_map>
#include "BaseModule.hpp"
#include "meta/for_each.hpp"
#include "Mediator.hpp"

namespace putils {
    template<typename CRTP, typename ...DataPackets>
    class Module : public virtual BaseModule {
    public:
        Module(Mediator * m = nullptr) : BaseModule(m) {
            static_assert(std::is_base_of<Module, CRTP>::value,
                          "Module's first template parameter should be its inheriting class");

            putils::tuple_for_each(std::tuple<putils::meta::type<DataPackets>...>(),
                                  [this](auto && type) {
                                      this->setHandler<putils_wrapped_type(type)>(
                                              [this](const auto & p) { static_cast<CRTP *>(this)->handle(p); }
                                      );
                                  });

			if (m != nullptr)
				m->addModule(*this);
        }

    public:
        template<typename Response, typename Query>
        Response query(Query && q) // q: DataPacket containing a "putils::BaseModule *sender" field
        {
            return Handler<Response>(getMediator(), FWD(q)).res;
        }

    private:
        template<typename Response>
        struct Handler : Module<Handler<Response>, Response> {
            template<typename Q>
            Handler(Mediator * m, Q && query) : BaseModule(m) {
                if constexpr (std::is_pointer<Response>::value)
                    res = nullptr;
                query.sender = this;
                this->send(FWD(query));
            }

            void handle(const Response & response) { res = response; }

            Response res;
        };

    private:
        const std::vector<putils::meta::type_index> & getSubscriptions() const noexcept final {
            static const std::vector<putils::meta::type_index> _subs = { putils::meta::type<DataPackets>::index... };
            return _subs;
        }
    };
}
