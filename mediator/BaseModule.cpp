#include "Module.hpp"

namespace putils {
    BaseModule::BaseModule(Mediator * mediator)
            : _mediator(mediator) {
    }

    BaseModule::~BaseModule() {
        if (_mediator)
            _mediator->removeModule(*this);
    }

    void BaseModule::sendDataPacket(const ADataPacket & packet) const noexcept {
        _mediator->sendDataPacket(packet);
    }

    void BaseModule::sendDataPacketTo(const ADataPacket & packet, BaseModule & dest) const noexcept {
        dest.receive(packet);
    }

    void BaseModule::runTask(const ThreadPool::function & f) const {
        _mediator->runTask(f);
    }
}
