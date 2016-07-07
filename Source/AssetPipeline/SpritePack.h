#pragma once

#include <Common/Filesystem.h>
#include <Common/optional.h>
#include <Common/String.h>
#include <Common/Hash.h>
#include <Renderer/Renderer.h>
#include <connorlib/serialization/toml.h>
#include <connorlib/imageload.h>
#include <vector>
#include <chrono>
#include <unordered_map>
#include <future>

enum class SpritePackType
{
    Sprites,
    AnimatedGif,
};

struct SpriteWindow
{
    SpriteWindow(float u, float v) : u_width(u), v_height(v) {}
    float u_width, v_height;
};

class SpritePack
{
public:
    static std::unique_ptr<SpritePack> LoadPack(
        IDevice *device, const fs::path &pack_folder, const TOML::Table &config_root
    );

    ISpriteSet *GetSpriteSet() const;
    uint32_t GetSpriteCount() const;
    ITexture *GetSprite(uint32_t index) const;
    std::optional<ITexture *> GetSprite(const String &name) const;
    bool GetSpriteWindow(uint32_t index, SpriteWindow *window) const;
    std::optional<uint32_t> FindSprite(const String &name) const;

protected:
    SpritePack() = default;

    RPtr<ISpriteSet> sprite_set;
    std::vector<String> sprite_names;
    std::vector<SpriteWindow> windows;
    std::unordered_map<String, uint32_t, StdHash<Fnv1A>> name_map;
};

class GifPack : public SpritePack
{
public:
    static std::unique_ptr<GifPack> LoadGif(
        IDevice *device, const fs::path &pack_folder, const TOML::Table &config_root
    );

    bool LoadFrame(uint32_t frame, ImageLoad::duration *duration);
    void CacheNextThreaded(uint32_t frame);

protected:
    GifPack(ImageLoad::AnimatedGif &&gif) : gif(std::move(gif)) {}
    bool InternalLoadFrame(uint32_t frame, ImageLoad::duration *duration);

    std::future<bool> cache_future;
    IStreamingTexture *texture;
    ImageLoad::AnimatedGif gif;
};
