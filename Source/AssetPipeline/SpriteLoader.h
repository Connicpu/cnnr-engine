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

    SpriteLoader(const SpriteLoader &) = delete;
    SpriteLoader(SpriteLoader &&) = default;
    SpriteLoader &operator=(const SpriteLoader &) = delete;
    SpriteLoader &operator=(SpriteLoader &&) = default;

    SpritePack *Load(const String &pack);
    void Clear();

private:
    RPtr<IDevice> device_;
    std::unordered_map<String, std::unique_ptr<SpritePack>, StdHash<Fnv1A>> loaded_packs_;
};
