#pragma once

#include "RenderBase.h"
#include "Params.h"

class IRenderTarget;
class ISpriteSet;
class IScene;
class ICamera;

class IDevice : public IRenderBase
{
public:
    // buffers must be an array of pointers to images that contain enough
    // RGBA data for the given width and height. If streaming is true then
    // buffers may be nullptr, or any individual buffers in that array.
    // May fail if too many sprites are requested.
    virtual void CreateSpriteSet(
        const SpriteSetParams *params,
        ISpriteSet **set
    ) = 0;

    virtual void CreateScene(
        IScene **scene
    ) = 0;

    virtual void DrawScene(
        IScene *scene,
        ICamera *camera
    ) = 0;

    // TODO: Other creation functions that need a device
};

