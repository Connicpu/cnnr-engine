#include "SpriteDesc.h"
#include <algorithm>

void SpriteDesc::Deserialize(const TOML::Table &sprite_desc, const fs::path &pack_folder)
{
    // Get the filename of the sprite image
    path = pack_folder / sprite_desc["Path"]->GetString();

    // Get a friendly name for the sprite
    if (auto name_val = sprite_desc["Name"])
        name = name_val->GetString();
    else
        name = path.filename().replace_extension("").generic_string();

    // Get the requested size of the image when rendered
    if (auto size_val = sprite_desc["Size"])
        size = (float)size_val->GetF64();
    else
        size = 1.0f;

    if (auto format_val = sprite_desc["Format"])
        format = format_val->GetString();
    else
    {
        format = path.extension().generic_string();
        format.erase(format.begin());
        std::transform(
            format.begin(), format.end(), format.begin(),
            [](char c) { return std::toupper(c, std::locale{ "en-US" }); }
        );
    }
}

ImageLoad::ImageId SpriteDesc::GetImageId()
{
    auto path_str = path.generic_string();
    return ImageLoad::ImageId::Path(path_str.c_str());
}

ImageLoad::Image SpriteDesc::GetImage(const ImageLoad::ImageId &id)
{
    if (format == "PNG")
    {
        return ImageLoad::Image::LoadPng(id);
    }

    throw std::runtime_error{ "Unknown image format `" + format + "`" };
}
