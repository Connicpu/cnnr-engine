#pragma once

#include <Common/optional.h>

class Component;
class Entity;
class EntityManager;

class ComponentList
{
public:
    ComponentList(EntityManager *manager);

    void insert(const Entity &e, Component &&component);
    void remove(const Entity &e);
    std::optional<Component &> get(const Entity &e);

    inline Component &operator[](const Entity &e);

protected:
    virtual void do_insert(const Entity &e, Component &&component) = 0;
    virtual void do_remove(const Entity &e) = 0;
    virtual std::optional<Component &> do_get(const Entity &e) = 0;

private:
    EntityManager *manager_;
};
