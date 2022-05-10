#include "Components/Defines.h"
#include <Components/Window.h>
#include <wayland-client.h>

losResult losCreateWindow(losWindow *, losWindowInfo &)
{
    return LOS_ERROR_FEATURE_NOT_IMPLEMENTED;
}

losResult losCreateKeyboard(losWindow)
{
    return LOS_ERROR_FEATURE_NOT_IMPLEMENTED;
}

losResult losCreateMouse(losWindow)
{
    return LOS_ERROR_FEATURE_NOT_IMPLEMENTED;
}

losResult losCreateTouch(losWindow)
{
    return LOS_ERROR_FEATURE_NOT_IMPLEMENTED;
}

losResult losUpdateWindow(losWindow)
{
    return LOS_ERROR_FEATURE_NOT_IMPLEMENTED;
}

bool losIsKeyDown(losWindow, losKeyboardButton)
{
    return false;
}

bool losIsMouseDown(losWindow, losMouseButton)
{
    return false;
}

losResult losRequestClose(losWindow)
{
    return LOS_ERROR_FEATURE_NOT_IMPLEMENTED;
}

losPostition losRequestMousePosition(losWindow)
{
    return {0, 0};
}

losPostition losRequestMouseWheelDelta(losWindow)
{
    return {0, 0};
}

losPostition losIsBeingPressed(losWindow)
{
    return {0, 0};
}

losResult losDestoryKeyboard(losWindow)
{
    return LOS_ERROR_FEATURE_NOT_IMPLEMENTED;
}

losResult losDestoryMouse(losWindow)
{
    return LOS_ERROR_FEATURE_NOT_IMPLEMENTED;
}

losResult losDestoryTouch(losWindow)
{
    return LOS_ERROR_FEATURE_NOT_IMPLEMENTED;
}

losResult losDestoryWindow(losWindow)
{
    return LOS_ERROR_FEATURE_NOT_IMPLEMENTED;
}