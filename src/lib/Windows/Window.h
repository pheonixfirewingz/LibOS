#pragma once
#include "windows_link.h"
#include "../Callbacks.h"
#include "../Graphics/GraphicsWindow.h"
#include <Components/Window.h>
struct losWindow_T
{
    HWND win_hand{nullptr};
    bool active = true;
    ResizeCallbackFunction resize_callback;
};