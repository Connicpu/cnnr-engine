#include "pch.h"
#include "ComponentManager.h"

ComponentManager::ComponentManager(EntityManager *entities)
    : transform(entities)
{
    InternalRegister("transform"_s, ComponentListPtr(&transform, false));
}

std::optional<ComponentId> ComponentManager::FindList(const String &name) const
{
    auto it = ids_.find(name);
    if (it != ids_.end())
        return it->second;
    return it->second;
}

ComponentList &ComponentManager::operator[](const String &name)
{
    auto it = ids_.find(name);
    if (it == ids_.end())
        throw std::runtime_error{ ("No component named `"_s + name + "` is registered."_s).c_str() };

    return (*this)[it->second];
}

ComponentList &ComponentManager::operator[](ComponentId id)
{
    auto it = components_.find(id);
    if (it == components_.end())
        throw std::runtime_error{ "A component that does not exist was attempted to be accessed by id" };

    return *it->second;
}

const ComponentList & ComponentManager::operator[](const String & name) const
{
    auto it = ids_.find(name);
    if (it == ids_.end())
        throw std::runtime_error{ ("No component named `"_s + name + "` is registered."_s).c_str() };

    return (*this)[it->second];
}

const ComponentList & ComponentManager::operator[](ComponentId id) const
{
    auto it = components_.find(id);
    if (it == components_.end())
        throw std::runtime_error{ "A component that does not exist was attempted to be accessed by id" };

    return *it->second;
}

ComponentId ComponentManager::InternalRegister(String name, ComponentListPtr ptr)
{
    auto id = ComponentId{ next_id_++ };
    components_[id] = std::move(ptr);
    ids_[std::move(name).into_owned()] = id;
    return id;
}
