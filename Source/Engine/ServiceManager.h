#pragma once

#include "GraphicsService.h"
#include <Common/optional.h>

struct ServiceManager
{
    bool running;
    std::optional<GraphicsService> graphics;
};
