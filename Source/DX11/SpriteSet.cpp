#include "SpriteSet.h"
#include "DxException.h"

SpriteSet::SpriteSet(
    DxInstance *instance, DxDevice *device,
    bool streaming, uint32_t spriteCount,
    uint32_t spriteWidth, uint32_t spriteHeight,
    const uint32_t ** buffers)
    : ImplRenderBase<ISpriteSet, DxInstance>{ instance },
    spriteWidth(spriteWidth), spriteHeight(spriteHeight)
{
    D3D11_TEXTURE2D_DESC texDesc;
    texDesc.ArraySize = spriteCount;
    texDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
    texDesc.CPUAccessFlags = streaming ? D3D11_CPU_ACCESS_WRITE : 0;
}

void SpriteSet::GetSpriteSize(uint32_t *width, uint32_t *height)
{
}

uint32_t SpriteSet::GetSpriteCount()
{
    return (uint32_t)entries.size();
}

bool SpriteSet::IsStreaming()
{
    return isStreaming;
}

bool SpriteSet::GetSprite(uint32_t index, ITexture **texture)
{
    if (index >= entries.size())
        return false;

    *texture = &entries[index];
    return true;
}

bool SpriteSet::GetStreamingSprite(uint32_t index, IStreamingTexture ** texture)
{
    if (index >= entries.size())
        return false;
    if (!isStreaming)
        return false;

    *texture = &entries[index];
    return true;
}

void SpriteEntry::GetSize(uint32_t *width, uint32_t *height)
{
    owner->GetSpriteSize(width, height);
}

void SpriteEntry::Update(const uint8_t *data, size_t len)
{
    HRESULT hr;
    assert(owner->isStreaming);
    assert(len == (owner->spriteWidth * owner->spriteHeight * 4));

    uint32_t subresource = D3D11CalcSubresource(0, index, 1);
    ComPtr<ID3D11DeviceContext> context;
    owner->device->GetImmediateContext(&context);

    D3D11_MAPPED_SUBRESOURCE resource;
    hr = context->Map(backingStore, subresource, D3D11_MAP_WRITE_DISCARD, 0, &resource);
    CheckHR(hr);

    memcpy(resource.pData, data, len);

    context->Unmap(backingStore, subresource);
}
