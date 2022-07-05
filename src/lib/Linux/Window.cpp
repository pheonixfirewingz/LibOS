#include "../Cmake.h"
#if CMAKE_SYSTEM_NUMBER == 0
#include "Window.h"
#include "Components/Window.h"
#include "xdg-shell-client-protocol.h"
#include <Components/Defines.h>
#include <cstdint>
#include <cstdio>
#include <cstring>

const xdg_surface_listener surface_listener = {[](void *, struct xdg_surface *, uint32_t) {}};

const xdg_toplevel_listener toplevel_listener = {[](void *, xdg_toplevel *, int32_t, int32_t, struct wl_array *) {},
                                                 [](void * data, xdg_toplevel *) 
                                                 {
                                                     losRequestClose(static_cast<losWindow>(data));
                                                 },
                                                 [](void *, xdg_toplevel *, int32_t, int32_t) {}};

const wl_keyboard_listener keyboard_listener = {
    [](void *, wl_keyboard *, uint32_t, int, uint32_t) {},
    [](void *, wl_keyboard *, uint32_t, wl_surface *, wl_array *) {},
    [](void *, wl_keyboard *, uint32_t, wl_surface *) {},
    [](void *, wl_keyboard *, uint32_t, uint32_t, uint32_t, uint32_t) {},
    [](void *, wl_keyboard *, uint32_t, uint32_t, uint32_t, uint32_t, uint32_t) {},
    [](void *, wl_keyboard *, int32_t, int32_t) {}};

const wl_pointer_listener pointer_listener = {
    [](void *, wl_pointer *, uint32_t, wl_surface *, wl_fixed_t, wl_fixed_t) {},
    [](void *, wl_pointer *, uint32_t, wl_surface *) {},
    [](void *, wl_pointer *, uint32_t, wl_fixed_t, wl_fixed_t) {},
    [](void *, wl_pointer *, uint32_t, uint32_t, uint32_t, uint32_t) {},
    [](void *, wl_pointer *, uint32_t, uint32_t, wl_fixed_t) {},
    [](void *, wl_pointer *) {},
    [](void *, wl_pointer *, uint32_t) {},
    [](void *, wl_pointer *, uint32_t, uint32_t) {},
    [](void *, wl_pointer *, uint32_t, int32_t) {}};

const wl_seat_listener seat_listener = {[](void *data, wl_seat *seat, uint32_t caps) {
                                            losWindow win = (losWindow)data;
                                            if ((caps & WL_SEAT_CAPABILITY_POINTER) && !win->pointer)
                                            {
                                                win->pointer = wl_seat_get_pointer(seat);
                                                wl_pointer_add_listener(win->pointer, &pointer_listener, win);
                                            }
                                            else if (!(caps & WL_SEAT_CAPABILITY_POINTER) && win->pointer)
                                            {
                                                wl_pointer_destroy(win->pointer);
                                                win->pointer = NULL;
                                            }

                                            if (caps & WL_SEAT_CAPABILITY_KEYBOARD)
                                            {
                                                win->keyboard = wl_seat_get_keyboard(seat);
                                                wl_keyboard_add_listener(win->keyboard, &keyboard_listener, win);
                                            }
                                            else if (!(caps & WL_SEAT_CAPABILITY_KEYBOARD))
                                            {
                                                wl_keyboard_destroy(win->keyboard);
                                                win->keyboard = NULL;
                                            }
                                        },
                                        [](void *, wl_seat *, const char *) {}};

static const xdg_wm_base_listener xdg_wm_base_listener = {
    [](void *, struct xdg_wm_base *xdg_wm_base, uint32_t serial) { xdg_wm_base_pong(xdg_wm_base, serial); }};

const wl_registry_listener registry_listener = {
    [](void *data, wl_registry *registry, uint32_t id, const char *interface, uint32_t) {
        losWindow win = (losWindow)data;

        if (strcmp(interface, "wl_compositor") == 0)
        {
            win->compositor = (wl_compositor *)wl_registry_bind(registry, id, &wl_compositor_interface, 1);
        }
        else if (strcmp(interface, "xdg_wm_base") == 0)
        {
            win->xdg_shell = (xdg_wm_base *)wl_registry_bind(registry, id, &xdg_wm_base_interface, 3);
            xdg_wm_base_add_listener(win->xdg_shell, &xdg_wm_base_listener, win);
        }
        else if (strcmp(interface, "wl_seat") == 0)
        {
            win->seat = (wl_seat *)wl_registry_bind(registry, id, &wl_seat_interface, 1);
            wl_seat_add_listener(win->seat, &seat_listener, win);
        }
    },
    [](void *, wl_registry *, uint32_t) {}};

bool hasWindowClosed() noexcept
{
    return false;
}

losResult losCreateWindow(losWindow *window, losWindowInfo &info)
{
    if (!(*window))
        return LOS_ERROR_HANDLE_IN_USE;

    (*window) = new losWindow_T();
    (*window)->display = wl_display_connect(NULL);
    if (!(*window)->display)
        return LOS_ERROR_COULD_NOT_INIT;

    (*window)->registry = wl_display_get_registry((*window)->display);
    if (!(*window)->registry)
        return LOS_ERROR_COULD_NOT_INIT;

    wl_registry_add_listener((*window)->registry, &registry_listener, (*window));
    wl_display_roundtrip((*window)->display);

    if (!(*window)->compositor)
        return LOS_ERROR_COULD_NOT_INIT;

    if (!(*window)->xdg_shell)
        return LOS_ERROR_COULD_NOT_INIT;

    (*window)->surface = wl_compositor_create_surface((*window)->compositor);
    if (!(*window)->surface)
        return LOS_ERROR_COULD_NOT_INIT;

    (*window)->xdg_win_surface = xdg_wm_base_get_xdg_surface((*window)->xdg_shell, (*window)->surface);
    if (!(*window)->xdg_win_surface)
        return LOS_ERROR_COULD_NOT_INIT;

    xdg_surface_add_listener((*window)->xdg_win_surface, &surface_listener, (*window));

    (*window)->xdg_top_level = xdg_surface_get_toplevel((*window)->xdg_win_surface);
    xdg_toplevel_add_listener((*window)->xdg_top_level,&toplevel_listener, (*window));
    xdg_toplevel_set_app_id((*window)->xdg_top_level, "LibOSWindowClass");
    xdg_toplevel_set_title((*window)->xdg_top_level, info.title);
    wl_surface_commit((*window)->surface);

    wl_region *region = wl_compositor_create_region((*window)->compositor);
    wl_region_add(region, 0, 0, info.window_size.width, info.window_size.height);
    wl_surface_set_opaque_region((*window)->surface, region);
    wl_region_destroy(region);
    
    wl_display_flush((*window)->display);

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

losResult losUpdateWindow(losWindow window)
{
    if (wl_display_dispatch(window->display) == -1)
        return LOS_WINDOW_CLOSE;
    return LOS_SUCCESS;
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

losPosition losRequestMousePosition(losWindow)
{
    return {0, 0};
}

losPosition losRequestMouseWheelDelta(losWindow)
{
    return {0, 0};
}

losPosition losIsBeingPressed(losWindow)
{
    return {0, 0};
}

losResult losDestroyKeyboard(losWindow window)
{
    if (window->keyboard)
        wl_keyboard_destroy(window->keyboard);
    return LOS_SUCCESS;
}

losResult losDestroyMouse(losWindow window)
{
    if (window->pointer)
        wl_pointer_destroy(window->pointer);
    if (window->seat)
        wl_seat_destroy(window->seat);
    return LOS_SUCCESS;
}

losResult losDestroyTouch(losWindow window)
{
    return losDestroyMouse(window);
}

losResult losDestroyWindow(losWindow window)
{
    if (window->xdg_win_surface)
        xdg_surface_destroy(window->xdg_win_surface);
    if (window->surface)
        wl_surface_destroy(window->surface);
    if (window->xdg_shell)
        xdg_wm_base_destroy(window->xdg_shell);
    if (window->compositor)
        wl_compositor_destroy(window->compositor);
    if (window->registry)
        wl_registry_destroy(window->registry);
    if (window->display)
        wl_display_disconnect(window->display);
    return LOS_SUCCESS;
}
/* Generated by wayland-scanner 1.20.0 */

/*
 * Copyright © 2008-2013 Kristian Høgsberg
 * Copyright © 2013      Rafael Antognolli
 * Copyright © 2013      Jasper St. Pierre
 * Copyright © 2010-2013 Intel Corporation
 * Copyright © 2015-2017 Samsung Electronics Co., Ltd
 * Copyright © 2015-2017 Red Hat Inc.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice (including the next
 * paragraph) shall be included in all copies or substantial portions of the
 * Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL
 * THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 * DEALINGS IN THE SOFTWARE.
 */

#include <wayland-util.h>

#ifndef __has_attribute
#    define __has_attribute(x) 0 /* Compatibility with non-clang compilers. */
#endif

#define WL_PRIVATE

extern const struct wl_interface wl_output_interface;
extern const struct wl_interface wl_seat_interface;
extern const struct wl_interface wl_surface_interface;
extern const struct wl_interface xdg_popup_interface;
extern const struct wl_interface xdg_positioner_interface;
extern const struct wl_interface xdg_surface_interface;
extern const struct wl_interface xdg_toplevel_interface;

static const struct wl_interface *xdg_shell_types[] = {
    NULL,
    NULL,
    NULL,
    NULL,
    &xdg_positioner_interface,
    &xdg_surface_interface,
    &wl_surface_interface,
    &xdg_toplevel_interface,
    &xdg_popup_interface,
    &xdg_surface_interface,
    &xdg_positioner_interface,
    &xdg_toplevel_interface,
    &wl_seat_interface,
    NULL,
    NULL,
    NULL,
    &wl_seat_interface,
    NULL,
    &wl_seat_interface,
    NULL,
    NULL,
    &wl_output_interface,
    &wl_seat_interface,
    NULL,
    &xdg_positioner_interface,
    NULL,
};

static const struct wl_message xdg_wm_base_requests[] = {
    {"destroy", "", xdg_shell_types + 0},
    {"create_positioner", "n", xdg_shell_types + 4},
    {"get_xdg_surface", "no", xdg_shell_types + 5},
    {"pong", "u", xdg_shell_types + 0},
};

static const struct wl_message xdg_wm_base_events[] = {
    {"ping", "u", xdg_shell_types + 0},
};

WL_PRIVATE const struct wl_interface xdg_wm_base_interface = {
    "xdg_wm_base", 4, 4, xdg_wm_base_requests, 1, xdg_wm_base_events,
};

static const struct wl_message xdg_positioner_requests[] = {
    {"destroy", "", xdg_shell_types + 0},
    {"set_size", "ii", xdg_shell_types + 0},
    {"set_anchor_rect", "iiii", xdg_shell_types + 0},
    {"set_anchor", "u", xdg_shell_types + 0},
    {"set_gravity", "u", xdg_shell_types + 0},
    {"set_constraint_adjustment", "u", xdg_shell_types + 0},
    {"set_offset", "ii", xdg_shell_types + 0},
    {"set_reactive", "3", xdg_shell_types + 0},
    {"set_parent_size", "3ii", xdg_shell_types + 0},
    {"set_parent_configure", "3u", xdg_shell_types + 0},
};

WL_PRIVATE const struct wl_interface xdg_positioner_interface = {
    "xdg_positioner", 4, 10, xdg_positioner_requests, 0, NULL,
};

static const struct wl_message xdg_surface_requests[] = {
    {"destroy", "", xdg_shell_types + 0},        {"get_toplevel", "n", xdg_shell_types + 7},
    {"get_popup", "n?oo", xdg_shell_types + 8},  {"set_window_geometry", "iiii", xdg_shell_types + 0},
    {"ack_configure", "u", xdg_shell_types + 0},
};

static const struct wl_message xdg_surface_events[] = {
    {"configure", "u", xdg_shell_types + 0},
};

WL_PRIVATE const struct wl_interface xdg_surface_interface = {
    "xdg_surface", 4, 5, xdg_surface_requests, 1, xdg_surface_events,
};

static const struct wl_message xdg_toplevel_requests[] = {
    {"destroy", "", xdg_shell_types + 0},
    {"set_parent", "?o", xdg_shell_types + 11},
    {"set_title", "s", xdg_shell_types + 0},
    {"set_app_id", "s", xdg_shell_types + 0},
    {"show_window_menu", "ouii", xdg_shell_types + 12},
    {"move", "ou", xdg_shell_types + 16},
    {"resize", "ouu", xdg_shell_types + 18},
    {"set_max_size", "ii", xdg_shell_types + 0},
    {"set_min_size", "ii", xdg_shell_types + 0},
    {"set_maximized", "", xdg_shell_types + 0},
    {"unset_maximized", "", xdg_shell_types + 0},
    {"set_fullscreen", "?o", xdg_shell_types + 21},
    {"unset_fullscreen", "", xdg_shell_types + 0},
    {"set_minimized", "", xdg_shell_types + 0},
};

static const struct wl_message xdg_toplevel_events[] = {
    {"configure", "iia", xdg_shell_types + 0},
    {"close", "", xdg_shell_types + 0},
    {"configure_bounds", "4ii", xdg_shell_types + 0},
};

WL_PRIVATE const struct wl_interface xdg_toplevel_interface = {
    "xdg_toplevel", 4, 14, xdg_toplevel_requests, 3, xdg_toplevel_events,
};

static const struct wl_message xdg_popup_requests[] = {
    {"destroy", "", xdg_shell_types + 0},
    {"grab", "ou", xdg_shell_types + 22},
    {"reposition", "3ou", xdg_shell_types + 24},
};

static const struct wl_message xdg_popup_events[] = {
    {"configure", "iiii", xdg_shell_types + 0},
    {"popup_done", "", xdg_shell_types + 0},
    {"repositioned", "3u", xdg_shell_types + 0},
};

WL_PRIVATE const struct wl_interface xdg_popup_interface = {
    "xdg_popup", 4, 3, xdg_popup_requests, 3, xdg_popup_events,
};
#endif