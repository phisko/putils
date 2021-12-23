#pragma once

#include "Attributes.hpp"

namespace putils::reflection {
    template<typename T>
    const Attributes & getRuntimeAttributes() noexcept;
}

#include "attributes_helper.inl"
