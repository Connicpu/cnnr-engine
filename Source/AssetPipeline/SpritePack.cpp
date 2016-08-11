#include "SpritePack.h"
#include "SpriteDesc.h"

std::unique_ptr<SpritePack> SpritePack::LoadPack(
    IDevice *device, const fs::path &pack_folder, const TOML::Table &config_root
)
{
    std::vector<ImageLoad::Image> images;
    std::vector<String> image_names;
    std::unordered_map<String, uint32_t, StdHash<Fnv1A>> name_map;
    bool has_size = false, consistent_size = true;
    uint32_t pack_width, pack_height;

    bool pixel_art = false;
    auto &pack_desc = config_root["Pack"]->GetTable();
    if (auto pixel = pack_desc["PixelArt"])
        pixel_art = pixel->IsBool() && pixel->GetBool();

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

        name_map.emplace(std::string(sprite_desc.name), (uint32_t)images.size());
        image_names.push_back(std::move(sprite_desc.name));
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

    SpriteSetParams params;
    params.streaming = false;
    params.sprite_count = (uint32_t)images.size();
    params.sprite_width = pack_width;
    params.sprite_height = pack_height;
    params.buffers = buffers.data();
    params.pixel_art = pixel_art;
    RPtr<ISpriteSet> set;
    device->CreateSpriteSet(&params, &set);

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

std::optional<ITexture*> SpritePack::GetSprite(const String &name) const
{
    if (auto id = FindSprite(name))
        return GetSprite(*id);
    return{};
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

std::optional<uint32_t> SpritePack::FindSprite(const String &name) const
{
    auto it = name_map.find(name);
    if (it == name_map.end())
        return std::nullopt;
    return it->second;
}

std::unique_ptr<GifPack> GifPack::LoadGif(
    IDevice *device, const fs::path &pack_folder, const TOML::Table &config_root
)
{
    auto &gif_config = config_root["Gif"]->GetTable();

    SpriteDesc sprite_desc;
    sprite_desc.Deserialize(gif_config, pack_folder);
    auto gif = ImageLoad::AnimatedGif::Load(sprite_desc.GetImageId());
    
    ImageLoad::Frame frame0;
    ImageLoad::duration dur0;
    if (!gif.GetFrame(0, &frame0, &dur0))
        return nullptr;

    uint32_t width, height;
    gif.GetSize(&width, &height);
    const uint8_t *buffer;
    frame0.GetBuffer(&buffer);

    SpriteSetParams params;
    params.streaming = true;
    params.sprite_count = 1;
    params.sprite_width = width;
    params.sprite_height = height;
    params.buffers = &buffer;
    RPtr<ISpriteSet> set;
    device->CreateSpriteSet(&params, &set);

    auto pack = std::unique_ptr<GifPack>{ new GifPack(std::move(gif)) };
    pack->sprite_set = std::move(set);
    pack->sprite_names.push_back(std::string(sprite_desc.name));
    pack->name_map.emplace(std::move(sprite_desc.name), 0);
    pack->sprite_set->GetStreamingSprite(0, &pack->texture);
    return std::move(pack);
}

GifPack::~GifPack()
{
    if (cache_future.valid())
        cache_future.get();
}

bool GifPack::LoadFrame(uint32_t frame, ImageLoad::duration *duration)
{
    bool result = true;
    if (cache_future.valid())
    {
        if (!cache_future.get())
        {
            frame = 0;
            result = false;
        }
    }
    return InternalLoadFrame(frame, duration) && result;
}

void GifPack::CacheNextThreaded(uint32_t frame)
{
    cache_future = std::async([this, frame]() -> bool
    {
        ImageLoad::duration dur;
        if (!this->InternalLoadFrame(frame, &dur))
        {
            this->InternalLoadFrame(0, &dur);
            return false;
        }
        return true;
    });
}

bool GifPack::InternalLoadFrame(uint32_t frame, ImageLoad::duration *duration)
{
    auto device = texture->GetDevice();
    device->Lock();

    ImageLoad::Frame frame_buf;
    if (!gif.GetFrame(frame, &frame_buf, duration))
    {
        device->Unlock();
        return false; // `frame` >= the number of frames inside the file
    }

    uint32_t width, height;
    gif.GetSize(&width, &height);
    const uint8_t *buffer;
    frame_buf.GetBuffer(&buffer);

    // Size is required here as a sanity check. Comparing some numbers
    // is miniscule compared to copying an entire image to the GPU :P

    texture->Update(buffer, width * height * 4);

    device->Unlock();
    return true; // the requested frame existed in the file!
}
