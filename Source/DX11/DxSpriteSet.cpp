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
    device(device),
    sprite_width(spriteWidth),
    sprite_height(spriteHeight),
    is_streaming(streaming)
{
    D3D11_TEXTURE2D_DESC tex_desc;
    D3D11_SUBRESOURCE_DATA subresources[MAX_SPRITES];
    D3D11_SUBRESOURCE_DATA *pSubresources = nullptr;

    if (spriteCount > MAX_SPRITES)
        __fastfail(1);

    tex_desc.ArraySize = spriteCount;
    tex_desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
    tex_desc.CPUAccessFlags = streaming ? D3D11_CPU_ACCESS_WRITE : 0;
    tex_desc.Usage = streaming ? D3D11_USAGE_DYNAMIC : D3D11_USAGE_DEFAULT;
    tex_desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    tex_desc.MiscFlags = 0;
    tex_desc.MipLevels = 1;
    tex_desc.SampleDesc = { 1, 0 };
    tex_desc.Width = spriteWidth;
    tex_desc.Height = spriteHeight;

    if (buffers)
    {
        for (uint32_t i = 0; i < spriteCount; ++i)
        {
            subresources[i].pSysMem = buffers[i];
            subresources[i].SysMemPitch = spriteWidth * 4;
            subresources[i].SysMemSlicePitch = spriteWidth * spriteHeight * 4;
        }
        pSubresources = subresources;
    }

    auto hr = device->device->CreateTexture2D(&tex_desc, pSubresources, &backing_store);
    CheckHR(hr);

    D3D11_SHADER_RESOURCE_VIEW_DESC srv_desc;
    srv_desc.Format = tex_desc.Format;
    srv_desc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
    srv_desc.Texture2D.MipLevels = 1;
    srv_desc.Texture2D.MostDetailedMip = 0;
    CheckHR(device->device->CreateShaderResourceView(backing_store, &srv_desc, &sr_view));

    entries.reserve(spriteCount);
    for (uint32_t i = 0; i < spriteCount; ++i)
    {
        entries.emplace_back(this, backing_store.p, i);
    }
}

void DxSpriteSet::GetSpriteSize(uint32_t *width, uint32_t *height)
{
    *width = sprite_width;
    *height = sprite_height;
}

uint32_t DxSpriteSet::GetSpriteCount()
{
    return (uint32_t)entries.size();
}

bool DxSpriteSet::IsStreaming()
{
    return is_streaming;
}

void DxSpriteSet::SetPixelArt(bool pa)
{
    is_pixel_art = pa;
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
    if (!is_streaming)
        return false;

    *texture = &entries[index];
    return true;
}

ID3D11ShaderResourceView *const &DxSpriteSet::GetView() const
{
    return sr_view.p;
}

IDevice *SpriteEntry::GetDevice()
{
    return owner->device.p;
}

void SpriteEntry::GetSize(uint32_t *width, uint32_t *height)
{
    owner->GetSpriteSize(width, height);
}

void SpriteEntry::Update(const uint8_t *data, size_t len)
{
    HRESULT hr;
    assert(owner->is_streaming);
    assert(len == (owner->sprite_width * owner->sprite_height * 4));

    uint32_t subresource = D3D11CalcSubresource(0, index, 1);
    auto *context = owner->device->context.p;

    D3D11_MAPPED_SUBRESOURCE resource;
    hr = context->Map(backing_store, subresource, D3D11_MAP_WRITE_DISCARD, 0, &resource);
    CheckHR(hr);

    for (uint32_t y = 0; y < owner->sprite_height; ++y)
    {
        auto dx_data = reinterpret_cast<char *>(resource.pData);
        auto dx_offset = dx_data + y * resource.RowPitch;
        auto data_offset = data + y * owner->sprite_width * 4;
        memcpy(dx_offset, data_offset, owner->sprite_width * 4);
    }

    context->Unmap(backing_store, subresource);
}
