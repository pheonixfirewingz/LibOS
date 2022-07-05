#pragma once
#include <Components/Window.h>
#include "../Callbacks.h"
#include "xdg-shell-client-protocol.h"

struct losWindow_T
{
    wl_display *display;
    wl_registry *registry;
    wl_compositor *compositor;
    xdg_wm_base* xdg_shell;
    wl_surface *surface;
    xdg_surface* xdg_win_surface;
    xdg_toplevel *xdg_top_level;
    wl_seat *seat;
    wl_pointer *pointer;
    wl_keyboard *keyboard;
    ResizeCallbackFunction resize_callback;
    requestObjectCallback object_callback;
};