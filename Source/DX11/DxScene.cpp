#include "DxScene.h"

using namespace Math;

DxScene::DxScene(DxInstance *inst, DxDevice *dev)
    : ImplRenderBase<IScene, DxInstance>(inst), device(dev)
{
}

SpriteHandle DxScene::CreateSprite(const SpriteObjectParams *params)
{
    auto allocation = object_allocator.Alloc();
    SpriteHandle sprite = (SpriteHandle)allocation.memory;
    sprite->transform = params->transform;
    sprite->texture = static_cast<SpriteEntry *>(params->texture);
    sprite->layer = params->layer;
    sprite->window = params->window;
    sprite->tint = params->tint;
    sprite->current_coord = sprite->CalculateCoord(segment_size);
    sprite->allocation = allocation;

    auto segment = &segments[sprite->current_coord];
}

void DxScene::DestroySprite(SpriteHandle sprite)
{
}

void DxScene::SetWindow(SpriteHandle sprite, const RectF *window)
{
}

void DxScene::GetWindow(SpriteHandle sprite, RectF *window)
{
}

void DxScene::SetLayer(SpriteHandle sprite, float layer)
{
}

void DxScene::GetLayer(SpriteHandle sprite, float *layer)
{
}

void DxScene::SetTexture(SpriteHandle sprite, ITexture *texture)
{
}

void DxScene::GetTexture(SpriteHandle sprite, ITexture **texture)
{
}

void DxScene::SetTransform(SpriteHandle sprite, const Matrix3x2F *transform)
{
}

void DxScene::GetTransform(SpriteHandle sprite, Matrix3x2F *transform)
{
}

void DxScene::SetTint(SpriteHandle sprite, const ColorF *color)
{
}

void DxScene::GetTint(SpriteHandle sprite, ColorF *color)
{
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
