#pragma once

namespace putils {
    template<typename T>
    int sign(T a) { return a < 0 ? -1 : a > 0 ? 1 : 0; }
}