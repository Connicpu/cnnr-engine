#include "SpriteLoader.h"
#include "SpritePack.h"
#include "SpriteDesc.h"

#include <Common/MMap.h>
#include <connorlib/serialization/toml.h>
#include <connorlib/imageload.h>

SpriteLoader::SpriteLoader(IDevice *device)
    : device_(device)
{
}

SpriteLoader::~SpriteLoader()
{
}

SpritePack *SpriteLoader::Load(const String &pack)
{
    auto loaded_it = loaded_packs_.find(pack);
    if (loaded_it != loaded_packs_.end())
        return loaded_it->second.get();

    fs::path pack_folder = fs::current_path() / "Assets"_s / "Sprites"_s / pack;
    fs::path desc_path = pack_folder / "Pack.toml"_s;

    MMap mmap;
    if (!mmap.Open(desc_path))
    {
        auto msg = "Failed to open pack `"_s + pack +
            "` ("_s + desc_path.generic_string() + ")"_s;
        throw std::runtime_error{ msg.as_owned().c_str() };
    }

    auto config = TOML::Value::Parse((const char *)mmap.GetMemory(), mmap.GetLength());
    auto &config_root = config->GetTable();

    // Read the Pack info
    auto &pack_desc = config_root["Pack"]->GetTable();
    auto pack_type_str = pack_desc["Type"]->GetString();
    SpritePackType pack_type;
    if (pack_type_str == "Sprites")
        pack_type = SpritePackType::Sprites;
    else if (pack_type_str == "AnimatedGif")
        pack_type = SpritePackType::AnimatedGif;
    else
        throw std::runtime_error{ ("Pack `"_s + pack + "` has an invalid Pack.Type"_s).as_owned().c_str() };
    
    // Read in the sprite list / animated gif
    std::unique_ptr<SpritePack> pack_ptr;
    if (pack_type == SpritePackType::Sprites)
    {
        pack_ptr = SpritePack::LoadPack(device_.p, pack_folder, config_root);
    }
    else if (pack_type == SpritePackType::AnimatedGif)
    {
        pack_ptr = GifPack::LoadGif(device_.p, pack_folder, config_root);
    }

    auto ppack = pack_ptr.get();
    loaded_packs_[pack] = std::move(pack_ptr);

    return ppack;
}

void SpriteLoader::Clear()
{
    loaded_packs_.clear();
}
