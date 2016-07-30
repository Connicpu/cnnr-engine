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

private:
    EntityStatus &status(Entity e);
    bool is_alive(Entity e) const;

    uint64_t next_id_;
    uint32_t next_index_;
    std::vector<EntityStatus> statuses_;
    std::vector<uint32_t> free_list_;
    HashSet<Entity> added_, modified_, removing_;
};
