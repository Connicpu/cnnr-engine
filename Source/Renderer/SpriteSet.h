#pragma once

#include "RenderBase.h"

class ITexture;
class IStreamingTexture;

class ISpriteSet : public IRenderBase
{
public:
    // Get the width and height in pixels of each image in this set
    virtual void GetSpriteSize(uint32_t *width, uint32_t *height) = 0;
    // Get the number of images in this set
    virtual uint32_t GetSpriteCount() = 0;
    // Get whether images can be streamed
    virtual bool IsStreaming() = 0;
    // Sets whether these textures should be sampled normally or treated as pixel art
    virtual void SetPixelArt(bool pa) = 0;

    // Get the regular non-streaming sprite pointer for image number `index`. This
    // function will fail and return false if index is out of bounds.
    virtual bool GetSprite(uint32_t index, ITexture **texture) = 0;
    // Get a pointer to a sprite that lets you update its data. This function will
    // fail and return false if index is out of bounds or this sprite set is not
    // set up for streaming.
    virtual bool GetStreamingSprite(uint32_t index, IStreamingTexture **texture) = 0;
};

