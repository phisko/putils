#pragma once

// stl
#include <atomic>
#include <unordered_map>
#include <future>

// meta
#include "meta/type.hpp"

// mediator
#include "BaseModule.hpp"

namespace putils
{
    class PUTILS_MEDIATOR_EXPORT Mediator {
    public:
        void addModule(BaseModule &m);
        void removeModule(BaseModule &m);

    public:
        template<typename T>
        void send(T &&data) {
            sendDataPacket(DataPacket<T>(std::forward<T>(data)));
        }

    public:
        void sendDataPacket(const ADataPacket &packet);

    private:
        // Map a type to the modules subscribing to that type
        std::unordered_map<putils::meta::type_index, std::vector<putils::BaseModule*>> _modules;
    };
}
