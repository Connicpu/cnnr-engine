#include "ComponentList.h"
#include "EntityManager.h"

ComponentList::ComponentList(EntityManager *entities)
    : entities_(entities)
{
}

void ComponentList::insert(const Entity &e, Component &&component)
{
    do_insert(e, std::move(component));
    entities_->OnModified(e);
}

void ComponentList::remove(const Entity &e)
{
    do_remove(e);
    entities_->OnModified(e);
}

std::optional<Component&> ComponentList::get(const Entity &e) const
{
    return do_get(e);
}

inline Component &ComponentList::operator[](const Entity &e) const
{
    if (auto c = get(e))
        return *c;
    throw std::runtime_error{ "Attempted to index a ComponentList where the requested component did not exist" };
}
