#pragma once

#include "RenderBase.h"
#include "Params.h"

struct SpriteObject;
using SpriteHandle = SpriteObject *;

class IScene : public IRenderBase
{
public:
    virtual SpriteHandle CreateSprite(const SpriteObjectParams *params) = 0;
    virtual void DestroySprite(SpriteHandle sprite) = 0;

    virtual void SetWindow(SpriteHandle sprite, const Math::RectF *window) = 0;
    virtual void GetWindow(SpriteHandle sprite, Math::RectF *window) = 0;

    virtual void SetLayer(SpriteHandle sprite, float layer) = 0;
    virtual void GetLayer(SpriteHandle sprite, float *layer) = 0;

    virtual void SetTexture(SpriteHandle sprite, ITexture *texture) = 0;
    virtual void GetTexture(SpriteHandle sprite, ITexture **texture) = 0;

    virtual void SetTransform(SpriteHandle sprite, const Math::Matrix3x2F *transform) = 0;
    virtual void GetTransform(SpriteHandle sprite, Math::Matrix3x2F *transform) = 0;

    virtual void SetTint(SpriteHandle sprite, const Math::ColorF *color) = 0;
    virtual void GetTint(SpriteHandle sprite, Math::ColorF *color) = 0;
};
