#pragma once

#include "RenderBase.h"

class IRenderTarget;
class ISpriteSet;

class IDevice : public IRenderBase
{
public:
    // buffers must be an array of pointers to images that contain enough
    // RGBA data for the given width and height. If streaming is true then
    // buffers may be nullptr, or any individual buffers in that array.
    // May fail if too many sprites are requested.
    virtual void CreateSpriteSet(
        bool streaming, uint32_t spriteCount,
        uint32_t spriteWidth, uint32_t spriteHeight,
        const uint32_t **buffers,
        ISpriteSet **set
    ) = 0;
};
