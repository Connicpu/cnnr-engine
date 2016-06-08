#include "HwndDisplay.h"
#include <Common/Platform.h>
#include <Common/InitOnce.h>
#include <Common/WindowsHelpers.h>
#include "DxDevice.h"

// Window Class Helpers
static RunOnce REGISTER_WINDOW_CLASS_RO;
static const wchar_t WINDOW_CLASS_NAME[] = L"CnnrEngineDX11MainWindowClass";

HwndDisplay::HwndDisplay(DxInstance *instance, DxDevice *device, LPCWSTR title)
    : ImplRenderBase<IDisplay, DxInstance>(instance), device(device)
{
    InitializeWindow(title);
    InitializeSwap();
}

HwndDisplay::~HwndDisplay()
{
}

bool HwndDisplay::PollEvent(Event **event)
{
    for (;;)
    {
        if (event_queue.try_pop(*event))
            return true;

        MSG msg;
        if (PeekMessageW(&msg, hwnd, 0, 0, PM_REMOVE))
        {
            TranslateMessage(&msg);
            DispatchMessageW(&msg);
            continue;
        }

        return false;
    }
}

void HwndDisplay::Present()
{
}

void HwndDisplay::Clear(float color[4])
{
}

void HwndDisplay::GetTargetObject(void *target)
{
    *(IDxTarget **)target = this;
}

void HwndDisplay::GetRTV(ID3D11RenderTargetView **rtv)
{
    ComPtr<ID3D11RenderTargetView> ptr{ this->render_target };
    *rtv = ptr.Detach();
}

LRESULT HwndDisplay::WndProc(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp)
{
    HwndDisplay *display = (HwndDisplay *)GetWindowLongPtrW(hwnd, GWLP_USERDATA);
    switch (msg)
    {
        // Upon creation of the window, we get the display from the CREATESTRUCT(W)
        // and set its value into the USERDATA section of our window.
        case WM_CREATE:
        {
            CREATESTRUCTW *create = (CREATESTRUCTW *)lp;
            display = (HwndDisplay *)create->lpCreateParams;
            SetWindowLongPtrW(hwnd, GWLP_USERDATA, (LONG_PTR)display);
            return 0;
        }
        default:
        {
            return DefWindowProcW(hwnd, msg, wp, lp);
        }
    }
}

void HwndDisplay::InitializeWindow(LPCWSTR title)
{
    HINSTANCE hinstance = GetHinstanceFromFn(&WndProc);
    RegisterWindowClass(hinstance);

    hwnd = CreateWindowExW(
        WS_EX_OVERLAPPEDWINDOW, // ex-style
        WINDOW_CLASS_NAME,      // class
        title,                  // window title
        WS_OVERLAPPEDWINDOW,    // style
        CW_USEDEFAULT,          // x-position
        CW_USEDEFAULT,          // y-position
        CW_USEDEFAULT,          // width
        CW_USEDEFAULT,          // height
        nullptr,                // parent window
        nullptr,                // menu handle
        hinstance,              // module instance
        (LPVOID)this            // creation parameter (passed to WM_CREATE's CREATESTRUCT)
    );
}

void HwndDisplay::InitializeSwap()
{
}

void HwndDisplay::RegisterWindowClass(HINSTANCE hinst)
{
    REGISTER_WINDOW_CLASS_RO.Once([hinst]()
    {
        WNDCLASSEXW wndc = { sizeof(wndc) };
        wndc.style = CS_VREDRAW | CS_HREDRAW;
        wndc.lpfnWndProc = WndProc;
        wndc.cbClsExtra = 0;
        wndc.cbWndExtra = sizeof(HwndDisplay *);
        wndc.hInstance = hinst;
        wndc.hIcon = LoadIconA(nullptr, IDI_APPLICATION);
        wndc.hCursor = LoadCursorA(nullptr, IDC_ARROW);
        wndc.hbrBackground = nullptr;
        wndc.lpszMenuName = nullptr;
        wndc.lpszClassName = WINDOW_CLASS_NAME;
        wndc.hIconSm = wndc.hIcon;
        RegisterClassExW(&wndc);
    });
}
