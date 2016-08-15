#pragma once

#include "Component.h"
#include <Renderer/Renderer.h>
#include <AssetPipeline/SpritePack.h>

struct SpriteAnimation;
struct GameData;

class Sprite
{
public:
    Sprite(IScene *scene, SpritePack *pack, uint32_t texture);

    void ChangeTexture(const String &name);
    void ChangeTexture(uint32_t index);
    void ChangeTexture(SpritePack *pack, uint32_t index);
    void SetAnimation(const std::optional<SpriteAnimation> &animation);

    void Update(const GameData &data);

private:
    RPtr<IScene> scene_;
    SpriteHandle handle_;
    SpritePack *pack_;
    std::optional<uint32_t> gif_;
};

struct SpriteAnimation
{
    uint32_t start;
    uint32_t end;
    uint32_t current;
    float frametime;
};
