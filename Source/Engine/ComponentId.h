#pragma once

#include <stdint.h>

struct ComponentId
{
    ComponentId() = default;
    explicit ComponentId(uint32_t id)
        : id(id)
    {
    }

    uint32_t id;

    bool operator==(ComponentId rhs) const
    {
        return id == rhs.id;
    }

    bool operator!=(ComponentId rhs) const
    {
        return id != rhs.id;
    }
};

template <typename H>
void hash_apply(const ComponentId &id, H &h)
{
    hash_apply<H>(id.id, h);
}
