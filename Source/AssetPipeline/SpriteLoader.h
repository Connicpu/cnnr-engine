#pragma once

#include <Renderer/Renderer.h>
#include <memory>
#include <string>
#include <unordered_map>

class SpritePack;

class SpriteLoader
{
public:
    SpriteLoader(IDevice *device);
    ~SpriteLoader();

    SpritePack *Load(const std::string &pack);

private:
    RPtr<IDevice> device_;
    std::unordered_map<std::string, std::unique_ptr<SpritePack>> loaded_packs_;
};
