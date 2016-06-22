#pragma once

#include <Common/Filesystem.h>
#include <Renderer/Renderer.h>
#include <connorlib/serialization/toml.h>
#include <connorlib/imageload.h>
#include <vector>
#include <chrono>
#include <unordered_map>

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
    bool GetSpriteWindow(uint32_t index, SpriteWindow *window) const;
    bool FindSprite(const std::string &name, uint32_t *sprite);

protected:
    SpritePack() = default;

    RPtr<ISpriteSet> sprite_set;
    std::vector<std::string> sprite_names;
    std::vector<SpriteWindow> windows;
    std::unordered_map<std::string, size_t> name_map;
};

class GifPack : public SpritePack
{
public:
    static std::unique_ptr<GifPack> LoadGif(const fs::path &pack_folder, const TOML::Table &config_root);

    bool LoadFrame(uint32_t frame, ImageLoad::duration *duration);

private:
    IStreamingTexture *texture;
    ImageLoad::AnimatedGif gif;
};
