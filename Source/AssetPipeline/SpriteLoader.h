#pragma once

#include <Renderer/Renderer.h>
#include <Common/String.h>
#include <Common/Hash.h>
#include <memory>
#include <string>
#include <unordered_map>

class SpritePack;

class SpriteLoader
{
public:
    SpriteLoader(IDevice *device);
    ~SpriteLoader();

    SpritePack *Load(const String &pack);

private:
    RPtr<IDevice> device_;
    std::unordered_map<String, std::unique_ptr<SpritePack>, StdHash<Fnv1A>> loaded_packs_;
};
