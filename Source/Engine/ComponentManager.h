#pragma once

#include "ComponentListImpls.h"
#include "DataComponent.h"
#include "Transform.h"
#include "ComponentList.h"

class ComponentManager
{
public:
    ComponentManager(EntityManager *entities);

    template <typename T>
    ComponentId RegisterHot(const String &name);

    ComponentList &operator[](const String &name);
    ComponentList &operator[](ComponentId id);

    HotComponentList<DataComponent<Transform>> transform;

private:
    ComponentId InternalRegister(String name, ComponentListPtr ptr);

    HashMap<ComponentId, ComponentListPtr, Fnv1A> components_;
    HashMap<String, ComponentId, Fnv1A> ids_;
    uint32_t next_id_ = 1;
};
