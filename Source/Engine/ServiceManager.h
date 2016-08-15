#pragma once

#include "GraphicsService.h"
#include <Common/optional.h>
#include <AssetPipeline/SpriteLoader.h>

struct ServiceManager
{
    bool running;
    std::optional<GraphicsService> graphics;
    std::optional<SpriteLoader> sprite_loader;
};
