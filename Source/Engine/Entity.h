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

    inline bool operator==(const Entity &rhs) const
    {
        return id_ == rhs.id_ && index_ == rhs.index_;
    }

    inline bool operator!=(const Entity &rhs) const
    {
        return !(*this == rhs);
    }

private:
    template <typename H>
    friend void hash_apply(const Entity &e, H &h);

    uint64_t id_;
    uint32_t index_;
};

template <typename H>
void hash_apply(const Entity &e, H &h)
{
    hash_apply(e.id_, h);
    hash_apply(e.index_, h);
}

struct IndexedEntity
{
    uint32_t index;
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
