#pragma once

#include <Renderer/Scene.h>
#include <Common/Hash.h>
#include <Common/BucketAllocator.h>
#include "DxInstance.h"
#include "DxDevice.h"
#include "DxSpriteSet.h"

struct SegCoord;
struct SpriteObject;
struct SpriteInstance;

struct SegCoord
{
    SegCoord(uint32_t x, uint32_t y)
        : x(x), y(y)
    {
    }

    uint32_t x, y;
};

inline bool operator==(SegCoord lhs, SegCoord rhs)
{
    return lhs.x == rhs.x && lhs.y == rhs.y;
}

template <typename H>
inline void hash_apply(const SegCoord &coord, H &h)
{
    h.write(&coord, sizeof(coord));
}

struct SpriteObject
{
    Matrix3x2F transform;
    SpriteEntry *texture;
    float layer;
    RectF window;
    ColorF tint;

    SegCoord current_coord;
    BucketAllocation allocation;

    SegCoord CalculateCoord(const Size2F segment_size);

    operator SpriteInstance() const;
};

struct SpriteInstance
{
    Matrix3x2F transform;
    uint32_t material_id;
    float layer;
    RectF window;
    ColorF tint;
};

struct StaticSection
{
    bool dirty;
};

struct SceneSegment
{
    SegCoord coord;

    StaticSection solid_statics;
};

class DxScene : public ImplRenderBase<IScene, DxInstance>
{
public:
    DxScene(DxInstance *inst, DxDevice *dev);

    virtual SpriteHandle CreateSprite(const SpriteObjectParams *params) override;
    virtual void DestroySprite(SpriteHandle sprite) override;

    virtual void SetWindow(SpriteHandle sprite, const RectF *window) override;
    virtual void GetWindow(SpriteHandle sprite, RectF *window) override;

    virtual void SetLayer(SpriteHandle sprite, float layer) override;
    virtual void GetLayer(SpriteHandle sprite, float *layer) override;

    virtual void SetTexture(SpriteHandle sprite, ITexture *texture) override;
    virtual void GetTexture(SpriteHandle sprite, ITexture **texture) override;

    virtual void SetTransform(SpriteHandle sprite, const Matrix3x2F *transform) override;
    virtual void GetTransform(SpriteHandle sprite, Matrix3x2F *transform) override;

    virtual void SetTint(SpriteHandle sprite, const ColorF *color) override;
    virtual void GetTint(SpriteHandle sprite, ColorF *color) override;

    RPtr<DxDevice> device;
    HashMap<SegCoord, SceneSegment, Fnv1A> segments;
    TBucketAllocator<SpriteObject, 2048> object_allocator;
    Size2F segment_size;
};
