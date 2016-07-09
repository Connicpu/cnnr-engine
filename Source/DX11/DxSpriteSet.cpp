#include "DxSpriteSet.h"
#include "DxException.h"
#include "DxDevice.h"

const uint32_t MAX_SPRITES = 4096;

DxSpriteSet::DxSpriteSet(
    DxInstance *instance, DxDevice *device,
    bool streaming, uint32_t spriteCount,
    uint32_t spriteWidth, uint32_t spriteHeight,
    const uint8_t *const * buffers)
    : ImplRenderBase<ISpriteSet, DxInstance>{ instance },
    device(device->device),
    spriteWidth(spriteWidth),
    spriteHeight(spriteHeight),
    isStreaming(streaming)
{
    D3D11_TEXTURE2D_DESC texDesc;
    D3D11_SUBRESOURCE_DATA subresources[MAX_SPRITES];
    D3D11_SUBRESOURCE_DATA *pSubresources = nullptr;

    if (spriteCount > MAX_SPRITES)
        __fastfail(1);

    texDesc.ArraySize = spriteCount;
    texDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
    texDesc.CPUAccessFlags = streaming ? D3D11_CPU_ACCESS_WRITE : 0;
    texDesc.Usage = streaming ? D3D11_USAGE_DYNAMIC : D3D11_USAGE_DEFAULT;
    texDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    texDesc.MiscFlags = 0;
    texDesc.MipLevels = 1;
    texDesc.SampleDesc = { 1, 0 };
    texDesc.Width = spriteWidth;
    texDesc.Height = spriteHeight;

    if (buffers)
    {
        for (uint32_t i = 0; i < spriteCount; ++i)
        {
            subresources[i].pSysMem = buffers[i];
            subresources[i].SysMemPitch = spriteWidth * 4;
            subresources[i].SysMemSlicePitch = 0;
        }
        pSubresources = subresources;
    }

    auto hr = device->device->CreateTexture2D(&texDesc, pSubresources, &backingStore);
    CheckHR(hr);

    entries.reserve(spriteCount);
    for (uint32_t i = 0; i < spriteCount; ++i)
    {
        entries.emplace_back(this, backingStore.p, i);
    }
}

void DxSpriteSet::GetSpriteSize(uint32_t *width, uint32_t *height)
{
    *width = spriteWidth;
    *height = spriteHeight;
}

uint32_t DxSpriteSet::GetSpriteCount()
{
    return (uint32_t)entries.size();
}

bool DxSpriteSet::IsStreaming()
{
    return isStreaming;
}

bool DxSpriteSet::GetSprite(uint32_t index, ITexture **texture)
{
    if (index >= entries.size())
        return false;

    *texture = &entries[index];
    return true;
}

bool DxSpriteSet::GetStreamingSprite(uint32_t index, IStreamingTexture ** texture)
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
    hr = context->Map(backing_store, subresource, D3D11_MAP_WRITE_DISCARD, 0, &resource);
    CheckHR(hr);

    memcpy(resource.pData, data, len);

    context->Unmap(backing_store, subresource);
}
