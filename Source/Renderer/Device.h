#pragma once

#include "RenderBase.h"

class IRenderTarget;

class IDevice : public IRenderBase
{
public:
    virtual bool SetTarget(IRenderTarget *target) = 0;

    // buffers must be an array of pointers to images that contain enough
    // RGBA data for the given width and height. If streaming is true then
    // buffers may be nullptr, or any individual buffers in that array.
    // May fail if too many sprites are requested.
    virtual bool CreateSpriteSet(
        bool streaming, uint32_t spriteCount,
        uint32_t spriteWidth, uint32_t spriteHeight,
        const uint32_t **buffers
    ) = 0;
};
