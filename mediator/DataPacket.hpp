#pragma once

#include "meta/type.hpp"

namespace putils
{
    struct ADataPacket
    {
        virtual ~ADataPacket() {}

        ADataPacket(putils::meta::type_index type) : type(type) {}

        putils::meta::type_index type;
    };

    template<typename T>
    struct DataPacket : ADataPacket
    {
        DataPacket(const T &data)
                : ADataPacket(putils::meta::type<T>::index), data(data)
        {}

        const T &data;
    };
}
