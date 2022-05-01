#include <Components/Window.h>
#if __has_include(<windows.h>)
#    define WINDOWS_MESG_HANDLE
#    include "Windows/Window.hpp"
#    undef WINDOWS_MESG_HANDLE
#endif

losResult losCreateWindow(losWindow * ptr, losWindowInfo & info)
{
#if __has_include(<windows.h>)
    return winCreateWindow(ptr, info);
#endif
}

losResult losCreateKeyboard(losWindow win)
{
#if __has_include(<windows.h>)
    return winCreateKeyboard(win);
#endif
}

losResult losCreateMouse(losWindow win)
{
#if __has_include(<windows.h>)
    return winCreateMouse(win);
#endif
}

losResult losCreateTouch(losWindow win)
{
#if __has_include(<windows.h>)
    return winCreateTouch(win);
#endif
}

losResult losUpdateWindow(losWindow win)
{
#if __has_include(<windows.h>)
    return winUpdateWindow(win);
#endif
}

losResult losRequestClose(losWindow win)
{
#if __has_include(<windows.h>)
    return winRequestClose(win);
#endif
}

bool losIsKeyDown(losWindow win, losKeyboardButton button)
{
#if __has_include(<windows.h>)
    return winIsKeyDown(win,button);
#endif
}

bool losIsMouseDown(losWindow win, losMouseButton button)
{
#if __has_include(<windows.h>)
    return winIsMouseDown(win, button);
#endif
}

losPostition losRequestMousePosition(losWindow win)
{
#if __has_include(<windows.h>)
    return winRequestMousePosition(win);
#endif
}

losPostition losIsBeingPressed(losWindow win)
{
#if __has_include(<windows.h>)
    return winIsBeingPressed(win);
#endif
}

losResult losDestoryKeyboard(losWindow win)
{
#if __has_include(<windows.h>)
    return winDestoryKeyboard(win);
#endif
}

losResult losDestoryMouse(losWindow win)
{
#if __has_include(<windows.h>)
    return winDestoryMouse(win);
#endif
}

losResult losDestoryTouch(losWindow win)
{
#if __has_include(<windows.h>)
    return winDestoryTouch(win);
#endif
}

losResult losDestoryWindow(losWindow win)
{
#if __has_include(<windows.h>)
    return winDestoryWindow(win);
#endif
}