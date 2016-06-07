#pragma once

#include "DxPlatform.h"

class IDxTarget
{
public:
    virtual void GetRTV(ID3D11RenderTargetView **rtv) = 0;
};
