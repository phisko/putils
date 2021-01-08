#pragma once

#include <atomic>
#include <unordered_map>
#include <future>
#include "BaseModule.hpp"
#include "ThreadPool.hpp"
#include "meta/type.hpp"

namespace putils
{
    class Mediator {
    public:
		Mediator(size_t threads = 0) : _threadPool(threads) {}
        ~Mediator() { running = false; }

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

    public:
		template<typename F>
        void runTask(F && f) {
            _threadPool.runTask(FWD(f));
        }

    public:
        std::atomic<bool> running { true };

    private:
        // Map a type to the modules subscribing to that type
        std::unordered_map<putils::meta::type_index, std::vector<putils::BaseModule*>> _modules;
		ThreadPool _threadPool;
    };
}
