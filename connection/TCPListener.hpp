#pragma once

#ifdef __unix__

# include <signal.h>

#endif

#include "ATCPListener.hpp"

namespace putils {
    class TCPListener final : public ATCPListener {
    public:
        TCPListener(short port, const std::string & host = "127.0.0.1", bool verbose = false)
                : ATCPListener(port, host, verbose) {}

    private:
        void doRemove(int) noexcept {}

        bool doAccept(int) noexcept { return true; }

        int doRead(int fd, char * dest, size_t length) noexcept { return (int) recv(fd, dest, length, 0); }

        int doWrite(int fd, const char * data, size_t length) noexcept { return (int) ::send(fd, data, length, 0); }
    };
}