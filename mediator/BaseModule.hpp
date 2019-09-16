#pragma once

#include <vector>
#include <functional>
#include "DataPacket.hpp"
#include "ThreadPool.hpp"

namespace putils {
    class Mediator;

    class BaseModule {
    public:
        BaseModule(Mediator * mediator = nullptr);
        virtual ~BaseModule();

    public:
        BaseModule(const BaseModule &) = delete;
        BaseModule & operator=(const BaseModule &) = delete;

    public:
        virtual const std::vector<pmeta::type_index> & getSubscriptions() const noexcept = 0;
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
        void runTask(const ThreadPool::function & f) const;

	protected:
        template<typename Data>
        void setHandler(const std::function<void(const Data &)> & handler) {
            _handlers[pmeta::type<Data>::index] = [this, handler](const ADataPacket & packet) {
                handler(static_cast<const putils::DataPacket<Data> &>(packet).data);
            };
        }

    private:
        Mediator * _mediator;
        std::unordered_map<pmeta::type_index, std::function<void(const putils::ADataPacket &)>> _handlers;
    };
}
