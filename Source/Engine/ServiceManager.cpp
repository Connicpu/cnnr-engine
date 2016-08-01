#include "ServiceManager.h"

void ServiceManager::AddNamed(const String &name, std::shared_ptr<void> item)
{
    named_services_.insert_or_assign(name, std::move(item));
}
