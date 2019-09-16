#pragma once

#ifdef __unix__

#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#elif defined(_WIN32)

#include <io.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <windows.h>

#endif

#include <stdexcept>

#include "connection.hpp"

namespace putils {
    class ATCPConnection {
    protected:
        virtual int doRead(int fd, char * dest, int length) noexcept = 0;
        virtual int doWrite(int fd, const char * data, int length) noexcept = 0;

    public:
        virtual ~ATCPConnection() {
            connection::closeSocket(_socket);
            connection::add(-1);
        }

    public:
        ATCPConnection(const std::string & host, short port, bool verbose = false)
                : _verbose(verbose) {
            connection::add(1);

            _socket = socket(AF_INET, SOCK_STREAM, 0);
            if (_socket < 0)
                throw std::runtime_error("Failed to create socket");

            struct sockaddr_in server;
            server.sin_family = AF_INET;
            server.sin_port = htons(port);
            server.sin_addr.s_addr = inet_addr(host.c_str());

            if (connect(_socket, (struct sockaddr *) (&server), sizeof(server)) == -1)
                throw std::runtime_error("Failed to connect");
        }

    public:
        template<typename T>
        void send(const T & data) {
            doSend(reinterpret_cast<const char *>(&data), sizeof(data));
        }

    private:
        void doSend(const char * data, size_t length) {
            size_t written = 0;
            while (written < length) {
                auto ret = doWrite(_socket, data + written, length);
                if (ret < 0)
                    throw std::runtime_error("Failed to write");
                written += ret;
            }
        }

    public:
        std::string receive() {
            char buff[1025];
            std::string total;

            int ret;
            while ((ret = doRead(_socket, buff, sizeof(buff) - 1)) == sizeof(buff) - 1) {
                buff[ret] = 0;
                total += buff;
            }

            if (ret > 0) {
                buff[ret] = 0;
                total += buff;
            } else if (ret < 0)
                throw std::runtime_error("Failed to read");

            return total;
        }

        template<typename T>
        void receive(T & dest) {
            size_t total = 0;

            int ret;
            while ((ret = doRead(_socket, (char *) &dest + total, sizeof(T) - 1)) == sizeof(T) - 1)
                total += ret;

            if (ret < 0)
                throw std::runtime_error("Failed to read");
        }

    protected:
        bool isVerbose() const { return _verbose; }
        int getSocket() { return _socket; }

    private:
        int _socket;
        bool _verbose;
    };

    template<>
    inline void ATCPConnection::send(const std::string_view & msg) {
        doSend(msg.data(), msg.length());
    }
}
