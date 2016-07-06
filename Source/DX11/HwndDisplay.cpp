#include "HwndDisplay.h"
#include <Common/Platform.h>
#include <Common/InitOnce.h>
#include <Common/WindowsHelpers.h>
#include "DxDevice.h"
#include "DxException.h"
#include <iostream>
#include <iomanip>

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

bool HwndDisplay::PollEvent(EventStorage &event)
{
    for (;;)
    {
        EventStorage storage;
        if (event_queue.try_pop(storage))
        {
            event = storage;
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

void HwndDisplay::BeginDraw()
{
    if (dirty_buffers)
    {
        InitializeSwap();
    }
}

void HwndDisplay::Clear(const float color[4])
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

inline void GetEventButton(UINT msg, WPARAM wp, MouseButton *button, ElementState *state)
{
    UINT base;
    if (msg >= WM_XBUTTONDOWN)
    {
        base = msg - WM_XBUTTONDOWN;
        if (HIWORD(wp) == XBUTTON1)
            *button = MouseButton::X1;
        else
            *button = MouseButton::X2;
    }
    else if (msg >= WM_MBUTTONDOWN)
    {
        base = msg - WM_MBUTTONDOWN;
        *button = MouseButton::Middle;
    }
    else if (msg >= WM_RBUTTONDOWN)
    {
        base = msg - WM_RBUTTONDOWN;
        *button = MouseButton::Right;
    }
    else
    {
        base = msg - WM_LBUTTONDOWN;
        *button = MouseButton::Left;
    }

    switch (base)
    {
        case 0:
            *state = ElementState::Pressed;
            break;
        case 1:
            *state = ElementState::Released;
            break;
        default:
            assert("Invalid event made it in here" && false);
            unreachable();
    }
}

static VirtualKeyCode MapVK(int vk, bool &has);

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

        // Window resizes!
        case WM_SIZE:
        {
            if (display)
            {
                Event::Resized event;
                event.width = LOWORD(lp);
                event.height = HIWORD(lp);
                display->event_queue.push(event);
                display->dirty_buffers = true;
            }
            break;
        }
        
        // Window got moved :3
        case WM_MOVE:
        {
            if (display)
            {
                Event::Moved event;
                event.x = (short)LOWORD(lp);
                event.y = (short)HIWORD(lp);
                display->event_queue.push(event);
            }
            break;
        }

        // RIP Window
        case WM_CLOSE:
        {
            PostQuitMessage(0);
            if (display)
            {
                Event event;
                event.type = EventType::Closed;
                display->closed = true;
                display->event_queue.push(event);
            }
            break;
        }

        // Oh boy! Drag-and-drop files! :D
        case WM_DROPFILES:
        {
            auto drop = (HDROP)wp;
            if (display)
            {
                uint32_t file_count = DragQueryFileW(drop, UINT_MAX, nullptr, 0);
                for (uint32_t i = 0; i < file_count; ++i)
                {
                    POINT point;
                    uint32_t path_len = DragQueryFileW(drop, i, nullptr, 0);
                    std::vector<wchar_t> temp_path(path_len + 1);
                    if (DragQueryFileW(drop, i, temp_path.data(), (uint32_t)temp_path.size()) &&
                        DragQueryPoint(drop, &point))
                    {
                        Event::DroppedFile event;
                        event.x = point.x;
                        event.y = point.y;
                        event.path = temp_path.data();
                        display->event_queue.push(event);
                    }
                }
            }
            DragFinish(drop);
            return 0;
        }
        
        // Receive characters!
        case WM_CHAR:
        {
            thread_local static bool HAS_HIGH_SURROGATE;
            thread_local static wchar_t LAST_HIGH_SURROGATE;

            uint32_t char_code;
            if (IS_HIGH_SURROGATE(wp))
            {
                HAS_HIGH_SURROGATE = true;
                LAST_HIGH_SURROGATE = (wchar_t)wp;
                break;
            }
            else if (IS_LOW_SURROGATE(wp))
            {
                if (!HAS_HIGH_SURROGATE)
                    break;

                wchar_t hs = LAST_HIGH_SURROGATE;
                wchar_t ls = (wchar_t)wp;

                HAS_HIGH_SURROGATE = false;
                if (!IS_SURROGATE_PAIR(hs, ls))
                    break;

                // Credit: following 4 lines taken from http://www.unicode.org/faq//utf_bom.html
                uint32_t X = (uint32_t(hs) & ((1 << 6) - 1)) << 10 | uint32_t(ls) & ((1 << 10) - 1);
                uint32_t W = (uint32_t(hs) >> 6) & ((1 << 5) - 1);
                uint32_t U = W + 1;
                uint32_t C = U << 16 | X;

                char_code = C;
            }
            else
            {
                char_code = (uint32_t)wp;
            }

            if (display)
            {
                Event::ReceivedCharacter event;
                event.codepoint = char_code;
                display->event_queue.push(event);
            }
            break;
        }
        
        // Receive/Lose focus
        case WM_KILLFOCUS:
        case WM_SETFOCUS:
        {
            if (display)
            {
                Event::Focused event;
                event.state = msg == WM_SETFOCUS;
                display->event_queue.push(event);
            }
            break;
        }

        // Keyboard input ;) the VK mapping is a doozy
        case WM_KEYDOWN:
        case WM_KEYUP:
        {
            uint8_t scancode = uint8_t((lp >> 16) & 0xFF);
            bool extended = (lp & 0x01000000) != 0;
            int vk = 0;
            switch ((int)wp)
            {
                case VK_SHIFT:
                    vk = MapVirtualKeyA(scancode, MAPVK_VSC_TO_VK_EX);
                    break;
                case VK_CONTROL:
                    if (extended)
                        vk = VK_RCONTROL;
                    else
                        vk = VK_LCONTROL;
                    break;
                case VK_MENU:
                    if (extended)
                        vk = VK_RMENU;
                    else
                        vk = VK_LMENU;
                    break;
                default:
                    vk = (int)wp;
                    break;
            }

            if (display)
            {
                Event::KeyboardInput event;
                event.type = EventType::KeyboardInput;
                event.scanCode = scancode;
                event.virtualKey = MapVK(vk, event.hasVK);
                if (msg == WM_KEYDOWN)
                    event.state = ElementState::Pressed;
                else
                    event.state = ElementState::Released;
                display->event_queue.push(event);
            }
        }

        // Whenever the mouse moves ;)
        case WM_MOUSEMOVE:
        {
            if (display)
            {
                Event::MouseMoved event;
                event.type = EventType::MouseMoved;
                event.x = (short)LOWORD(lp);
                event.y = (short)HIWORD(lp);
                display->event_queue.push(event);
            }
            break;
        }

        // Mouse wheel scrolling
        case WM_MOUSEWHEEL:
        {
            auto raw_delta = GET_WHEEL_DELTA_WPARAM(wp);
            auto delta = float(raw_delta) / WHEEL_DELTA;

            if (display)
            {
                Event::MouseWheel event;
                event.dx = 0;
                event.dy = delta;
                event.isPixel = false;
                event.touchPhase = TouchPhase::Moved;
                display->event_queue.push(event);
            }
            break;
        }

        // Mouse button events. SO MANY! XD
        case WM_LBUTTONDOWN: case WM_LBUTTONUP:
        case WM_RBUTTONDOWN: case WM_RBUTTONUP:
        case WM_MBUTTONDOWN: case WM_MBUTTONUP:
        case WM_XBUTTONDOWN: case WM_XBUTTONUP:
        {
            if (display)
            {
                Event::MouseInput event;
                event.type = EventType::MouseInput;
                GetEventButton(msg, wp, &event.button, &event.state);
                display->event_queue.push(event);
            }
            break;
        }

        // Touch events :3
        case WM_TOUCH:
        {
            UINT num_inputs = LOWORD(wp);
            auto inputs = std::vector<TOUCHINPUT>(num_inputs);

            if (display && GetTouchInputInfo((HTOUCHINPUT)lp, num_inputs, inputs.data(), sizeof(TOUCHINPUT)))
            {
                for (auto &input : inputs)
                {
                    Event::Touch event;
                    event.x = input.x * 100.f;
                    event.y = input.y * 100.f;
                    event.id = input.dwID;

                    // Determine the phase
                    if (input.dwFlags & TOUCHEVENTF_DOWN)
                        event.phase = TouchPhase::Started;
                    else if (input.dwFlags & TOUCHEVENTF_UP)
                        event.phase = TouchPhase::Ended;
                    else if (input.dwFlags & TOUCHEVENTF_MOVE)
                        event.phase = TouchPhase::Moved;
                    else
                        continue;

                    // Determine the source
                    if (input.dwFlags & TOUCHEVENTF_PALM)
                        event.source = TouchSource::Palm;
                    else if (input.dwFlags & TOUCHEVENTF_PEN)
                        event.source = TouchSource::Pen;
                    else
                        event.source = TouchSource::Finger;

                    display->event_queue.push(event);
                }

                CloseTouchInputHandle((HTOUCHINPUT)lp);
                return 0;
            }
            break;
        }
        
        default:
        {
            break;
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

    DragAcceptFiles(hwnd, true);

    RECT rect;
    GetClientRect(hwnd, &rect);
    width = uint32_t(rect.right - rect.left);
    height = uint32_t(rect.bottom - rect.top);
}

void HwndDisplay::InitializeSwap()
{
    // TODO: Depth buffer? Do I want to support out-of-order drawing?
    render_target.Release();
    back_buffer.Release();
    swap_chain.Release();

    HRESULT hr;
    DXGI_SWAP_CHAIN_DESC1 desc;
    desc.Width = width;
    desc.Height = height;
    desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    desc.Stereo = false;
    desc.SampleDesc = { 1, 0 };
    desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    desc.BufferCount = 16;
    desc.Scaling = DXGI_SCALING_STRETCH;
    desc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
    desc.AlphaMode = DXGI_ALPHA_MODE_IGNORE;
    desc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

    DXGI_SWAP_CHAIN_FULLSCREEN_DESC full_desc;
    full_desc.RefreshRate = { 60, 1 };
    full_desc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
    full_desc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
    full_desc.Windowed = true;

    auto inst = GetInst();
    hr = inst->dxgi_factory->CreateSwapChainForHwnd(
        device->device,
        hwnd,
        &desc,
        &full_desc,
        nullptr,
        &swap_chain
    );
    CheckHR(hr);

    hr = swap_chain->GetBuffer(0, IID_PPV_ARGS(&back_buffer));
    CheckHR(hr);

    D3D11_RENDER_TARGET_VIEW_DESC rtv_desc;
    rtv_desc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
    rtv_desc.Format = desc.Format;
    rtv_desc.Texture2D.MipSlice = 0;

    hr = device->device->CreateRenderTargetView(
        back_buffer,
        &rtv_desc,
        &render_target
    );
    CheckHR(hr);
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

static VirtualKeyCode MapVK(int vk, bool &has)
{
    has = true;
    if (vk >= 'A' && vk <= 'Z')
        return (VirtualKeyCode)(((int)VirtualKeyCode::A) + (vk - 'A'));
    if (vk >= '0' && vk <= '9')
        return (VirtualKeyCode)(((int)VirtualKeyCode::Key0) + (vk - '0'));
    if (vk >= VK_F1 && vk <= VK_F15)
        return (VirtualKeyCode)(((int)VirtualKeyCode::F1) + (vk - VK_F1));
    if (vk >= VK_NUMPAD0 && vk <= VK_NUMPAD9)
        return (VirtualKeyCode)(((int)VirtualKeyCode::Numpad0) + (vk - VK_NUMPAD0));

    switch (vk)
    {
        case VK_ESCAPE:
            return VirtualKeyCode::Escape;

        case VK_SNAPSHOT:
            return VirtualKeyCode::Snapshot;
        case VK_SCROLL:
            return VirtualKeyCode::Scroll;
        case VK_PAUSE:
            return VirtualKeyCode::Pause;

        case VK_INSERT:
            return VirtualKeyCode::Insert;
        case VK_HOME:
            return VirtualKeyCode::Home;
        case VK_DELETE:
            return VirtualKeyCode::Delete;
        case VK_END:
            return VirtualKeyCode::End;
        case VK_NEXT:
            return VirtualKeyCode::PageDown;
        case VK_PRIOR:
            return VirtualKeyCode::PageUp;

        case VK_LEFT:
            return VirtualKeyCode::Left;
        case VK_UP:
            return VirtualKeyCode::Up;
        case VK_RIGHT:
            return VirtualKeyCode::Right;
        case VK_DOWN:
            return VirtualKeyCode::Down;

        case VK_BACK:
            return VirtualKeyCode::Back;
        case VK_RETURN:
            return VirtualKeyCode::Return;
        case VK_SPACE:
            return VirtualKeyCode::Space;

        case VK_NUMLOCK:
            return VirtualKeyCode::Numlock;

        case VK_ADD:
            return VirtualKeyCode::Add;
        case VK_OEM_7:
            return VirtualKeyCode::Apostrophe;
        case VK_APPS:
            return VirtualKeyCode::Apps;
        case VK_OEM_102:
            return VirtualKeyCode::Backslash;
        case VK_CAPITAL:
            return VirtualKeyCode::Capital;
        case VK_OEM_1:
            return VirtualKeyCode::Colon;
        case VK_OEM_COMMA:
            return VirtualKeyCode::Comma;
        case VK_CONVERT:
            return VirtualKeyCode::Convert;
        case VK_DECIMAL:
            return VirtualKeyCode::Decimal;
        case VK_DIVIDE:
            return VirtualKeyCode::Divide;
        case VK_OEM_PLUS:
            return VirtualKeyCode::Equals;
        case VK_OEM_3:
            return VirtualKeyCode::Grave;
        case VK_KANA:
            return VirtualKeyCode::Kana;
        case VK_KANJI:
            return VirtualKeyCode::Kanji;
        case VK_LCONTROL:
            return VirtualKeyCode::LControl;
        case VK_LMENU:
            return VirtualKeyCode::LMenu;
        case VK_LSHIFT:
            return VirtualKeyCode::LShift;
        case VK_LWIN:
            return VirtualKeyCode::LWin;
        case VK_LAUNCH_MAIL:
            return VirtualKeyCode::Mail;
        case VK_LAUNCH_MEDIA_SELECT:
            return VirtualKeyCode::MediaSelect;
        case VK_MEDIA_STOP:
            return VirtualKeyCode::MediaStop;
        case VK_OEM_MINUS:
            return VirtualKeyCode::Minus;
        case VK_MULTIPLY:
            return VirtualKeyCode::Multiply;
        case VK_VOLUME_MUTE:
            return VirtualKeyCode::Mute;
        case VK_BROWSER_FORWARD:
            return VirtualKeyCode::NavigateForward;
        case VK_BROWSER_BACK:
            return VirtualKeyCode::NavigateBackward;
        case VK_MEDIA_NEXT_TRACK:
            return VirtualKeyCode::NextTrack;
        case VK_NONCONVERT:
            return VirtualKeyCode::NoConvert;
        case VK_OEM_PERIOD:
            return VirtualKeyCode::Period;
        case VK_MEDIA_PLAY_PAUSE:
            return VirtualKeyCode::PlayPause;
        case VK_MEDIA_PREV_TRACK:
            return VirtualKeyCode::PrevTrack;
        case VK_RCONTROL:
            return VirtualKeyCode::RControl;
        case VK_RMENU:
            return VirtualKeyCode::RMenu;
        case VK_RSHIFT:
            return VirtualKeyCode::RShift;
        case VK_RWIN:
            return VirtualKeyCode::RWin;
        case VK_OEM_2:
            return VirtualKeyCode::Slash;
        case VK_SLEEP:
            return VirtualKeyCode::Sleep;
        case VK_BROWSER_STOP:
            return VirtualKeyCode::Stop;
        case VK_SUBTRACT:
            return VirtualKeyCode::Subtract;
        case VK_TAB:
            return VirtualKeyCode::Tab;
        case VK_VOLUME_DOWN:
            return VirtualKeyCode::VolumeDown;
        case VK_VOLUME_UP:
            return VirtualKeyCode::VolumeUp;
        case VK_BROWSER_FAVORITES:
            return VirtualKeyCode::WebFavorites;
        case VK_BROWSER_HOME:
            return VirtualKeyCode::WebHome;
        case VK_BROWSER_REFRESH:
            return VirtualKeyCode::WebRefresh;
        case VK_BROWSER_SEARCH:
            return VirtualKeyCode::WebSearch;

        default:
            has = false;
            return (VirtualKeyCode)0;
    }
}
