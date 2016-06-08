#include "DxInstance.h"
#include "DxDevice.h"
#include "HwndDisplay.h"
#include "DxException.h"
#include <Renderer/RenderHelpers.h>
#include <codecvt>

#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3d11.lib")

template <typename Elem>
using Vec = std::vector<Elem>;

template <typename Elem, template <typename _Elem> typename Cont = Vec>
class Stack
{
public:
    Stack() = default;

private:
    Cont<Elem> container;
};

void CreateRenderInstance(const CreateInstParams *params, IRenderInstance **inst)
{
    RPtr<DxInstance> ptr = MakeRenderObject<DxInstance>(params);
    *inst = ptr.Release();

    Stack<int> stack;
}

DxInstance::DxInstance(const CreateInstParams *params)
{
    CheckHR(CreateDXGIFactory1(IID_PPV_ARGS(&dxgi_factory)));
    CheckHR(dxgi_factory->EnumAdapters1(params->adapter_num, &dxgi_adapter));
}

void DxInstance::CreateDevice(const DeviceParams *params, IDevice **device)
{
    UINT flags = D3D11_CREATE_DEVICE_BGRA_SUPPORT;
    if (params->debug)
        flags |= D3D11_CREATE_DEVICE_DEBUG;

    static const D3D_FEATURE_LEVEL feature_levels[] =
    {
        D3D_FEATURE_LEVEL_10_0,
    };

    D3D_FEATURE_LEVEL feature_level;
    ComPtr<ID3D11Device> d3d_device;
    ComPtr<ID3D11DeviceContext> d3d_context;
    CheckHR(D3D11CreateDevice(
        dxgi_adapter,
        D3D_DRIVER_TYPE_UNKNOWN,
        nullptr,
        flags,
        feature_levels,
        ARRAYSIZE(feature_levels),
        D3D11_SDK_VERSION,
        &d3d_device,
        &feature_level,
        &d3d_context
    ));
}

void DxInstance::CreateDisplay(const DisplayParams *params, IDisplay **display)
{
    auto dev = dynamic_cast<DxDevice *>(params->device);
    assert(dev);

    std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
    std::wstring title = converter.from_bytes(params->window_title);

    *display = MakeRenderObject<HwndDisplay>(this, dev, title.c_str()).Release();
}
