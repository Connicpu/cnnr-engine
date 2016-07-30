#include "SystemManager.h"

void SystemManager::OnEvent(const GameData &data, const EntityEvent &event)
{
    for (auto *system : execution_queue_)
    {
        system->OnEntityEvent(data, event);
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
