#pragma once

#include <iostream>
#include "ATCPConnection.hpp"

#ifdef __unix__

# include <signal.h>

#endif

#include "fwd.hpp"

namespace putils {
    class TCPConnection final : public ATCPConnection {
    public:
        template<typename String>
        TCPConnection(String && host, short port, bool verbose = false)
                : ATCPConnection(FWD(host), port, verbose) {
#ifdef __unix__
            signal(SIGPIPE, SIG_IGN);
#endif
        }

    private:
        int doRead(int fd, char * dest, int length) noexcept { return (int) recv(fd, dest, (size_t) length, 0); }

        int doWrite(int fd, const char * data, int length) noexcept { return (int) ::send(fd, data, (size_t) length, 0); }
    };
}
