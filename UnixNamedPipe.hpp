#pragma once

#include <fstream>
#include <sys/types.h>
#include <sys/stat.h>

#include "concat.hpp"

namespace putils {
    class UnixNamedPipe : public std::fstream {
    public:
        template<typename String>
        UnixNamedPipe(String && fileName)
                : _fileName(FWD(fileName)) {
            this->open(_fileName.c_str());
        }
        UnixNamedPipe() = default;

    public:
        ~UnixNamedPipe() {
            this->close();
        }

    public:
        void open(const std::string & fileName, std::ifstream::openmode mode = std::ifstream::in | std::ifstream::out) {
            this->close();

            mkfifo(fileName.c_str(), 0666); // Ignore error
            std::fstream::open(fileName.c_str(), mode);
            _fileName = fileName;
        }

        void close(bool erase = true) {
            if (!is_open())
                return;

            std::fstream::close();
            if (erase)
                system(putils::concat("rm -f ", _fileName).c_str());

            _fileName = "";
        }

    private:
        std::string _fileName;
    };
}