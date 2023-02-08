#pragma once
// LIBOS LICENCE
//
// GNU Lesser General Public License Version 3.0
//
// Copyright Luke Shore (c) 2020, 2023
#include "../Interface/AbstractWindow.h"
#if __has_include(<xcb/xcb.h>) && defined(ON_LINUX)
#    include <xcb/xcb.h>
#    include <xcb/xcb_keysyms.h>
#    define XK_LATIN1
#    define XK_MISCELLANY
#    include <X11/keysym.h>
#    undef XK_MISCELLANY
#    undef XL_LATIN1
#endif
class XcbWindow : public BaseWindow
{
#if __has_include(<xcb/xcb.h>) && defined(ON_LINUX)
    bool error = false;
    xcb_connection_t *con;
    xcb_window_t win;
    xcb_screen_t *screen;
    xcb_key_symbols_t *symbols;
    losSize configured_size;
    xcb_intern_atom_reply_t *atom_wm_delete_window;
    std::atomic_bool should_close{false}, isInWindow{false}, isRest{false}, keys[256]{}, buttons[3]{};
    int16_ts x = 0, y = 0;
    const uint16_t window_key_look_up_table[256] = {
        19,
        10,
        11,
        12,
        13,
        14,
        15,
        16,
        17,
        18,
        38,
        56,
        54,
        40,
        26,
        41,
        42,
        43,
        31,
        44,
        45,
        46,
        58,
        57,
        32,
        33,
        24,
        27,
        39,
        28,
        30,
        55,
        25,
        53,
        29,
        52,
        XK_apostrophe,
        XK_backslash,
        XK_comma,
        XK_bracketleft,
        XK_minus,
        XK_period,
        XK_bracketright,
        XK_semicolon,
        XK_slash,
        XK_BackSpace,
        XK_Delete,
        XK_End,
        XK_KP_Enter,
        XK_Escape,
        XK_Home,
        XK_Insert,
        XK_Prior,
        XK_Next,
        XK_Pause,
        XK_space,
        XK_Tab,
        XK_Caps_Lock,
        XK_Num_Lock,
        XK_Scroll_Lock,
        67,
        68,
        69,
        70,
        71,
        72,
        73,
        74,
        75,
        76,
        95,
        96,
        XK_Menu,
        XK_Meta_L,
        XK_Meta_R,
        XK_Control_L,
        XK_Control_L,
        XK_Control_R,
        XK_Shift_L,
        XK_Shift_L,
        XK_Shift_R,
        XK_Super_L,
        XK_Super_R,
        XK_Print,
        116,
        113,
        114,
        111,
        90,
        87,
        88,
        89,
        83,
        84,
        85,
        79,
        80,
        81,
        86,
        XK_KP_Decimal,
        106,
        21,
        63,
        86,
        82,
    };
#endif
  public:
#if __has_include(<xcb/xcb.h>) && defined(ON_LINUX)
    explicit XcbWindow(const std::string title, losSize win_size) noexcept;
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
        return error;
    };
    virtual void *internalGet() noexcept final override;
#else
    explicit XcbWindow(const std::string, losSize) noexcept
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
        return XCB_API;
    }
};
