#include "SpritePack.h"
#include "SpriteDesc.h"

std::unique_ptr<SpritePack> SpritePack::LoadPack(
    IDevice *device, const fs::path &pack_folder, const TOML::Table &config_root
)
{
    std::vector<ImageLoad::Image> images;
    std::vector<std::string> image_names;
    std::unordered_map<std::string, uint32_t> name_map;
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

        image_names.push_back(sprite_desc.name);
        name_map.emplace(std::move(sprite_desc.name), (uint32_t)images.size());
        images.emplace_back(std::move(image));
    }

    size_t pack_buffer_size = pack_width * pack_height * 4;
    size_t pack_stride = pack_width * 4;

    std::vector<std::unique_ptr<uint8_t[]>> temp_buffers;
    std::vector<const uint8_t *> buffers;
    std::vector<SpriteWindow> windows;
    if (consistent_size)
    {
        for (auto &image : images)
        {
            auto frame = image.GetFrame();
            const uint8_t *buffer;
            frame.GetBuffer(&buffer);
            buffers.push_back(buffer);
        }
    }
    else
    {
        for (auto &image : images)
        {
            uint32_t width, height;
            image.GetSize(&width, &height);
            windows.emplace_back(width / (float)pack_width, height / (float)pack_height);
            size_t image_stride = width * 4;

            assert(pack_stride >= image_stride);

            auto frame = image.GetFrame();
            const uint8_t *orig_buffer;
            frame.GetBuffer(&orig_buffer);

            if (image_stride == pack_stride)
            {
                buffers.push_back(orig_buffer);
            }
            else
            {
                auto temp_buffer = std::make_unique<uint8_t[]>(pack_buffer_size);
                for (uint32_t y = 0; y < height; ++y)
                {
                    memcpy(&temp_buffer[y * pack_stride], &orig_buffer[y * image_stride], image_stride);
                }

                buffers.push_back(temp_buffer.get());
                temp_buffers.push_back(std::move(temp_buffer));
            }
        }
    }

    RPtr<ISpriteSet> set;
    device->CreateSpriteSet(
        false,
        (uint32_t)images.size(),
        pack_width, pack_height,
        buffers.data(),
        &set
    );

    auto pack = std::unique_ptr<SpritePack>{ new SpritePack };
    pack->sprite_set = std::move(set);
    pack->sprite_names = std::move(image_names);
    pack->windows = std::move(windows);
    pack->name_map = std::move(name_map);
    return std::move(pack);
}

ISpriteSet * SpritePack::GetSpriteSet() const
{
    return sprite_set.p;
}

uint32_t SpritePack::GetSpriteCount() const
{
    return sprite_set->GetSpriteCount();
}

ITexture *SpritePack::GetSprite(uint32_t index) const
{
    ITexture *tex;
    if (sprite_set->GetSprite(index, &tex))
        return tex;
    return nullptr;
}

bool SpritePack::GetSpriteWindow(uint32_t index, SpriteWindow *window) const
{
    if (windows.empty())
    {
        *window = { 1.0f, 1.0f };
    }
    else
    {
        if (index >= windows.size())
            return false;
        *window = windows[index];
    }
    return true;
}

bool SpritePack::FindSprite(const std::string &name, uint32_t *sprite)
{
    auto it = name_map.find(name);
    if (it == name_map.end())
        return false;

    *sprite = it->second;
    return true;
}

std::unique_ptr<GifPack> GifPack::LoadGif(const fs::path &pack_folder, const TOML::Table &config_root)
{
    return std::unique_ptr<GifPack>();
}

bool GifPack::LoadFrame(uint32_t frame, ImageLoad::duration *duration)
{
    ImageLoad::Frame frame_buf;
    if (!gif.GetFrame(frame, &frame_buf, duration))
        return false; // `frame` >= the number of frames inside the file

    uint32_t width, height;
    gif.GetSize(&width, &height);
    const uint8_t *buffer;
    frame_buf.GetBuffer(&buffer);

    // Size is required here as a sanity check. Comparing some numbers
    // is miniscule compared to copying an entire image to the GPU :P
    texture->Update(buffer, width * height * 4);

    return true; // the requested frame existed in the file!
}
