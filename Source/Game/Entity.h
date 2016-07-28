#pragma once

#include <stdint.h>

struct IndexedEntity;

class Entity
{
public:
    inline Entity() = default;
    inline Entity(uint64_t id, uint32_t index);

    inline uint64_t GetId() const;
    inline explicit operator IndexedEntity() const;

private:
    uint64_t id_;
    uint32_t index_;
};


inline Entity::Entity(uint64_t id, uint32_t index)
    : id_(id), index_(index)
{
}

inline uint64_t Entity::GetId() const
{
    return id_;
}

Entity::operator IndexedEntity() const
{
    return IndexedEntity{ index_ };
}

struct IndexedEntity
{
    uint32_t index;
};
