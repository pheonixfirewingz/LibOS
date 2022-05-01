#pragma once
#include "windows_link.h"
#include <cstdlib>

struct losWindow_T
{
    HWND win_hand{nullptr};
};
LRESULT CALLBACK WndProc(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam);
#ifdef WINDOWS_MESG_HANDLE
LRESULT CALLBACK WndProc(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam)
{
    switch (Msg)
    {
    case WM_CLOSE:
        std::exit(0);
        break;
    case WM_DEVICECHANGE:
        break;
    case WM_SIZE:
        break;
    case WM_KEYDOWN:
        break;
    case WM_KEYUP:
        break;
    case WM_LBUTTONDOWN:
        break;
    case WM_LBUTTONUP:
        break;
    case WM_RBUTTONDOWN:
        break;
    case WM_RBUTTONUP:
        break;
    case WM_MBUTTONDOWN:
        break;
    case WM_MBUTTONUP:
        break;
    case WM_MOUSEHOVER:
        break;
    case WM_MOUSELEAVE:
        break;
    case WM_MOUSEWHEEL:
        break;
    case WM_MOUSEMOVE:
        break;
    default:
        break;
    }
    return DefWindowProc(hWnd, Msg, wParam, lParam);
}
#endif

losResult winCreateWindow(losWindow *win, losWindowInfo &info)
{
    *win = new losWindow_T();
    WNDCLASSEX wc;

    ZeroMemory(&wc, sizeof(WNDCLASSEX));
    wc.cbSize = sizeof(WNDCLASSEX);
    wc.hbrBackground = reinterpret_cast<HBRUSH>(COLOR_WINDOW);
    wc.lpfnWndProc = WndProc;
    wc.hInstance = GetModuleHandle(nullptr);
    wc.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wc.lpszClassName = "LibOSWindowClass";
    RegisterClassEx(&wc);

    (*win)->win_hand =
        CreateWindowEx(0, "LunaLuxWindowClass", info.title, WS_OVERLAPPEDWINDOW, 0, 0, info.windowSize.width,
                       info.windowSize.height, nullptr, nullptr, GetModuleHandle(nullptr), nullptr);

    ShowWindow((*win)->win_hand, SW_SHOWDEFAULT);
    MoveWindow((*win)->win_hand, GetSystemMetrics(SM_CXSCREEN) / 12, GetSystemMetrics(SM_CYSCREEN) / 12,
               info.windowSize.width, info.windowSize.height, true);
    return LOS_SUCCESS;
}

losResult winCreateKeyboard(losWindow)
{
    return LOS_SUCCESS;
}

losResult winCreateMouse(losWindow)
{
    return LOS_SUCCESS;
}

losResult winCreateTouch(losWindow)
{
    return LOS_SUCCESS;
}

losResult winUpdateWindow(losWindow win)
{
    MSG msg;
    PeekMessage(&msg, win->win_hand, 0, 0, PM_REMOVE);
    TranslateMessage(&msg);
    DispatchMessage(&msg);
    return LOS_SUCCESS;
}

losResult winRequestClose(losWindow)
{
    return LOS_SUCCESS;
}

bool winIsKeyDown(losWindow, losKeyboardButton)
{
    return false;
}

bool winIsMouseDown(losWindow, losMouseButton)
{
    return false;
}

losPostition winRequestMousePosition(losWindow)
{
    return {0, 0};
}

losPostition winIsBeingPressed(losWindow)
{
    return {0, 0};
}

losResult winDestoryKeyboard(losWindow)
{
    return LOS_SUCCESS;
}

losResult winDestoryMouse(losWindow)
{
    return LOS_SUCCESS;
}

losResult winDestoryTouch(losWindow)
{
    return LOS_SUCCESS;
}

losResult winDestoryWindow(losWindow win)
{
    DestroyWindow(win->win_hand);
    UnregisterClassW(L"LibOSWindowClass", GetModuleHandle(nullptr));
    return LOS_SUCCESS;
}