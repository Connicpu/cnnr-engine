#include "EntityManager.h"

struct EntityStatus
{
    bool alive = false;
    uint64_t id;
};

EntityManager::EntityManager()
    : next_id_(1), next_index_(0)
{
}

EntityManager::~EntityManager()
{
}

Entity EntityManager::CreateEntity()
{
    uint32_t id = next_id_++;
    uint32_t index;
    if (free_list_.empty())
    {
        index = next_index_++;
    }
    else
    {
        index = free_list_.back();
        free_list_.pop_back();
    }

    Entity entity{ id, index };
    status(entity) = { true, id };

    added_.insert(entity);
    return entity;
}

void EntityManager::RemoveEntity(Entity e)
{
    if (!is_alive(e))
        throw std::runtime_error{ "Tried to double-delete an entity" };

    added_.erase(e);
    modified_.erase(e);
    removed_.insert(e);
    free_list_.push_back(static_cast<IndexedEntity>(e).index);
    status(e).alive = false;
}

EntityStatus &EntityManager::status(Entity e)
{
    auto index = static_cast<IndexedEntity>(e).index;
    if (index >= statuses_.size())
        statuses_.resize(size_t((index + 1) * 1.5f));
    return statuses_[index];
}

bool EntityManager::is_alive(Entity e) const
{
    auto index = static_cast<IndexedEntity>(e).index;
    if (index >= statuses_.size())
        return false;
    return statuses_[index].alive && statuses_[index].id == e.GetId();
}
