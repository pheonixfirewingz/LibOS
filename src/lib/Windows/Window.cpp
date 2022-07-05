#include "../Cmake.h"
#if CMAKE_SYSTEM_NUMBER == 1
#include "Window.h"
#include "windows_link.h"
#include <Components/Window.h>
#include <memory>

const static uint16 window_key_look_up_table[256] = {
    0x30,         0x31,          0x32,        0x33,        0x34,       0x35,       0x36,         0x37,
    0x38,         0x39,          0x41,        0x42,        0x43,       0x44,       0x45,         0x46,
    0x47,         0x48,          0x49,        0x4A,        0x4B,       0x4C,       0x4D,         0x4E,
    0x4F,         0x50,          0x51,        0x52,        0x53,       0x54,       0x55,         0x56,
    0x57,         0x58,          0x59,        0x5A,        VK_OEM_7,   VK_OEM_5,   VK_OEM_COMMA, VK_OEM_4,
    VK_OEM_MINUS, VK_OEM_PERIOD, VK_OEM_6,    VK_OEM_1,    VK_OEM_2,   VK_BACK,    VK_DELETE,    VK_END,
    VK_EXECUTE,   VK_ESCAPE,     VK_HOME,     VK_INSERT,   VK_PRIOR,   VK_NEXT,    VK_PAUSE,     VK_SPACE,
    VK_TAB,       VK_CAPITAL,    VK_NUMLOCK,  VK_SCROLL,   VK_F1,      VK_F2,      VK_F3,        VK_F4,
    VK_F5,        VK_F6,         VK_F7,       VK_F8,       VK_F9,      VK_F10,     VK_F11,       VK_F12,
    VK_F13,       VK_F14,        VK_F15,      VK_F16,      VK_F17,     VK_F18,     VK_F19,       VK_F20,
    VK_F21,       VK_F22,        VK_F23,      VK_F24,      VK_MENU,    VK_LMENU,   VK_RMENU,     VK_CONTROL,
    VK_LCONTROL,  VK_RCONTROL,   VK_SHIFT,    VK_LSHIFT,   VK_RSHIFT,  VK_LWIN,    VK_RWIN,      VK_SNAPSHOT,
    VK_DOWN,      VK_LEFT,       VK_RIGHT,    VK_UP,       VK_NUMPAD0, VK_NUMPAD1, VK_NUMPAD2,   VK_NUMPAD3,
    VK_NUMPAD4,   VK_NUMPAD5,    VK_NUMPAD6,  VK_NUMPAD7,  VK_NUMPAD8, VK_NUMPAD9, VK_ADD,       VK_DECIMAL,
    VK_DIVIDE,    VK_OEM_PLUS,   VK_MULTIPLY, VK_SUBTRACT,
};
static bool should_window_close = false;
static bool mouse_inside_window = true;
static bool key_buttons[256];
static bool mouse_buttons[3];
static uint64 mouse_position_x = 0;
static uint64 mouse_position_y = 0;
static float64 mouse_wheel_delta_x = 0;
static float64 mouse_wheel_delta_y = 0;
requestObjectCallback object_callback;
losWindow* window;

bool hasWindowClosed() noexcept
{
    return should_window_close;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam)
{
    switch (Msg)
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
        if((*window)->resize_callback)
            (*window)->resize_callback((refHandle)object_callback("refHandle"),MAKEPOINTS(lParam).x, MAKEPOINTS(lParam).y);
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
        if (Msg == WM_MOUSEWHEEL)
            mouse_wheel_delta_y = (float64)GET_WHEEL_DELTA_WPARAM(wParam) / WHEEL_DELTA;
        else
            mouse_wheel_delta_x = (float64)GET_WHEEL_DELTA_WPARAM(wParam) / WHEEL_DELTA;
        break;
    }
    case WM_MOUSEMOVE: {
        mouse_position_x = (lParam & 0xffff);
        mouse_position_y = ((lParam >> 16) & 0xffff);
        break;
    }
    default:
        break;
    }
    return DefWindowProc(hWnd, Msg, wParam, lParam);
}

losResult losCreateWindow(losWindow *win, losWindowInfo &info)
{
    if (!(*win))
        return LOS_ERROR_HANDLE_IN_USE;

    *win = new losWindow_T();
    WNDCLASSEX wc;

    window = win;


    object_callback = std::move(info.request_callback);

    ZeroMemory(&wc, sizeof(WNDCLASSEX));
    wc.cbSize = sizeof(WNDCLASSEX);
    wc.hbrBackground = reinterpret_cast<HBRUSH>(COLOR_WINDOW);
    wc.lpfnWndProc = WndProc;
    wc.hInstance = GetModuleHandle(nullptr);
    wc.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wc.lpszClassName = static_cast<LPCSTR>("LibOSWindowClass");
    RegisterClassEx(&wc);

    (*win)->win_hand =
        CreateWindowEx(0, static_cast<LPCSTR>("LibOSWindowClass"), info.title, WS_OVERLAPPEDWINDOW, 0, 0, info.window_size.height,
                       info.window_size.width, nullptr, nullptr, GetModuleHandle(nullptr), nullptr);

    ShowWindow((*win)->win_hand, SW_SHOWDEFAULT);
    MoveWindow((*win)->win_hand, GetSystemMetrics(SM_CXSCREEN) / 12, GetSystemMetrics(SM_CYSCREEN) / 12,
               info.window_size.height, info.window_size.width, true);
    return LOS_SUCCESS;
}

losResult losCreateKeyboard(losWindow)
{
    return LOS_SUCCESS;
}

losResult losCreateMouse(losWindow)
{
    return LOS_SUCCESS;
}

losResult losCreateTouch(losWindow)
{
    return LOS_SUCCESS;
}

losResult losUpdateWindow(losWindow win)
{
    if (!should_window_close)
    {
        MSG msg;
        PeekMessage(&msg, win->win_hand, 0, 0, PM_REMOVE);
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    else
        return LOS_WINDOW_CLOSE;
    return LOS_SUCCESS;
}

losResult losDestroyWindow(losWindow);
losResult losRequestClose(losWindow win)
{
    should_window_close = true;
    losDestroyWindow(win);
    return LOS_SUCCESS;
}

bool losIsKeyDown(losWindow ,losKeyboardButton key)
{
    return key_buttons[window_key_look_up_table[key]];
}

bool losIsMouseDown(losWindow ,losMouseButton button)
{
    return mouse_buttons[button];
}

losPosition losRequestMouseWheelDelta(losWindow)
{
    return {mouse_position_x, mouse_position_y};
}

losPosition losRequestMousePosition(losWindow)
{
    return {static_cast<uint64>(mouse_wheel_delta_x),static_cast<uint64>(mouse_wheel_delta_y)};
}

losPosition losIsBeingPressed(losWindow win)
{
    return losRequestMousePosition(win);
}


losResult losDestroyKeyboard(losWindow)
{
    return LOS_SUCCESS;
}

losResult losDestroyMouse(losWindow)
{
    return LOS_SUCCESS;
}

losResult losDestroyTouch(losWindow)
{
    return LOS_SUCCESS;
}

losResult losDestroyWindow(losWindow win)
{
    if (!win->active)
        return LOS_SUCCESS;

    DestroyWindow(win->win_hand);
    UnregisterClassW(L"LibOSWindowClass", GetModuleHandle(nullptr));
    win->active = false;
    return LOS_SUCCESS;
}
#endif