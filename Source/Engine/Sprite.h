#pragma once

#include "Component.h"
#include <Renderer/Renderer.h>
#include <AssetPipeline/SpritePack.h>

struct GameData;

struct SpriteAnimation
{
    uint32_t start;
    uint32_t end;
    uint32_t current;
    float frametime;
};

struct GifState
{
    GifState(uint32_t frame, GifFuture load);

    uint32_t current_frame;
    GifFuture load_future;
    ImageLoad::duration frame_dur;
};

class Sprite final : public Component
{
public:
    Sprite();
    ~Sprite();

    Sprite(const Sprite &) = delete;
    Sprite(Sprite &&) = default;
    Sprite &operator=(const Sprite &) = delete;
    Sprite &operator=(Sprite &&) = default;

    void Initialize(IScene *scene, SpritePack *pack, const SpriteObjectParams &params);

    bool ChangeTexture(const String &name);
    bool ChangeTexture(uint32_t index);
    bool ChangeTexture(SpritePack *pack, uint32_t index);
    void SetAnimation(const std::optional<SpriteAnimation> &animation);

    void Update(const GameData &data, const Math::Matrix3x2F &transform);

private:
    RPtr<IScene> scene_;
    SpriteHandle handle_;
    SpritePack *pack_;

    std::optional<SpriteAnimation> anim_;
    std::optional<GifState> gif_;
};
