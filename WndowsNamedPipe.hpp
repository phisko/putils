#pragma once

namespace putils {
    class WindowsNamedPipe {
    public:
        WindowsNamedPipe(std::string_view fileName) {
            throw std::logic_error("Named pipes have not been implemented on Windows");
        }

        WindowsNamedPipe() {
            throw std::logic_error("Named pipes have not been implemented on Windows");
        }
    };
}
