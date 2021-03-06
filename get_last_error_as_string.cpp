#ifdef _WIN32

#include "get_last_error_as_string.hpp"

#include <direct.h>
#include <Windows.h>

namespace putils {
    std::string GetLastErrorAsString() noexcept {
        auto errorMessageID = GetLastError();
        if (errorMessageID == 0)
            return "";

        LPSTR messageBuffer = nullptr;
        size_t size = FormatMessageA(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
                                     NULL, errorMessageID, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPSTR)&messageBuffer, 0, NULL);

        std::string message(messageBuffer, size);

        //Free the buffer.
        LocalFree(messageBuffer);

        return message;
    }
}

#endif