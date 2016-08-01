#pragma once

#include "Entity.h"
#include <Common/Hash.h>
#include <stdint.h>
#include <vector>

struct GameData;
struct EntityStatus;

class EntityManager
{
public:
    EntityManager();
    ~EntityManager();

    Entity CreateEntity();
    void RemoveEntity(Entity e);
    void OnModified(Entity e);

    void FlushQueue(GameData &data);

    class iterator;
    iterator begin() const;
    iterator end() const;

private:
    EntityStatus &status(Entity e);
    bool is_alive(Entity e) const;

    uint64_t next_id_ = 1;
    uint32_t next_index_ = 0;
    uint32_t living_ = 0;
    std::vector<EntityStatus> statuses_;
    std::vector<uint32_t> free_list_;
    HashSet<Entity> added_, modified_, removing_;
};

class EntityManager::iterator
{
public:
    static iterator make_begin(const EntityManager *manager);
    static iterator make_end(const EntityManager *manager);

    inline iterator &operator++();
    inline iterator operator++(int);
    inline Entity operator*();
    inline bool operator==(const iterator &rhs) const;
    inline bool operator!=(const iterator &rhs) const;

private:
    iterator() = default;
    const EntityManager *manager_;
    uint32_t index_;
};

struct EntityStatus
{
    EntityStatus() = default;
    EntityStatus(bool alive, uint64_t id)
        : alive(alive), id(id)
    {
    }

    bool alive = false;
    uint64_t id;
};

inline EntityManager::iterator &EntityManager::iterator::operator++()
{
    do
    {
        index_++;
        if (manager_->statuses_[index_].alive)
            break;
    }
    while (index_ < manager_->next_index_);

    return *this;
}

inline EntityManager::iterator EntityManager::iterator::operator++(int)
{
    iterator copy(*this);
    ++(*this);
    return copy;
}

inline Entity EntityManager::iterator::operator*()
{
    return Entity(manager_->statuses_[index_].id, index_);
}

inline bool EntityManager::iterator::operator==(const iterator &rhs) const
{
    return manager_ == rhs.manager_ && index_ == rhs.index_;
}

inline bool EntityManager::iterator::operator!=(const iterator &rhs) const
{
    return !operator==(rhs);
}
