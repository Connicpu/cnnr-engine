#include "DxScene.h"

using namespace Math;

DxScene::DxScene(DxInstance *inst, Size2F segment_size)
    : ImplRenderBase<IScene, DxInstance>(inst), segment_size(segment_size)
{
}

SpriteHandle DxScene::CreateSprite(const SpriteObjectParams *params)
{
    SpriteSection section;
    if (params->is_translucent)
        section = SpriteSection::Translucent;
    else if (params->is_static)
        section = SpriteSection::SolidStatic;
    else
        section = SpriteSection::SolidDynamic;

    auto allocation = object_allocator.Alloc();
    SpriteHandle sprite = (SpriteHandle)allocation.memory;
    sprite->transform = params->transform;
    sprite->texture = static_cast<SpriteEntry *>(params->texture);
    sprite->layer = params->layer;
    sprite->window = params->window;
    sprite->tint = params->tint;
    sprite->section = section;
    sprite->current_coord = sprite->CalculateCoord(segment_size);
    sprite->allocation = allocation;

    auto &segment = segments[sprite->current_coord];
    if (!segment)
    {
        segment = std::make_unique<SceneSegment>();
    }

    auto sprite_set = sprite->texture->owner;
    switch (section)
    {
        case SpriteSection::SolidDynamic:
        {
            auto &group = segment->solid_sprites.sprites[sprite_set];
            auto &batch = segment->solid_sprites.batches[sprite_set];

            group.insert(sprite);

            if (!batch)
            {
                batch = std::make_unique<SpriteBatch>();
                batch->set = sprite_set;
            }
            batch->dirty = true;
            sprite->dirty_flag = &batch->dirty;
            break;
        }

        case SpriteSection::SolidStatic:
        {
            auto &group = segment->solid_statics.sprites[sprite_set];
            auto &batch = segment->solid_statics.batches[sprite_set];

            group.insert(sprite);

            if (!batch)
            {
                batch = std::make_unique<SpriteBatch>();
                batch->set = sprite_set;
            }
            batch->dirty = true;
            sprite->dirty_flag = &batch->dirty;
            break;
        }

        case SpriteSection::Translucent:
        {
            segment->translucents.sprites.push_back(sprite);
            segment->translucents.dirty = true;
            segment->translucents.needs_sorting = true;
            sprite->dirty_flag = &segment->translucents.dirty;
            sprite->sort_flag = &segment->translucents.needs_sorting;
            break;
        }
    }

    return sprite;
}

void DxScene::DestroySprite(SpriteHandle sprite)
{
    auto &segment = segments[sprite->current_coord];
    auto sprite_set = sprite->texture->owner;
    switch (sprite->section)
    {
        case SpriteSection::SolidDynamic:
        {
            auto &group = segment->solid_sprites.sprites[sprite_set];

            group.erase(sprite);
            *sprite->dirty_flag = true;
            break;
        }

        case SpriteSection::SolidStatic:
        {
            auto &group = segment->solid_statics.sprites[sprite_set];

            group.erase(sprite);
            *sprite->dirty_flag = true;
            break;
        }

        case SpriteSection::Translucent:
        {
            auto &sprites = segment->translucents.sprites;
            sprites.erase(std::find(sprites.begin(), sprites.end(), sprite));
            *sprite->dirty_flag = true;
            break;
        }
    }

    object_allocator.Free(sprite->allocation);
}

void DxScene::SetWindow(SpriteHandle sprite, const RectF *window)
{
    sprite->window = *window;
    *sprite->dirty_flag = true;
}

void DxScene::GetWindow(SpriteHandle sprite, RectF *window)
{
    *window = sprite->window;
}

void DxScene::SetLayer(SpriteHandle sprite, float layer)
{
    sprite->layer = layer;
    *sprite->dirty_flag = true;
    if (sprite->sort_flag)
        *sprite->sort_flag = true;
}

void DxScene::GetLayer(SpriteHandle sprite, float *layer)
{
    *layer = sprite->layer;
}

void DxScene::SetTexture(SpriteHandle sprite, ITexture *texture)
{
    auto new_texture = static_cast<SpriteEntry *>(texture);
    auto old_set = sprite->texture->owner;
    auto new_set = new_texture->owner;
    sprite->texture = new_texture;
    *sprite->dirty_flag = true;

    if (old_set != new_set)
    {
        auto &segment = segments[sprite->current_coord];
        switch (sprite->section)
        {
            case SpriteSection::SolidDynamic:
            {
                // Remove from the old group
                auto &old_group = segment->solid_sprites.sprites[old_set];
                old_group.erase(sprite);
                *sprite->dirty_flag = true;

                // Add to the new group
                auto &new_group = segment->solid_sprites.sprites[new_set];
                auto &new_batch = segment->solid_sprites.batches[new_set];

                new_group.insert(sprite);

                if (!new_batch)
                {
                    new_batch = std::make_unique<SpriteBatch>();
                    new_batch->set = new_set;
                }
                new_batch->dirty = true;
                sprite->dirty_flag = &new_batch->dirty;
                break;
            }

            case SpriteSection::SolidStatic:
            {
                // Remove from the old group
                auto &old_group = segment->solid_statics.sprites[old_set];
                old_group.erase(sprite);
                *sprite->dirty_flag = true;

                // Add to the new group
                auto &new_group = segment->solid_statics.sprites[new_set];
                auto &new_batch = segment->solid_statics.batches[new_set];

                new_group.insert(sprite);

                if (!new_batch)
                {
                    new_batch = std::make_unique<SpriteBatch>();
                    new_batch->set = new_set;
                }
                new_batch->dirty = true;
                sprite->dirty_flag = &new_batch->dirty;
                break;
            }

            case SpriteSection::Translucent:
            {
                segment->translucents.dirty = true;
                break;
            }
        }
    }
}

void DxScene::GetTexture(SpriteHandle sprite, ITexture **texture)
{
    *texture = sprite->texture;
}

void DxScene::SetTransform(SpriteHandle sprite, const Matrix3x2F *transform)
{
    sprite->transform = *transform;

    auto old_coord = sprite->current_coord;
    auto new_coord = sprite->CalculateCoord(segment_size);
    if (old_coord != new_coord)
    {
        auto &old_segment = segments[old_coord];
        auto &new_segment = segments[new_coord];
        auto sprite_set = sprite->texture->owner;

        switch (sprite->section)
        {
            case SpriteSection::SolidDynamic:
            {
                // Remove from the old group
                auto &old_group = old_segment->solid_sprites.sprites[sprite_set];
                old_group.erase(sprite);
                *sprite->dirty_flag = true;

                // Add to the new group
                auto &new_group = new_segment->solid_sprites.sprites[sprite_set];
                auto &new_batch = new_segment->solid_sprites.batches[sprite_set];

                new_group.insert(sprite);

                if (!new_batch)
                {
                    new_batch = std::make_unique<SpriteBatch>();
                    new_batch->set = sprite_set;
                }
                new_batch->dirty = true;
                sprite->dirty_flag = &new_batch->dirty;
                break;
            }

            case SpriteSection::SolidStatic:
            {
                // Remove from the old group
                auto &old_group = old_segment->solid_statics.sprites[sprite_set];
                old_group.erase(sprite);
                *sprite->dirty_flag = true;

                // Add to the new group
                auto &new_group = new_segment->solid_statics.sprites[sprite_set];
                auto &new_batch = new_segment->solid_statics.batches[sprite_set];

                new_group.insert(sprite);

                if (!new_batch)
                {
                    new_batch = std::make_unique<SpriteBatch>();
                    new_batch->set = sprite_set;
                }
                new_batch->dirty = true;
                sprite->dirty_flag = &new_batch->dirty;
                break;
            }

            case SpriteSection::Translucent:
            {
                // Remove from the old group
                auto &old_sprites = old_segment->translucents.sprites;
                old_sprites.erase(std::find(old_sprites.begin(), old_sprites.end(), sprite));
                *sprite->dirty_flag = true;

                // Add to the new group
                new_segment->translucents.sprites.push_back(sprite);
                new_segment->translucents.dirty = true;
                new_segment->translucents.needs_sorting = true;
                sprite->dirty_flag = &new_segment->translucents.dirty;
                sprite->sort_flag = &new_segment->translucents.needs_sorting;
                break;
            }
        }

        sprite->current_coord = new_coord;
    }
}

void DxScene::GetTransform(SpriteHandle sprite, Matrix3x2F *transform)
{
    *transform = sprite->transform;
}

void DxScene::SetTint(SpriteHandle sprite, const ColorF *color)
{
    sprite->tint = *color;
    *sprite->dirty_flag = true;
}

void DxScene::GetTint(SpriteHandle sprite, ColorF *color)
{
    *color = sprite->tint;
}

SegCoord SpriteObject::CalculateCoord(const Size2F segment_size)
{
    auto coordf = Vec2(transform.m31, transform.m32) / segment_size;
    return
    {
        (uint32_t)(int32_t)std::floor(coordf.x),
        (uint32_t)(int32_t)std::floor(coordf.y),
    };
}

SpriteObject::operator SpriteInstance() const
{
    SpriteInstance inst;
    inst.transform = transform;
    inst.material_id = texture->index;
    inst.layer = layer;
    inst.window = window;
    inst.tint = tint;
    return inst;
}
