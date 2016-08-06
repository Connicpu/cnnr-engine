#pragma once

#include <Common/optional.h>
#include <Common/MaybeOwned.h>
#include <memory>

class Component;
class Entity;
class EntityManager;

class ComponentList
{
public:
    ComponentList(EntityManager *entities);
    inline virtual ~ComponentList() {}

    void insert(const Entity &e, Component &&component);
    void insert(const Entity &e);
    void remove(const Entity &e);
    std::optional<Component &> get(const Entity &e) const;

    Component &operator[](const Entity &e) const;

protected:
    virtual void do_insert(const Entity &e, Component &&component) = 0;
    virtual void do_insert(const Entity &e) = 0;
    virtual void do_remove(const Entity &e) = 0;
    virtual std::optional<Component &> do_get(const Entity &e) const = 0;

private:
    EntityManager *entities_;
};

using ComponentListPtr = MaybeOwned<ComponentList>;
