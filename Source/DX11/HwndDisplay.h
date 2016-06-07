#pragma once

#include <concurrent_queue.h>
#include <Renderer/Display.h>
#include <Renderer/RenderHelpers.h>
#include "DxInstance.h"
#include "DxPlatform.h"
#include "DxTarget.h"

class DxDevice;

class HwndDisplay : public ImplRenderBase<IDisplay, DxInstance>, IDxTarget
{
public:
    HwndDisplay(DxInstance *instance, DxDevice *device, LPCWSTR title);
    ~HwndDisplay();

    // IDisplay
    virtual bool PollEvent(Event **event) override;
    virtual void Present() override;

    // IRenderTarget
    virtual void Clear(float color[4]) override;
    virtual void GetTargetObject(void *target) override;

    // IDxTarget
    virtual void GetRTV(ID3D11RenderTargetView **rtv) = 0;

private:
    static LRESULT WINAPI WndProc(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp);

    void InitializeWindow(LPCWSTR title);
    void InitializeSwap();

    static void RegisterWindowClass(HINSTANCE hinst);

    concurrency::concurrent_queue<Event *> event_queue;
    HWND hwnd;

    RPtr<DxDevice> device;
    ComPtr<IDXGISwapChain> swap_chain;
    ComPtr<ID3D11RenderTargetView> render_target;
};
