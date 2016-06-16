#include "SpriteLoader.h"
#include "SpritePack.h"
#include "MMap.h"

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

    fs::path desc_path = fs::current_path() / "Assets" / "Sprites" / pack / "Pack.toml";

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
    if (pack_type == SpritePackType::Sprites)
    {
        for (auto &sprite_desc : config_root["Sprite"]->GetArray())
        {
            // todo
        }
    }

    return nullptr;
}
