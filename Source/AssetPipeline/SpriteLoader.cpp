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
    if (pack_type == SpritePackType::Sprites)
    {
		std::vector<ImageLoad::Image> images;
		bool has_size = false, consistent_size = true;
		uint32_t pack_width, pack_height;

        for (auto &sprite_data : config_root["Sprite"]->GetArray())
        {
			SpriteDesc sprite_desc;
			sprite_desc.Deserialize(sprite_data.GetTable(), pack_folder);

			auto img_id = sprite_desc.GetImageId();
			auto image = sprite_desc.GetImage(img_id);

			// Consistent size is a lot easier, but sometimes packs need to contain "assorted" sprites.
			// I'll work out how to support that :P
			uint32_t width, height;
			image.GetSize(&width, &height);
			if (has_size)
			{
				consistent_size = consistent_size && width == pack_width && height == pack_height;
				pack_width = (std::max)(width, pack_width);
				pack_height = (std::max)(height, pack_height);
			}
			else
			{
				pack_width = width;
				pack_height = height;
				has_size = true;
			}

			images.emplace_back(std::move(image));
        }
    }

    return nullptr;
}
