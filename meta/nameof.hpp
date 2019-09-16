#pragma once

#include <string_view>

#define pmeta_nameof(name) #name
#define pmeta_nameof_private(name) std::string_view(#name).substr(1)
