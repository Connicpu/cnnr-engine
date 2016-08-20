#pragma once

#include "GraphicsService.h"
#include <Common/optional.h>
#include <AssetPipeline/SpriteLoader.h>

struct ServiceManager
{
    std::optional<GraphicsService> graphics;
    std::optional<SpriteLoader> sprite_loader;

    HashMap<String, RPtr<IScene>> scenes;
    std::vector<String> scene_order;
    RPtr<ICamera> camera;

    double delta;
    bool running;
};
