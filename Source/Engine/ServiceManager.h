#pragma once

#include <memory>
#include <unordered_map>
#include <typeindex>
#include <Common/String.h>
#include <Common/optional.h>

class ServiceManager
{
public:
    template <typename T>
    void AddTyped(std::optional<const String&> name, std::shared_ptr<void> item);
    void AddNamed(const String &name, std::shared_ptr<void> item);

    template <typename T>
    T &get() const;

    template <typename T>
    T &get(const std::type_info &type) const;

    template <typename T>
    T &get(const String &name) const;

private:
    std::unordered_map<std::type_index, std::shared_ptr<void>> typed_services_;
    HashMap<String, std::shared_ptr<void>> named_services_;
};

template<typename T>
inline void ServiceManager::AddTyped(std::optional<const String&> name, std::shared_ptr<void> item)
{
    typed_services_.insert_or_assign(typeid(T), item);
}

template<typename T>
inline T &ServiceManager::get() const
{
    return get(typeid(T));
}

template<typename T>
inline T &ServiceManager::get(const std::type_info &type) const
{
    auto it = typed_services_.find(type);
    if (it == typed_services_.end())
        throw std::runtime_error{ ("Could not find a typed service for "_s + type.name()).c_str() };
    return static_cast<T &>(*it->second);
}

template<typename T>
inline T &ServiceManager::get(const String &name) const
{
    auto it = named_services_.find(name);
    if (it == named_services_.end())
        throw std::runtime_error{ ("Could not find a named service "_s + name).c_str() };
    return static_cast<T &>(*it->second);
}
