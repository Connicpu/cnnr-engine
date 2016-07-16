#pragma once

#include "RenderBase.h"

class IDevice;

class ITexture
{
public:
    virtual IDevice *GetDevice() = 0;
    virtual void GetSize(uint32_t *width, uint32_t *height) = 0;
};

class IStreamingTexture : public ITexture
{
public:
    virtual void Update(const uint8_t *data, size_t len) = 0;
};

