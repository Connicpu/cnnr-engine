#pragma once

#include <Common/String.h>
#include <Common/optional.h>
#include "System.h"

struct GameData;
struct EntityEvent;

class SystemManager
{
public:
    void OnEvent(const GameData &data, const EntityEvent &event);

    template <typename T = System>
    std::optional<T&> get(const String &name);
    template <typename T = System>
    std::optional<T const&> get(const String &name) const;

    System &operator[](const String &name);
    const System &operator[](const String &name) const;

private:
    HashMap<String, std::pair<SystemPtr, int>> systems_;
    std::vector<System *> execution_queue_;
};

template<typename T>
inline std::optional<T&> SystemManager::get(const String &name)
{
    auto it = systems_.find(name);
    if (it != systems_.end())
        return dynamic_cast<T&>(*it->second.first);
    return std::nullopt;
}

template<typename T>
inline std::optional<T const&> SystemManager::get(const String &name) const
{
    auto it = systems_.find(name);
    if (it != systems_.end())
        return dynamic_cast<T const&>(*it->second.first);
    return std::nullopt;
}
