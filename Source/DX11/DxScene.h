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

inline bool operator!=(SegCoord lhs, SegCoord rhs)
{
    return lhs.x != rhs.x || lhs.y != rhs.y;
}

template <typename H>
inline void hash_apply(const SegCoord &coord, H &h)
{
    h.write(&coord, sizeof(coord));
}

enum class SpriteSection
{
    SolidDynamic,
    SolidStatic,
    Translucent,
};

struct SpriteObject
{
    Matrix3x2F transform;
    SpriteEntry *texture;
    float layer;
    RectF window;
    ColorF tint;

    SpriteSection section;
    SegCoord current_coord;
    BucketAllocation allocation;
    bool *dirty_flag;
    bool *sort_flag;

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

struct SpriteBatch
{
    DxSpriteSet *set;
    ComPtr<ID3D11Buffer> instance_buffer;
    bool dirty = false;
};

struct SolidSection
{
    HashMap<DxSpriteSet *, HashSet<SpriteHandle, Fnv1A>, Fnv1A> sprites;
    HashMap<DxSpriteSet *, std::unique_ptr<SpriteBatch>, Fnv1A> batches;
};

struct TranslucentSection
{
    std::vector<SpriteHandle> sprites;
    std::vector<SpriteBatch> batches;
    bool dirty;
    bool needs_sorting;
};

struct SceneSegment
{
    SolidSection solid_sprites;
    SolidSection solid_statics;
    TranslucentSection translucents;
};

class DxScene : public ImplRenderBase<IScene, DxInstance>
{
public:
    DxScene(DxInstance *inst, Size2F segment_size);

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

    HashMap<SegCoord, std::unique_ptr<SceneSegment>, Fnv1A> segments;
    TBucketAllocator<SpriteObject, 2048> object_allocator;
    Size2F segment_size;
};
