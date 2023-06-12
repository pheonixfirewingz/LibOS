// LIBOS LICENCE
//
// GNU Lesser General Public License Version 3.0
//
// Copyright Luke Shore (c) 2020, 2023
#include "../Window/Wayland.hpp"
#include "../Window/Win32.hpp"
#include "../Window/WinRT.hpp"
#include "../Window/Xcb.hpp"
#include <libos/Window.h>
#include <new>
#include <stdexcept>
struct losWindow_T
{
    BaseWindow *window;
};

losResult losCreateWindow(losWindow *window, losWindowInfo info)
{
    (*window) = new (std::nothrow) losWindow_T();
    if ((*window) == NULL)
        return LOS_ERROR_COULD_NOT_INIT;

    if (info.title == NULL)
    {
        info.title = "Title Not Set";
        info.title_size = 14;
    }

#ifdef ON_WINDOWS
        (*window)->window = new (std::nothrow) Win32Window(info.title, info.window_size);
#elif defined(ON_UWP)
        (*window)->window = new (std::nothrow) WinRTWindow(info.title, info.window_size);
#elif defined(ON_LINUX) && defined(NO_XCB)
        (*window)->window = new (std::nothrow) WaylandWindow(info.title, info.window_size);
#elif defined(ON_LINUX) && !defined(NO_XCB)
        (*window)->window = new (std::nothrow) WaylandWindow(info.title, info.window_size);
        if ((*window)->window->hasFailed())
        {
            delete (*window)->window;
            (*window)->window = new (std::nothrow) XcbWindow(info.title, info.window_size);
        }
#else
        return LOS_ERROR_FEATURE_NOT_IMPLEMENTED;
#endif

    if ((*window)->window == NULL)
        return LOS_ERROR_COULD_NOT_INIT;
    if ((*window)->window->hasFailed())
        return LOS_ERROR_COULD_NOT_INIT;
    return LOS_SUCCESS;
}

losResult losUpdateWindow(losWindow window)
{
    return window->window->losUpdateWindow();
}

losSize losRequestWindowSize(losWindow window)
{
    return std::move(window->window->getWindowSize());
}

uint8_t losIsKeyDown(losWindow window, losKeyboardButton key)
{
    return (uint8_t)window->window->losIsKeyDown(key);
}

uint8_t losIsMouseDown(losWindow window, losMouseButton button)
{
    return (uint8_t)window->window->losIsMouseDown(button);
}

losResult losRequestClose(losWindow window)
{
    return window->window->losRequestClose();
}

losSize losRequestMousePosition(losWindow window)
{
    return window->window->losRequestMousePosition();
}

losSize losRequestMouseWheelDelta(losWindow window)
{
    return window->window->losRequestMouseWheelDelta();
}

losSize losIsBeingPressed(losWindow window)
{
    return window->window->losIsBeingPressed();
}

void losDestroyWindow(losWindow window)
{
    window->window->losDestroyWindow();
    delete window->window;
}

losUsedWindowAPI losWhatApiIsWindowUsed(losWindow window)
{
    return window->window->getType();
}

void *losGetWindowNativePointer(losWindow window)
{
    return window->window->internalGet();
}
