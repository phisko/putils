#include "Module.hpp"

// putils
#include "putils_profiling.hpp"

namespace putils {
    BaseModule::BaseModule(Mediator * mediator) noexcept
        : _mediator(mediator) {
    }

    BaseModule::~BaseModule() noexcept {
        if (_mediator)
            _mediator->removeModule(*this);
    }

    void BaseModule::sendDataPacket(const ADataPacket & packet) const noexcept {
		PUTILS_PROFILING_SCOPE;
        _mediator->sendDataPacket(packet);
    }

    void BaseModule::sendDataPacketTo(const ADataPacket & packet, BaseModule & dest) const noexcept {
		PUTILS_PROFILING_SCOPE;
        dest.receive(packet);
    }
}
