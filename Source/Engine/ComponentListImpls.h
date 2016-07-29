#pragma once

#include "ComponentList.h"
#include "Entity.h"
#include <Common/Hash.h>
#include <vector>

template <typename T>
class HotComponentList final : public ComponentList
{
public:
    virtual void insert(const Entity &e, Component &&component) override;
    virtual void remove(const Entity &e) override;
    virtual std::optional<Component &> get(const Entity &e) override;

private:
    std::vector<std::optional<T>> components_;
};

template <typename T>
class ColdComponentList final : public ComponentList
{
public:
    virtual void insert(const Entity &e, Component &&component) override;
    virtual void remove(const Entity &e) override;
    virtual std::optional<Component &> get(const Entity &e) override;

private:
    HashMap<uint32_t, T> components_;
};

template<typename T>
inline void HotComponentList<T>::insert(const Entity &e, Component &&component)
{
    auto index = static_cast<IndexedEntity>(e).index;
    if (index >= components_.size())
        components_.resize(size_t((index + 1) * 1.5f));

    components_[index] = std::make_optional(dynamic_cast<T &&>(component));
}

template<typename T>
inline void HotComponentList<T>::remove(const Entity &e)
{
    auto index = static_cast<IndexedEntity>(e).index;
    if (index < components_.size())
        components_[index] = std::nullopt;
}

template<typename T>
inline std::optional<Component&> HotComponentList<T>::get(const Entity &e)
{
    auto index = static_cast<IndexedEntity>(e).index;
    if (index < components_.size())
        return components_[index];
    return std::nullopt;
}

template<typename T>
inline void ColdComponentList<T>::insert(const Entity &e, Component &&component)
{
    using namespace std;
    auto index = static_cast<IndexedEntity>(e).index;
    components_.insert_or_assign(index, dynamic_cast<T &&>(component)));
}

template<typename T>
inline void ColdComponentList<T>::remove(const Entity & e)
{
}

template<typename T>
inline std::optional<Component&> ColdComponentList<T>::get(const Entity & e)
{
    return std::optional<Component&>();
}
