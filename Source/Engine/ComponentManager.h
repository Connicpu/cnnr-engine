#pragma once

#include "ComponentListImpls.h"
#include "DataComponent.h"
#include "Transform.h"
#include "ComponentList.h"
#include "ComponentId.h"

class ComponentManager
{
public:
    ComponentManager(EntityManager *entities);

    template <typename T>
    ComponentId RegisterHot(String name);

    std::optional<ComponentId> FindList(const String &name) const;

    ComponentList &operator[](const String &name);
    ComponentList &operator[](ComponentId id);

    const ComponentList &operator[](const String &name) const;
    const ComponentList &operator[](ComponentId id) const;

    HotComponentList<Transform> transform;

private:
    ComponentId InternalRegister(String name, ComponentListPtr ptr);

    EntityManager *entities_;
    HashMap<ComponentId, ComponentListPtr, Fnv1A> components_;
    HashMap<String, ComponentId, Fnv1A> ids_;
    uint32_t next_id_ = 1;
};

template<typename T>
inline ComponentId ComponentManager::RegisterHot(String name)
{
    if (auto old_id = FindList(name))
        ids_.erase(name);

    auto id = ComponentId(next_id_++);
    components_[id] = ComponentListPtr{ new HotComponentList<T>(entities_) };
    ids_.insert_or_assign(std::move(name), id);
}
