#pragma once

#include <type_traits>
#include <mutex>

namespace putils {
    // Lock a mutex, execute a functor, unlock mutex
    template<typename Mutex, typename Func, typename ...Args>
    std::result_of_t<Func(Args...)> lock_and_run(Mutex & m, Func && f, Args && ...args) {
        std::lock_guard<Mutex> l(m);
        return f(std::forward<Args>(args)...);
    }
}