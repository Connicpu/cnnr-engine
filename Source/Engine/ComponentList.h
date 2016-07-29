#pragma once

#include <Common/optional.h>

class Component;
class Entity;

class ComponentList
{
public:
    virtual void insert(const Entity &e, Component &&component) = 0;
    virtual void remove(const Entity &e) = 0;
    virtual std::optional<Component &> get(const Entity &e) = 0;

    inline Component &operator[](const Entity &e)
    {
        if (auto c = get(e))
            return *c;
        throw std::runtime_error{ "Attempted to index a ComponentList where the requested component did not exist" };
    }
};
