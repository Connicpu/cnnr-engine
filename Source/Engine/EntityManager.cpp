#include "pch.h"
#include "EntityManager.h"
#include "GameData.h"
#include "EntityEvent.h"

EntityManager::EntityManager()
{
}

EntityManager::~EntityManager()
{
}

Entity EntityManager::CreateEntity()
{
    uint64_t id = next_id_++;
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

    living_++;
    added_.insert(entity);
    return entity;
}

void EntityManager::RemoveEntity(Entity e)
{
    if (!is_alive(e) || removing_.find(e) != removing_.end())
        throw std::runtime_error{ "Tried to double-delete an entity" };

    added_.erase(e);
    modified_.erase(e);
    removing_.insert(e);
}

void EntityManager::OnModified(Entity e)
{
    if (!is_alive(e))
        throw std::runtime_error{ "Tried to call OnModified for a dead entity" };

    if (added_.find(e) != added_.end())
        return;
    if (removing_.find(e) != removing_.end())
        return;

    modified_.insert(e);
}

void EntityManager::SetName(Entity e, String name)
{
    names_.insert_or_assign(e, std::move(name).into_owned());
}

String EntityManager::GetName(Entity e) const
{
    auto it = names_.find(e);
    if (it != names_.end())
        return it->second.span();
    return "UNNAMED"_s;
}

void EntityManager::FlushQueue(GameData &data)
{
    for (Entity e : added_)
    {
        EntityEvent event(EntityEvent::EntityAdded, e);
        data.systems.OnEvent(data, event);
    }

    for (Entity e : modified_)
    {
        EntityEvent event(EntityEvent::EntityModified, e);
        data.systems.OnEvent(data, event);
    }

    for (Entity e : removing_)
    {
        EntityEvent event(EntityEvent::EntityRemoved, e);
        data.systems.OnEvent(data, event);
    }

    for (Entity e : removing_)
    {
        living_--;
        status(e).alive = false;
    }

    added_.clear();
    modified_.clear();
    removing_.clear();
}

EntityManager::iterator EntityManager::begin() const
{
    return iterator::make_begin(this);
}

EntityManager::iterator EntityManager::end() const
{
    return iterator::make_end(this);
}

EntityStatus &EntityManager::status(Entity e)
{
    auto index = static_cast<IndexedEntity>(e).index;
    if (index >= statuses_.size())
        statuses_.resize(size_t(index + 1));
    return statuses_[index];
}

bool EntityManager::is_alive(Entity e) const
{
    auto index = static_cast<IndexedEntity>(e).index;
    if (index >= statuses_.size())
        return false;
    return statuses_[index].alive && statuses_[index].id == e.GetId();
}

EntityManager::iterator EntityManager::iterator::make_begin(const EntityManager *manager)
{
    iterator it;
    it.manager_ = manager;
    it.index_ = 0;

    if (manager->next_index_ > 0 && !manager->statuses_[0].alive)
        ++it;

    return it;
}

EntityManager::iterator EntityManager::iterator::make_end(const EntityManager *manager)
{
    iterator it;
    it.manager_ = manager;
    it.index_ = manager->next_index_;
    return it;
}
