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
    return LOS_SUCCESS;
#endif
}

losResult losCreateMouse(losWindow win)
{
#if __has_include(<windows.h>)
    return LOS_SUCCESS;
#endif
}

losResult losCreateTouch(losWindow win)
{
#if __has_include(<windows.h>)
    return LOS_SUCCESS;
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

bool losIsKeyDown(losWindow, losKeyboardButton button)
{
#if __has_include(<windows.h>)
    return winIsKeyDown(button);
#endif
}

bool losIsMouseDown(losWindow, losMouseButton button)
{
#if __has_include(<windows.h>)
    return winIsMouseDown(button);
#endif
}

losPostition losRequestMousePosition(losWindow)
{
#if __has_include(<windows.h>)
    return winRequestMousePosition();
#endif
}

losPostition losRequestMouseWheelDelta(losWindow)
{
#if __has_include(<windows.h>)
    return winRequestMouseWheelDelta();
#endif
}

losPostition losIsBeingPressed(losWindow win)
{
#if __has_include(<windows.h>)
    return winIsBeingPressed();
#endif
}

losResult losDestoryKeyboard(losWindow win)
{
#if __has_include(<windows.h>)
    return LOS_SUCCESS;
#endif
}

losResult losDestoryMouse(losWindow win)
{
#if __has_include(<windows.h>)
    return LOS_SUCCESS;
#endif
}

losResult losDestoryTouch(losWindow win)
{
#if __has_include(<windows.h>)
    return LOS_SUCCESS;
#endif
}

losResult losDestoryWindow(losWindow win)
{
#if __has_include(<windows.h>)
    return winDestoryWindow(win);
#endif
}