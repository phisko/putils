#include "get_call_stack.hpp"

// putils
#include "lengthof.hpp"
#include "string.hpp"
#include "profiling.hpp"

// windows
#ifdef _MSC_VER
#include "windows.h"
#include "dbghelp.h"
#endif

namespace putils {
	std::string get_call_stack(int frames_to_ignore) noexcept {
		PUTILS_PROFILING_SCOPE;

		std::string ret;
#ifdef _MSC_VER
		const auto process = GetCurrentProcess();
		SymInitialize(process, nullptr, true);

		void * stack[128];
		const auto frames = CaptureStackBackTrace(0, (DWORD)putils::lengthof(stack), stack, nullptr);

		char symbol_buffer[sizeof(SYMBOL_INFO) + 256];
		auto symbol = (SYMBOL_INFO *)symbol_buffer;
		symbol->MaxNameLen = 256;
		symbol->SizeOfStruct = sizeof(SYMBOL_INFO);

		for (int i = 0; i < frames; i++) {
			SymFromAddr(process, (DWORD64)(stack[i]), 0, symbol);

			DWORD displacement;
			IMAGEHLP_LINE64 line;
			SymGetLineFromAddr64(process, (DWORD64)(stack[i]), &displacement, &line);

			static constexpr auto stack_frames_to_ignore = 1;
			if (i >= stack_frames_to_ignore + stack_frames_to_ignore) {
				const putils::string<256> s("\t {}: {} - (l.{})", frames - i - 1, symbol->Name, line.LineNumber);
				if (!ret.empty())
					ret += '\n';
				ret += s;
			}
		}
#endif
		return ret;
	}
}
