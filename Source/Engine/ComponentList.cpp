#include "ComponentList.h"

ComponentList::ComponentList(EntityManager *manager)
    : manager_(manager)
{
}

void ComponentList::insert(const Entity &e, Component &&component)
{
    do_insert(e, std::move(component));
}

void ComponentList::remove(const Entity &e)
{
    do_remove(e);
}

std::optional<Component&> ComponentList::get(const Entity &e)
{
    return do_get(e);
}

inline Component &ComponentList::operator[](const Entity &e)
{
    if (auto c = get(e))
        return *c;
    throw std::runtime_error{ "Attempted to index a ComponentList where the requested component did not exist" };
}
