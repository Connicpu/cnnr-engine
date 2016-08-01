#include "SystemManager.h"

void SystemManager::RegisterSystem(String name, SystemPtr system)
{
    execution_queue_.push_back(system.get());
    systems_.insert_or_assign(std::move(name), std::move(system));

    auto compare = [](const System *lhs, const System *rhs)
    {
        return lhs->priority < rhs->priority;
    };
    std::stable_sort(execution_queue_.begin(), execution_queue_.end(), std::move(compare));
}

void SystemManager::OnEvent(const GameData &data, const EntityEvent &event)
{
    for (auto *system : execution_queue_)
    {
        system->OnEntityEvent(data, event);
    }
}

void SystemManager::Process(GameData &data)
{
    for (auto &system : execution_queue_)
    {
        system->Process(data);
    }
}

System &SystemManager::operator[](const String &name)
{
    if (auto sys = get(name))
        return *sys;
    throw std::runtime_error{ ("There is no registered system named "_s + name).c_str() };
}

const System &SystemManager::operator[](const String &name) const
{
    if (auto sys = get(name))
        return *sys;
    throw std::runtime_error{ ("There is no registered system named "_s + name).c_str() };
}
