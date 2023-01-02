#ifdef _WIN32

#include "get_last_error_as_string.hpp"

// windows
#include <direct.h>
#include <Windows.h>

// putils
#include "putils/profiling.hpp"

namespace putils {
	std::string get_last_error_as_string() noexcept {
		PUTILS_PROFILING_SCOPE;

		const auto error_message_id = GetLastError();
		if (error_message_id == 0)
			return "";

		LPSTR message_buffer = nullptr;
		const size_t size = FormatMessageA(
			FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
			NULL, error_message_id, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
			(LPSTR)&message_buffer, 0, NULL
		);

		const std::string message(message_buffer, size);

		//Free the buffer.
		LocalFree(message_buffer);

		return message;
	}
}

#endif