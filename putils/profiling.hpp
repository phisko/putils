#pragma once

#ifdef PUTILS_PROFILING
#include <tracy/Tracy.hpp>
#define PUTILS_PROFILING_SCOPE ZoneScoped
#define PUTILS_PROFILING_FRAME FrameMark
#else
#define PUTILS_PROFILING_SCOPE
#define PUTILS_PROFILING_FRAME
#endif