#include "pch.h"
#include "DxScene.h"
#include "DxCamera.h"
#include "DxException.h"

using namespace Math;

static SpriteVertex SpriteVertices[] =
{
    { Vec2(-0.5f, 0.5f), Vec2(0, 0) },
    { Vec2(-0.5f, -0.5f), Vec2(0, 1) },
    { Vec2(0.5f, -0.5f), Vec2(1, 1) },

    { Vec2(-0.5f, 0.5f), Vec2(0, 0) },
    { Vec2(0.5f, -0.5f), Vec2(1, 1) },
    { Vec2(0.5f, 0.5f), Vec2(1, 0) },
};

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
    *sprite->dirty_flag = true;

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
    if (!rasterizer)
        InitializeRasterizer(device->device);
    if (!blender)
        InitializeBlender(device->device);
    if (!sampler)
        InitializeSampler(device->device);

    if (!sprite_vertices.Get())
    {
        sprite_vertices.CreateImmutable(
            device->device,
            ARRAYSIZE(SpriteVertices),
            SpriteVertices);
    }

    camera->Upload(device->device, device->context);

    device->context->RSSetState(rasterizer);
    device->context->OMSetBlendState(blender, nullptr, 0xFFFFFF);

    const int32_t CULL_MARGIN = 2;
    RectF viewport;
    camera->GetViewRect(&viewport);
    SegCoord top_left{ Point2(viewport.left, viewport.top), segment_size };
    SegCoord bottom_right{ Point2(viewport.right, viewport.bottom), segment_size };
    int32_t x_min = std::min(int32_t(top_left.x), int32_t(bottom_right.x)) - CULL_MARGIN;
    int32_t x_max = std::max(int32_t(top_left.x), int32_t(bottom_right.x)) + CULL_MARGIN;
    int32_t y_min = std::min(int32_t(top_left.y), int32_t(bottom_right.y)) - CULL_MARGIN;
    int32_t y_max = std::max(int32_t(top_left.y), int32_t(bottom_right.y)) + CULL_MARGIN;

    for (int32_t y = y_min; y <= y_max; ++y)
    {
        for (int32_t x = x_min; x <= x_max; ++x)
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

        // Resort the sprites if any layers have changed in this region.
        // Presumably this list will be small *crosses fingers*
        if (section.needs_sorting)
        {
            std::stable_sort(section.sprites.begin(), section.sprites.end());
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
    ID3D11Buffer *const vert_buffers[] = { sprite_vertices.Get(), batch.instance_buffer.Get() };
    const UINT vert_strides[] = { sizeof(SpriteVertex), sizeof(SpriteInstance) };
    const UINT vert_offsets[] = { 0, 0 };

    ID3D11DeviceContext *context = device->context;
    if (batch.set->is_pixel_art)
        context->PSSetSamplers(0, 1, &pixel_sampler.p);
    else
        context->PSSetSamplers(0, 1, &sampler.p);

    device->sprite_shader.Bind(context);
    context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
    context->IASetVertexBuffers(0, 2, vert_buffers, vert_strides, vert_offsets);
    context->VSSetConstantBuffers(0, 1, &camera->buffer.p);
    context->PSSetShaderResources(0, 1, &batch.set->GetView());

    context->DrawInstanced(ARRAYSIZE(SpriteVertices), batch.instance_buffer.Count(), 0, 0);
}

void DxScene::InitializeRasterizer(ID3D11Device *device)
{
    D3D11_RASTERIZER_DESC desc;
    desc.FillMode = D3D11_FILL_SOLID;
    desc.CullMode = D3D11_CULL_NONE;
    desc.FrontCounterClockwise = true;
    desc.DepthBias = 0;
    desc.DepthBiasClamp = 0.0f;
    desc.SlopeScaledDepthBias = 0.0f;
    desc.DepthClipEnable = false;
    desc.ScissorEnable = false;
    desc.MultisampleEnable = true;
    desc.AntialiasedLineEnable = true;
    CheckHR(device->CreateRasterizerState(&desc, &rasterizer));
}

void DxScene::InitializeBlender(ID3D11Device *device)
{
    D3D11_BLEND_DESC desc;
    desc.AlphaToCoverageEnable = TRUE;
    desc.IndependentBlendEnable = FALSE;
    desc.RenderTarget[0].BlendEnable = TRUE;
    desc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
    desc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
    desc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
    desc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_SRC_ALPHA;
    desc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_DEST_ALPHA;
    desc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
    desc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
    CheckHR(device->CreateBlendState(&desc, &blender));
}

void DxScene::InitializeSampler(ID3D11Device *device)
{
    D3D11_SAMPLER_DESC desc;
    desc.Filter = D3D11_FILTER_ANISOTROPIC;
    desc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
    desc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
    desc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
    desc.MipLODBias = 0;
    desc.MaxAnisotropy = 8;
    desc.MinLOD = -FLT_MAX;
    desc.MaxLOD = FLT_MAX;
    desc.ComparisonFunc = D3D11_COMPARISON_NEVER;
    *(ColorF *)desc.BorderColor = Color(1, 1, 1);
    CheckHR(device->CreateSamplerState(&desc, &sampler));

    desc.Filter = D3D11_FILTER_MIN_MAG_MIP_POINT;
    CheckHR(device->CreateSamplerState(&desc, &pixel_sampler));
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
