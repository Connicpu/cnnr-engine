#include "HwndDisplay.h"
#include <Common/Platform.h>
#include <Common/InitOnce.h>
#include <Common/WindowsHelpers.h>
#include "DxDevice.h"
#include "DxException.h"

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

bool HwndDisplay::PollEvent(EventPtr &event)
{
    for (;;)
    {
		Event *ptr;
		if (event_queue.try_pop(ptr))
		{
			event = EventPtr{ ptr, EventFree(&free) };
			return true;
		}

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
    UINT flags = 0;
    if (occluded)
    {
        flags |= DXGI_PRESENT_TEST;
    }

    auto result = swap_chain->Present(0, flags);
    if (result == DXGI_STATUS_OCCLUDED)
    {
        occluded = true;
        return;
    }
    
    occluded = false;
    CheckHR(result);
}

bool HwndDisplay::Closed()
{
	return closed;
}

void HwndDisplay::Clear(float color[4])
{
    device->context->ClearRenderTargetView(render_target, color);
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
        case WM_MOUSEMOVE:
        {
			if (display)
			{
				Event::MouseMoved event;
				event.type = EventType::MouseMoved;
				event.x = (short)LOWORD(lp);
				event.y = (short)HIWORD(lp);
				display->event_queue.push(MakeEvent(event).release());
			}
			break;
        }
		case WM_CLOSE:
		{
			if (display)
			{
				Event event;
				event.type = EventType::Closed;
				display->closed = true;
				display->event_queue.push(MakeEvent(event).release());
			}
			break;
		}
        default:
        {
            return DefWindowProcW(hwnd, msg, wp, lp);
        }
    }
	return DefWindowProcW(hwnd, msg, wp, lp);
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

	ShowWindow(hwnd, SW_SHOW);
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
        wndc.hIcon = LoadIcon(nullptr, IDI_APPLICATION);
        wndc.hCursor = LoadCursor(nullptr, IDC_ARROW);
        wndc.hbrBackground = nullptr;
        wndc.lpszMenuName = nullptr;
        wndc.lpszClassName = WINDOW_CLASS_NAME;
        wndc.hIconSm = wndc.hIcon;
        RegisterClassExW(&wndc);
    });
}

