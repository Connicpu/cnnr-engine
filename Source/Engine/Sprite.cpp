#include "pch.h"
#include "Sprite.h"
#include "GameData.h"

Sprite::Sprite()
    : handle_(nullptr)
{
}

Sprite::~Sprite()
{
    if (handle_)
    {
        scene_->DestroySprite(handle_);
    }
}

void Sprite::Initialize(IScene *scene, SpritePack *pack, const SpriteObjectParams &params)
{
    scene_ = scene;
    pack_ = pack;
    handle_ = scene->CreateSprite(&params);
}

bool Sprite::ChangeTexture(const String &name)
{
    if (auto idx = pack_->FindSprite(name))
        return ChangeTexture(*idx);
    return false;
}

bool Sprite::ChangeTexture(uint32_t index)
{
    if (auto tex = pack_->GetSprite(index))
    {
        scene_->SetTexture(handle_, tex);
        return true;
    }
    return false;
}

bool Sprite::ChangeTexture(SpritePack *pack, uint32_t index)
{
    auto old_pack = pack_;
    pack_ = pack;
    if (!ChangeTexture(index))
    {
        pack_ = old_pack;
        return false;
    }

    if (auto gp = dynamic_cast<GifPack *>(pack_))
    {
        gif_ = GifState{ 0, gp->CacheNextThreaded(0) };
    }
    else
    {
        gif_ = std::nullopt;
    }

    return true;
}

void Sprite::SetAnimation(const std::optional<SpriteAnimation> &animation)
{
    anim_ = animation;
}

void Sprite::Update(const GameData &data, const Math::Matrix3x2F &transform)
{
    if (gif_)
    {
        // Do gif stuff
    }
    else if (anim_)
    {
        // Do animation stuff
    }

    scene_->SetTransform(handle_, &transform);
}

GifState::GifState(uint32_t frame, GifFuture load)
    : current_frame(frame), load_future(std::move(load)), frame_dur(0)
{

}
