#pragma once

#include <Common/Filesystem.h>
#include <connorlib/serialization/toml.h>
#include <connorlib/imageload.h>

struct SpriteDesc
{
    fs::path path;
    std::string name;
    std::string format;
    float size;

    void Deserialize(const TOML::Table &sprite_desc, const fs::path &pack_folder);

    ImageLoad::ImageId GetImageId();
    ImageLoad::Image GetImage(const ImageLoad::ImageId &id);
};
