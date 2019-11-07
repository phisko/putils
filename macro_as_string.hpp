#pragma once

#define putils_macro_as_string_impl(macro) #macro
#define putils_macro_as_string(macro) putils_macro_as_string_impl(macro)