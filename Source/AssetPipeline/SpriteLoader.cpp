#include "SpriteLoader.h"
#include "SpritePack.h"
#include "MMap.h"
#include  "SpriteDesc.h"

#include <connorlib/serialization/toml.h>
#include <connorlib/imageload.h>

SpriteLoader::SpriteLoader(IDevice *device)
    : device_(device)
{
}

SpriteLoader::~SpriteLoader()
{
}

SpritePack *SpriteLoader::Load(const std::string &pack)
{
    auto loaded_it = loaded_packs_.find(pack);
    if (loaded_it != loaded_packs_.end())
        return loaded_it->second.get();

    fs::path pack_folder = fs::current_path() / "Assets" / "Sprites" / pack;
    fs::path desc_path = pack_folder / "Pack.toml";

    MMap mmap;
    if (!mmap.Open(desc_path))
    {
        auto msg = "Failed to open pack `" + pack +
            "` (" + desc_path.generic_string() + ")";
        throw std::runtime_error{ msg };
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
        throw std::runtime_error{ "Pack `" + pack + "` has an invalid Pack.Type" };
    
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
