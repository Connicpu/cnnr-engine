#include "DxScene.h"
#include "DxCamera.h"
#include "DxException.h"

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

void DxScene::Draw(DxDevice *device, DxCamera *camera)
{
    camera->Upload(device->device, device->context);

    const int32_t CULL_MARGIN = 2;
    RectF viewport;
    camera->GetViewRect(&viewport);
    SegCoord top_left{ Point2(viewport.left, viewport.top), segment_size };
    SegCoord bottom_right{ Point2(viewport.right, viewport.bottom), segment_size };
    uint32_t x_min = std::min(int32_t(top_left.x), int32_t(bottom_right.x)) - CULL_MARGIN;
    uint32_t x_max = std::max(int32_t(top_left.x), int32_t(bottom_right.x)) + CULL_MARGIN;
    uint32_t y_min = std::min(int32_t(top_left.y), int32_t(bottom_right.y)) - CULL_MARGIN;
    uint32_t y_max = std::max(int32_t(top_left.y), int32_t(bottom_right.y)) + CULL_MARGIN;

    for (uint32_t y = y_min; y <= y_max; ++y)
    {
        for (uint32_t x = x_min; x <= x_max; ++x)
        {
            SegCoord coord{ uint32_t(x), uint32_t(y) };
            DrawSegment(coord, device, camera);
        }
    }
}

void DxScene::DrawSegment(SegCoord coord, DxDevice *device, DxCamera *camera)
{
    auto segment_iter = segments.find(coord);
    if (segment_iter == segments.end())
        return;
    auto &segment = *segment_iter->second;

    if (segment.solid_sprites.sprites.empty() &&
        segment.solid_statics.sprites.empty() &&
        segment.translucents.sprites.empty())
    {
        segments.erase(segment_iter);
        return;
    }

    // Draw all of the solid sprites
    for (auto &section : { &segment.solid_statics, &segment.solid_sprites })
    {
        if (section->sprites.empty())
            continue;

        std::vector<DxSpriteSet *> batch_kill_list;
        for (auto &pair : section->batches)
        {
            auto &key = pair.first;
            auto &batch = *pair.second;
            auto &sprites = section->sprites[key];

            if (sprites.empty())
            {
                batch_kill_list.push_back(pair.first);
                continue;
            }

            if (pair.second->dirty)
            {
                // Ensure our instance buffer has enough room for all these sprites
                batch.instance_buffer.Reserve(device->device, uint32_t(sprites.size()));

                // Upload the sprites into the instance buffer
                auto upload = batch.instance_buffer.BeginUpload(device->context);
                for (auto *sprite : sprites)
                {
                    batch.instance_buffer.Push(*sprite, upload);
                }
                batch.instance_buffer.EndUpload(upload);
                pair.second->dirty = false;
            }

            // Draw the batch :3
            DrawBatch(batch, device, camera);
        }

        // Remove empty batches
        for (auto &kill : batch_kill_list)
        {
            section->sprites.erase(kill);
            section->batches.erase(kill);
        }
    }

    // Draw all of the translucent sprites
    if (!segment.translucents.sprites.empty())
    {
        auto &section = segment.translucents;

        // Resort the sprites if any layers have changed in this region
        if (section.needs_sorting)
        {
            std::sort(section.sprites.begin(), section.sprites.end());
            section.needs_sorting = false;
            section.dirty = true;
        }

        // Upload the sprites if anything has changed
        if (section.dirty)
        {
            auto old_batches = std::move(section.batches);
            DxSpriteSet *run_set = nullptr;
            std::vector<SpriteHandle> current_run;

            // Subroutine to pop an item from the batch queue or create a new one
            auto next_batch = [&]
            {
                if (old_batches.empty())
                {
                    return SpriteBatch{};
                }
                else
                {
                    auto batch = std::move(old_batches.back());
                    old_batches.pop_back();
                    return std::move(batch);
                }
            };

            // Subroutine to push the current list of sprites into the batch list
            auto commit_run = [&]
            {
                auto batch = next_batch();

                auto &buffer = batch.instance_buffer;
                batch.set = run_set;
                batch.dirty = false;
                buffer.Reserve(device->device, uint32_t(current_run.size()));

                auto upload = buffer.BeginUpload(device->context);
                for (auto *sprite : current_run)
                {
                    buffer.Push(*sprite, upload);
                }
                buffer.EndUpload(upload);
                section.batches.push_back(std::move(batch));
                current_run.clear();
            };

            // Put all of the sprites into runs
            for (auto *sprite : section.sprites)
            {
                auto set = sprite->texture->owner;
                if (run_set && run_set != set)
                    commit_run();
                run_set = set;
                current_run.push_back(sprite);
            }
            commit_run();

            section.dirty = false;
        }

        for (auto &batch : section.batches)
        {
            DrawBatch(batch, device, camera);
        }
    }
}

void DxScene::DrawBatch(const SpriteBatch &batch, DxDevice *device, DxCamera *camera)
{
}

SegCoord SpriteObject::CalculateCoord(const Size2F segment_size)
{
    return{ Point2(transform.m31, transform.m32), segment_size };
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
