#include "Win32.hpp"
// LIBOS LICENCE
//
// GNU Lesser General Public License Version 3.0
//
// Copyright Luke Shore (c) 2020, 2023
Win32Window::Win32Window(const std::string title, const losSize window_size) noexcept
{
    WNDCLASSEX wc;

    ZeroMemory(&wc, sizeof(WNDCLASSEX));
    wc.cbSize = sizeof(WNDCLASSEX);
    wc.hbrBackground = reinterpret_cast<HBRUSH>(COLOR_WINDOW);
    wc.lpfnWndProc = wndProc;
    wc.hInstance = GetModuleHandle(nullptr);
    wc.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wc.lpszClassName = static_cast<LPCSTR>("LibOSWindowClass");
    RegisterClassEx(&wc);
#pragma warning(disable : 4244)
    win_hand = CreateWindowEx(0, static_cast<LPCSTR>("LibOSWindowClass"), title.c_str(), WS_OVERLAPPEDWINDOW, 0, 0, 
                        window_size.length_one, window_size.length_two,nullptr, nullptr, GetModuleHandle(nullptr), nullptr);
    ShowWindow(win_hand, SW_SHOWDEFAULT);
    MoveWindow(win_hand, GetSystemMetrics(SM_CXSCREEN) / 12, GetSystemMetrics(SM_CYSCREEN) / 12, window_size.length_one, window_size.length_two, true);
#pragma warning(default : 4244)
    SetWindowLongPtr(win_hand, GWLP_USERDATA, (LONG_PTR)this);
}

LRESULT CALLBACK Win32Window::classWndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    switch (msg)
    {
    case WM_CLOSE:
        should_window_close = true;
        break;
    case WM_DEVICECHANGE: {
        ZeroMemory(&key_buttons[0], sizeof(key_buttons));
        ZeroMemory(&mouse_buttons[0], sizeof(mouse_buttons));
        break;
    }
    case WM_SIZING: {
        break;
    }
    case WM_KEYDOWN:
        key_buttons[wParam] = true;
        break;
    case WM_KEYUP:
        key_buttons[wParam] = false;
        break;
    case WM_RBUTTONDOWN:
        mouse_buttons[0] = true;
        break;
    case WM_RBUTTONUP:
        mouse_buttons[0] = false;
        break;
    case WM_MBUTTONDOWN:
        mouse_buttons[1] = true;
        break;
    case WM_MBUTTONUP:
        mouse_buttons[1] = false;
        break;
    case WM_LBUTTONDOWN:
        mouse_buttons[2] = true;
        break;
    case WM_LBUTTONUP:
        mouse_buttons[2] = false;
        break;
    case WM_MOUSEHOVER:
        mouse_inside_window = true;
        break;
    case WM_MOUSELEAVE:
        mouse_inside_window = false;
        break;
    case WM_MOUSEWHEEL:
    case WM_MOUSEHWHEEL: {
        if (mouse_inside_window)
        {
            if (msg == WM_MOUSEWHEEL)
                mouse_wheel_delta_y = (float64_t)GET_WHEEL_DELTA_WPARAM(wParam) / WHEEL_DELTA;
            else
                mouse_wheel_delta_x = (float64_t)GET_WHEEL_DELTA_WPARAM(wParam) / WHEEL_DELTA;
        }
        break;
    }
    case WM_MOUSEMOVE: {
        if (mouse_inside_window)
        {
            mouse_position_x = (lParam & 0xffff);
            mouse_position_y = ((lParam >> 16) & 0xffff);
        }
        break;
    }
    default:
        return DefWindowProc(hwnd, msg, wParam, lParam);
    }
    return TRUE;
}

bool Win32Window::hasWindowClosed() const noexcept
{
    return should_window_close;
}

losResult Win32Window::losUpdateWindow() noexcept
{
    if (!should_window_close)
    {
        MSG msg;
        PeekMessage(&msg, win_hand, 0, 0, PM_REMOVE);
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    else
        return LOS_WINDOW_CLOSE;
    return LOS_SUCCESS;
}

bool Win32Window::losIsKeyDown(losKeyboardButton key) const noexcept
{
    return key_buttons[window_key_look_up_table[key]];
}

bool Win32Window::losIsMouseDown(losMouseButton button) const noexcept
{
    return mouse_buttons[button];
}

losResult Win32Window::losRequestClose() noexcept
{
    should_window_close = true;
    losDestroyWindow();
    return LOS_SUCCESS;
}

losSize Win32Window::losRequestMousePosition() const noexcept
{
    return {mouse_position_x, mouse_position_y};
}

losSize Win32Window::losRequestMouseWheelDelta() const noexcept
{
    return {static_cast<int64_t>(mouse_wheel_delta_x), static_cast<int64_t>(mouse_wheel_delta_y)};
}

losSize Win32Window::losIsBeingPressed() const noexcept
{
    return {mouse_position_x, mouse_position_y};
}

void Win32Window::losDestroyWindow() noexcept
{
    DestroyWindow(win_hand);
    UnregisterClassW(L"LibOSWindowClass", GetModuleHandle(nullptr));
}

losSize * Win32Window::getWindowSize() noexcept
{
    return &win_size;
}

void* Win32Window::internalGet() noexcept
{
    return new losWindowWin32(win_hand);
}