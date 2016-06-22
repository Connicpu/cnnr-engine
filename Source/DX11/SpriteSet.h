#pragma once

#include <Common/Platform.h>
#include <Renderer/SpriteSet.h>
#include <Renderer/RenderHelpers.h>
#include <Renderer/Texture.h>
#include <vector>

#include "DxInstance.h"
#include "DxPlatform.h"

struct SpriteEntry;
class DxDevice;

class SpriteSet : public ImplRenderBase<ISpriteSet, DxInstance>
{
public:
    SpriteSet(
        DxInstance *instance, DxDevice *device,
        bool streaming, uint32_t spriteCount,
        uint32_t spriteWidth, uint32_t spriteHeight,
        const uint8_t **buffers
    );

    // Get the width and height in pixels of each image in this set
    virtual void GetSpriteSize(uint32_t *width, uint32_t *height) override;
    // Get the number of images in this set
    virtual uint32_t GetSpriteCount() override;
    // Get whether images can be streamed
    virtual bool IsStreaming() override;

    // Get the regular non-streaming sprite pointer for image number `index`. This
    // function will fail and return false if index is out of bounds.
    virtual bool GetSprite(uint32_t index, ITexture **texture) override;
    // Get a pointer to a sprite that lets you update its data. This function will
    // fail and return false if index is out of bounds or this sprite set is not
    // set up for streaming.
    virtual bool GetStreamingSprite(uint32_t index, IStreamingTexture **texture) override;

private:
    std::vector<SpriteEntry> entries;
    ComPtr<ID3D11Texture2D> backingStore;
    ComPtr<ID3D11Device> device;
    uint32_t spriteWidth, spriteHeight;
    bool isStreaming;
    friend struct SpriteEntry;
};

struct SpriteEntry : public IStreamingTexture
{
public:
    SpriteEntry(SpriteSet *owner, ID3D11Texture2D *backing, uint32_t i)
        : owner(owner), backingStore(backing), index(i)
    {
    }

    virtual void GetSize(uint32_t *width, uint32_t *height) override;
    virtual void Update(const uint8_t *data, size_t len) override;

private:
    SpriteSet *owner;
    ID3D11Texture2D *backingStore;
    uint32_t index;
};
