# [function](function.hpp)

Fixed-size replacement for `std::function`. Avoids dynamic allocations by holding the functor in a static, fixed-size buffer.