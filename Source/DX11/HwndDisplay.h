#pragma once

#include <Renderer/Display.h>
#include <Renderer/RenderHelpers.h>
#include "DxInstance.h"

class HwndDisplay : public ImplRenderBase<IDisplay, DxInstance>
{
public:

private:
    HWND hwnd;
};
