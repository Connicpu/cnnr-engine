#pragma once

#include <Renderer/Scene.h>
#include <Common/Hash.h>
#include <Common/BucketAllocator.h>
#include "DxInstance.h"
#include "DxDevice.h"
#include "DxSpriteSet.h"
#include "DxVertexBuffer.h"

struct SegCoord;
struct SpriteObject;
struct SpriteInstance;
class DxCamera;

struct SegCoord
{
    SegCoord(Point2F point, Size2F seg_size)
    {
        using namespace Math;
        auto coordf = Vec2(point) / seg_size;
        x = (uint32_t)(int32_t)std::floor(coordf.x);
        y = (uint32_t)(int32_t)std::floor(coordf.y);
    }

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
    DxVertexBuffer<SpriteInstance> instance_buffer;
    DxSpriteSet *set;
    bool dirty = false;
};

struct SpriteVertex
{
    inline SpriteVertex(Vec2F pos, Vec2F uv)
        : pos(pos), uv(uv)
    {
    }

    Vec2F pos;
    Vec2F uv;
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
    bool dirty = true;
    bool needs_sorting = true;
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

    void Draw(DxDevice *device, DxCamera *camera);
    void DrawSegment(SegCoord coord, DxDevice *device, DxCamera *camera);
    void DrawBatch(const SpriteBatch &batch, DxDevice *device, DxCamera *camera);

    HashMap<SegCoord, std::unique_ptr<SceneSegment>, Fnv1A> segments;
    TBucketAllocator<SpriteObject, 2048> object_allocator;
    DxVertexBuffer<SpriteVertex> sprite_vertices;
    ComPtr<ID3D11RasterizerState> rasterizer;
    ComPtr<ID3D11BlendState> blender;
    ComPtr<ID3D11SamplerState> sampler;
    Size2F segment_size;

private:
    void InitializeRasterizer(ID3D11Device *device);
    void InitializeBlender(ID3D11Device *device);
    void InitializeSampler(ID3D11Device *device);
};
