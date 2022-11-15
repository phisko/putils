#pragma once

// stl
#include <vector>
#include <functional>

// mediator
#include "DataPacket.hpp"
#include "ThreadPool.hpp"

namespace putils {
    class Mediator;

    class PUTILS_MEDIATOR_EXPORT BaseModule {
    public:
        BaseModule(Mediator * mediator = nullptr) noexcept;
        virtual ~BaseModule() noexcept;

    public:
        virtual const std::vector<putils::meta::type_index> & getSubscriptions() const noexcept = 0;
        void receive(const putils::ADataPacket & packet) noexcept {
			_handlers[packet.type](packet);
        }

    public:
        void setMediator(Mediator * mediator) { _mediator = mediator; }
        const Mediator * getMediator() const { return _mediator; }
        Mediator * getMediator() { return _mediator; }

    public:
        template<typename T>
        void send(const T & data) const {
            sendDataPacket(DataPacket<T>(data));
        }

        template<typename T>
        void sendTo(const T & data, BaseModule & dest) const {
            sendDataPacketTo(DataPacket<T>(data), dest);
        }

    private:
        void sendDataPacket(const ADataPacket & packet) const noexcept;
        void sendDataPacketTo(const ADataPacket & packet, BaseModule & dest) const noexcept;

	protected:
        template<typename Data>
        void setHandler(const std::function<void(const Data &)> & handler) {
            _handlers[putils::meta::type<Data>::index] = [this, handler](const ADataPacket & packet) {
                handler(static_cast<const putils::DataPacket<Data> &>(packet).data);
            };
        }

    private:
        Mediator * _mediator;
        std::unordered_map<putils::meta::type_index, std::function<void(const putils::ADataPacket &)>> _handlers;
    };
}
