#pragma once

#include "ComponentList.h"
#include "Entity.h"
#include <Common/Hash.h>
#include <vector>

template <typename T>
class HotComponentList final : public ComponentList
{
public:
    HotComponentList(EntityManager *entities)
        : ComponentList(entities)
    {
    }

    inline void insert(const Entity &e)
    {
        ComponentList::insert(e);
    }

    inline void insert(const Entity &e, T &&component)
    {
        ComponentList::insert(e, static_cast<Component &&>(component));
    }

    inline std::optional<T &> get(const Entity &e) const
    {
        if (auto val = ComponentList::get(e))
            return static_cast<T &>(*val);
        return std::nullopt;
    }

    inline T &operator[](const Entity &e)
    {
        return static_cast<T &>(ComponentList::operator[](e));
    }

protected:
    virtual void do_insert(const Entity &e, Component &&component) override;
    virtual void do_insert(const Entity &e) override;
    virtual void do_remove(const Entity &e) override;
    virtual std::optional<Component &> do_get(const Entity &e) const override;

private:
    mutable std::vector<std::optional<T>> components_;
};

template <typename T>
class ColdComponentList final : public ComponentList
{
public:
    ColdComponentList(EntityManager *entities)
        : ComponentList(entities)
    {
    }

    inline std::optional<T &> get(const Entity &e)
    {
        if (auto val = ComponentList::get(e))
            return static_cast<T &>(*val);
        return std::nullopt;
    }

    inline T &operator[](const Entity &e)
    {
        return static_cast<T &>(ComponentList::operator[](e));
    }

protected:
    virtual void do_insert(const Entity &e, Component &&component) override;
    virtual void do_insert(const Entity &e) override;
    virtual void do_remove(const Entity &e) override;
    virtual std::optional<Component &> do_get(const Entity &e) const override;

private:
    mutable HashMap<uint32_t, T> components_;
};

template<typename T>
inline void HotComponentList<T>::do_insert(const Entity &e, Component &&component)
{
    auto index = static_cast<IndexedEntity>(e).index;
    if (index >= components_.size())
        components_.resize(size_t((index + 1) * 1.5f));

    components_[index] = std::make_optional(dynamic_cast<T &&>(component));
}

template<typename T>
inline void HotComponentList<T>::do_insert(const Entity &e)
{
    auto index = static_cast<IndexedEntity>(e).index;
    if (index >= components_.size())
        components_.resize(size_t((index + 1) * 1.5f));
    components_[index] = std::make_optional(T{});
}

template<typename T>
inline void HotComponentList<T>::do_remove(const Entity &e)
{
    auto index = static_cast<IndexedEntity>(e).index;
    if (index < components_.size())
        components_[index] = std::nullopt;
}

template<typename T>
inline std::optional<Component&> HotComponentList<T>::do_get(const Entity &e) const
{
    auto index = static_cast<IndexedEntity>(e).index;
    if (index < components_.size())
        return components_[index].template as_ref<Component>();
    return std::nullopt;
}

template<typename T>
inline void ColdComponentList<T>::do_insert(const Entity &e, Component &&component)
{
    auto index = static_cast<IndexedEntity>(e).index;
    components_.insert_or_assign(index, dynamic_cast<T &&>(component));
}

template<typename T>
inline void ColdComponentList<T>::do_insert(const Entity &e)
{
    auto index = static_cast<IndexedEntity>(e).index;
    components_.insert_or_assign(index, T{});
}

template<typename T>
inline void ColdComponentList<T>::do_remove(const Entity &e)
{
    auto index = static_cast<IndexedEntity>(e).index;
    components_.erase(index);
}

template<typename T>
inline std::optional<Component&> ColdComponentList<T>::do_get(const Entity &e) const
{
    auto index = static_cast<IndexedEntity>(e).index;
    auto it = components_.find(index);
    if (it != components_.end())
        return it->second;
    return std::nullopt;
}
