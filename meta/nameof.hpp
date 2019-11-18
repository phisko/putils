#pragma once

#include <string_view>

#define putils_nameof(name) #name
#define putils_nameof_private(name) std::string_view(#name).substr(1)
