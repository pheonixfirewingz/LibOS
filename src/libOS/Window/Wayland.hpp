#pragma once
// LIBOS LICENCE
//
// GNU Lesser General Public License Version 3.0
//
// Copyright Luke Shore (c) 2020, 2023
#include "../Interface/AbstractWindow.h"
#if __has_include(<wayland-client.h>) && defined(ON_LINUX)
#    include "xdg-shell-client-protocol.h"
#    include <extend_std/LookUpTable.h>
#    include <libdecor.h>
#    include <wayland-client.h>
#    include <xkbcommon/xkbcommon-keysyms.h>
#    include <xkbcommon/xkbcommon.h>
#endif

class WaylandWindow : public BaseWindow
{
  protected:
#if __has_include(<wayland-client.h>) && defined(ON_LINUX)
    std::atomic_bool should_close{false}, keys[256]{};
    int16_ts x = 0, y = 0;
    bool error = false;
    const std::ReadOnlyLookupTable<uint16_t, uint16_t> upper_versions = {
        {XKB_KEY_A, XKB_KEY_a}, {XKB_KEY_B, XKB_KEY_b},  {XKB_KEY_C, XKB_KEY_c}, {XKB_KEY_D, XKB_KEY_d},
        {XKB_KEY_E, XKB_KEY_e}, {XKB_KEY_F, XKB_KEY_f},  {XKB_KEY_G, XKB_KEY_g}, {XKB_KEY_H, XKB_KEY_h},
        {XKB_KEY_I, XKB_KEY_i}, {XKB_KEY_J, XKB_KEY_j},  {XKB_KEY_K, XKB_KEY_k}, {XKB_KEY_L, XKB_KEY_l},
        {XKB_KEY_M, XKB_KEY_m}, {XKB_KEY_N, XKB_KEY_n},  {XKB_KEY_O, XKB_KEY_o}, {XKB_KEY_P, XKB_KEY_p},
        {XKB_KEY_Q, XKB_KEY_q}, {XKB_KEY_R, XKB_KEY_r},  {XKB_KEY_S, XKB_KEY_s}, {XKB_KEY_T, XKB_KEY_t},
        {XKB_KEY_U, XKB_KEY_u}, {XKB_KEY_V, XKB_KEY_v},  {XKB_KEY_W, XKB_KEY_w}, {XKB_KEY_X, XKB_KEY_x},
        {XKB_KEY_Y, XKB_KEY_y}, { XKB_KEY_Z, XKB_KEY_z }};

    const uint16_t window_key_look_up_table[256] = {
        XKB_KEY_0,         XKB_KEY_1,          XKB_KEY_2,           XKB_KEY_3,           XKB_KEY_4,
        XKB_KEY_5,         XKB_KEY_6,          XKB_KEY_7,           XKB_KEY_8,           XKB_KEY_9,
        XKB_KEY_a,         XKB_KEY_b,          XKB_KEY_c,           XKB_KEY_d,           XKB_KEY_e,
        XKB_KEY_f,         XKB_KEY_g,          XKB_KEY_h,           XKB_KEY_i,           XKB_KEY_j,
        XKB_KEY_k,         XKB_KEY_l,          XKB_KEY_m,           XKB_KEY_n,           XKB_KEY_o,
        XKB_KEY_p,         XKB_KEY_q,          XKB_KEY_r,           XKB_KEY_s,           XKB_KEY_t,
        XKB_KEY_u,         XKB_KEY_v,          XKB_KEY_w,           XKB_KEY_x,           XKB_KEY_y,
        XKB_KEY_z,         XKB_KEY_apostrophe, XKB_KEY_backslash,   XKB_KEY_comma,       XKB_KEY_braceleft,
        XKB_KEY_minus,     XKB_KEY_period,     XKB_KEY_braceright,  XKB_KEY_semicolon,   XKB_KEY_slash,
        XKB_KEY_BackSpace, XKB_KEY_Delete,     XKB_KEY_End,         XKB_KEY_Return,      XKB_KEY_Escape,
        XKB_KEY_Home,      XKB_KEY_Insert,     XKB_KEY_Page_Down,   XKB_KEY_Page_Up,     XKB_KEY_Pause,
        XKB_KEY_space,     XKB_KEY_Tab,        XKB_KEY_Caps_Lock,   XKB_KEY_Num_Lock,    XKB_KEY_Scroll_Lock,
        XKB_KEY_F1,        XKB_KEY_F2,         XKB_KEY_F3,          XKB_KEY_F4,          XKB_KEY_F5,
        XKB_KEY_F6,        XKB_KEY_F7,         XKB_KEY_F8,          XKB_KEY_F9,          XKB_KEY_F10,
        XKB_KEY_F11,       XKB_KEY_F12,        XKB_KEY_Alt_L,       XKB_KEY_Alt_L,       XKB_KEY_Alt_R,
        XKB_KEY_Control_L, XKB_KEY_Control_L,  XKB_KEY_Control_R,   XKB_KEY_Shift_L,     XKB_KEY_Shift_L,
        XKB_KEY_Shift_R,   XKB_KEY_Super_L,    XKB_KEY_Super_R,     XKB_KEY_Print,       XKB_KEY_Down,
        XKB_KEY_Left,      XKB_KEY_Right,      XKB_KEY_Up,          XKB_KEY_KP_0,        XKB_KEY_KP_1,
        XKB_KEY_KP_2,      XKB_KEY_KP_3,       XKB_KEY_KP_4,        XKB_KEY_KP_5,        XKB_KEY_KP_6,
        XKB_KEY_KP_7,      XKB_KEY_KP_8,       XKB_KEY_KP_9,        XKB_KEY_KP_Add,      XKB_KEY_KP_Decimal,
        XKB_KEY_KP_Divide, XKB_KEY_KP_Equal,   XKB_KEY_KP_Multiply, XKB_KEY_KP_Subtract,
    };

    // wayland
    wl_display *display;
    wl_registry *registry;
    wl_compositor *compositor;
    wl_seat *seat;
    xdg_wm_base *wm_base;
    wl_surface *surface;
    // lib decor
    libdecor_frame *frame;
    libdecor *context;
    losSize content_size;
    losSize configured_size;
    losSize floating_size;
    libdecor_window_state window_state;
    // keyboard
    wl_keyboard *keyboard;
    struct xkb_state *xkb_state;
    struct xkb_context *xkb_context;
    struct xkb_keymap *xkb_keymap;
    // mouse
    static void registry_global(void *data, wl_registry *registry, uint32_t name, const char *interface,
                                uint32_t version);
    static void registry_global_remove(void *, wl_registry *, uint32_t){};
    static void keyboard_keymap(void *data, wl_keyboard *wl_keyboard, uint32_t format, int32_t fd, uint32_t size);
    static void keyboard_enter(void *data, wl_keyboard *wl_keyboard, uint32_t serial, wl_surface *surface,
                               wl_array *keys);
    static void keyboard_leave(void *, wl_keyboard *, uint32_t, wl_surface *){};
    static void keyboard_modifiers(void *data, wl_keyboard *keyboard, uint32_t serial, uint32_t mods_depressed,
                                   uint32_t mods_latched, uint32_t mods_locked, uint32_t group);
    static void keyboard_key(void *data, wl_keyboard *keyboard, uint32_t serial, uint32_t time, uint32_t key,
                             uint32_t state);
    static void keyboard_repeat_info(void *, wl_keyboard *, int32_t, int32_t){};
    static void wm_base_ping(void *data, xdg_wm_base *xdg_wm_base, uint32_t serial);
    static void lib_decor_handle_error(libdecor *context, libdecor_error error, const char *message);
    static void lib_decor_handle_close(libdecor_frame *frame, void *user_data);
    static void lib_decor_handle_commit(libdecor_frame *frame, void *user_data);
    static void lib_decor_handle_dismiss_popup(libdecor_frame *frame, const char *seat_name, void *user_data);
    static void lib_decor_handle_configure(libdecor_frame *frame, libdecor_configuration *configuration,
                                           void *user_data);
    static void seat_capabilities(void *data, wl_seat *wl_seat, uint32_t capabilities);
    static void seat_name(void *, wl_seat *, const char *){};
    static void redraw(WaylandWindow *window);

#    pragma GCC diagnostic push
#    pragma GCC diagnostic ignored "-Wmissing-field-initializers"
    libdecor_interface lib_decor_iface = {
        .error = lib_decor_handle_error,
    };

    libdecor_frame_interface lib_decor_frame_iface = {
        lib_decor_handle_configure,
        lib_decor_handle_close,
        lib_decor_handle_commit,
        lib_decor_handle_dismiss_popup,
    };
#    pragma GCC diagnostic pop

    const wl_keyboard_listener keyboard_listener = {
        .keymap = keyboard_keymap,
        .enter = keyboard_enter,
        .leave = keyboard_leave,
        .key = keyboard_key,
        .modifiers = keyboard_modifiers,
        .repeat_info = keyboard_repeat_info,
    };

    const wl_seat_listener seat_listener = {
        .capabilities = seat_capabilities,
        .name = seat_name,
    };

    const xdg_wm_base_listener wm_base_listener = {
        .ping = wm_base_ping,
    };

    const wl_registry_listener registry_listener = {
        .global = registry_global,
        .global_remove = registry_global_remove,
    };
#endif
  public:
#if __has_include(<wayland-client.h>) && defined(ON_LINUX)
    // temp
    wl_shm *shm;
    explicit WaylandWindow(const std::string title, losSize win_size) noexcept;
    virtual bool hasWindowClosed() const noexcept final override;
    virtual losResult losUpdateWindow() noexcept final override;
    virtual bool losIsKeyDown(losKeyboardButton) const noexcept final override;
    virtual bool losIsMouseDown(losMouseButton) const noexcept final override;
    virtual losResult losRequestClose() noexcept final override;
    virtual losSize losRequestMousePosition() const noexcept final override;
    virtual losSize losRequestMouseWheelDelta() const noexcept final override;
    virtual losSize losIsBeingPressed() const noexcept final override;
    virtual void losDestroyWindow() noexcept final override;
    virtual losSize getWindowSize() noexcept final override;
    virtual bool hasFailed() const noexcept final override
    {
#    ifdef WITH_DEBUG
        if (error)
            puts("[LIBOS] <SOFT_ERROR> -> failed to create Wayland window fallback to XCB");
#    endif
        return error;
    };
    virtual void *internalGet() noexcept final override;
#else
    explicit WaylandWindow(const std::string, losSize) noexcept
    {
    }
    virtual bool hasWindowClosed() const noexcept final override
    {
        return true;
    }
    virtual losResult losUpdateWindow() noexcept final override
    {
        return LOS_ERROR_FEATURE_NOT_IMPLEMENTED;
    }
    virtual bool losIsKeyDown(losKeyboardButton) const noexcept final override
    {
        return false;
    }
    virtual bool losIsMouseDown(losMouseButton) const noexcept final override
    {
        return false;
    }
    virtual losResult losRequestClose() noexcept final override
    {
        return LOS_ERROR_FEATURE_NOT_IMPLEMENTED;
    }
    virtual losSize losRequestMousePosition() const noexcept final override
    {
        return {};
    }
    virtual losSize losRequestMouseWheelDelta() const noexcept final override
    {
        return {};
    }
    virtual losSize losIsBeingPressed() const noexcept final override
    {
        return {};
    }
    virtual void losDestroyWindow() noexcept final override
    {
    }
    virtual losSize getWindowSize() noexcept final override
    {
        return {};
    }
    virtual void *internalGet() noexcept final override
    {
        return nullptr;
    }

    virtual bool hasFailed() const noexcept final override
    {
        return true;
    };
#endif
    virtual losUsedWindowAPI getType() const noexcept final override
    {
        return WAYLAND_API;
    }
};
