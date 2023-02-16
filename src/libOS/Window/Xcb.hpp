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
#    include "../../shared/extend_std/LookUpTable.h"
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
    const std::ReadOnlyLookupTable<int32_t, losKeyboardButton> window_key_look_up_table = {
        {48, LOS_KEY_0},
        {49, LOS_KEY_1},
        {50, LOS_KEY_2},
        {51, LOS_KEY_3},
        {52, LOS_KEY_4},
        {53, LOS_KEY_5},
        {54, LOS_KEY_6},
        {55, LOS_KEY_7},
        {56, LOS_KEY_8},
        {57, LOS_KEY_9},
        {97, LOS_KEY_A},
        {98, LOS_KEY_B},
        {99, LOS_KEY_C},
        {100, LOS_KEY_D},
        {101, LOS_KEY_E},
        {102, LOS_KEY_F},
        {103, LOS_KEY_G},
        {104, LOS_KEY_H},
        {105, LOS_KEY_I},
        {106, LOS_KEY_J},
        {107, LOS_KEY_K},
        {108, LOS_KEY_L},
        {109, LOS_KEY_M},
        {110, LOS_KEY_N},
        {111, LOS_KEY_O},
        {112, LOS_KEY_P},
        {113, LOS_KEY_Q},
        {114, LOS_KEY_R},
        {115, LOS_KEY_S},
        {116, LOS_KEY_T},
        {117, LOS_KEY_U},
        {118, LOS_KEY_V},
        {119, LOS_KEY_W},
        {120, LOS_KEY_X},
        {121, LOS_KEY_Y},
        {122, LOS_KEY_Z},
        {XK_apostrophe, LOS_KEY_APOSTROPHE},
        {XK_backslash, LOS_KEY_BACKSLASH},
        {XK_comma, LOS_KEY_COMMA},
        {XK_bracketleft, LOS_KEY_LEFT_BRACKET},
        {XK_minus, LOS_KEY_MINUS},
        {XK_period, LOS_KEY_PERIOD},
        {XK_bracketright, LOS_KEY_RIGHT_BRACKET},
        {XK_semicolon, LOS_KEY_SEMICOLON},
        {XK_slash, LOS_KEY_SLASH},
        {XK_BackSpace, LOS_KEY_BACKSPACE},
        {XK_Delete, LOS_KEY_DELETE},
        {XK_End, LOS_KEY_END},
        {XK_Return, LOS_KEY_ENTER},
        {XK_Escape, LOS_KEY_ESCAPE},
        {XK_Home, LOS_KEY_HOME},
        {XK_Insert, LOS_KEY_INSERT},
        {XK_Page_Down, LOS_KEY_PAGE_DOWN},
        {XK_Page_Up, LOS_KEY_PAGE_UP},
        {XK_Pause, LOS_KEY_PAUSE},
        {XK_space, LOS_KEY_SPACE},
        {XK_Caps_Lock, LOS_KEY_CAPS_LOCK},
        {XK_Num_Lock, LOS_KEY_NUM_LOCK},
        {XK_Scroll_Lock, LOS_KEY_SCROLL_LOCK},
        {XK_F1, LOS_KEY_F1},
        {XK_F2, LOS_KEY_F2},
        {XK_F3, LOS_KEY_F3},
        {XK_F4, LOS_KEY_F4},
        {XK_F5, LOS_KEY_F5},
        {XK_F6, LOS_KEY_F6},
        {XK_F7, LOS_KEY_F7},
        {XK_F8, LOS_KEY_F8},
        {XK_F9, LOS_KEY_F9},
        {XK_F10, LOS_KEY_F10},
        {XK_F11, LOS_KEY_F11},
        {XK_F12, LOS_KEY_F12},
        {XK_Alt_L, LOS_KEY_ALT},
        {XK_Alt_L, LOS_KEY_LEFT_ALT},
        {XK_Alt_R, LOS_KEY_RIGHT_ALT},
        {XK_Control_L, LOS_KEY_CONTROL},
        {XK_Control_L, LOS_KEY_LEFT_CONTROL},
        {XK_Control_R, LOS_KEY_RIGHT_CONTROL},
        {XK_Shift_L, LOS_KEY_SHIFT},
        {XK_Shift_L, LOS_KEY_LEFT_SHIFT},
        {XK_Shift_R, LOS_KEY_RIGHT_SHIFT},
        {XK_Super_L, LOS_KEY_LEFT_SUPER},
        {XK_Super_R, LOS_KEY_RIGHT_SUPER},
        {XK_Print, LOS_KEY_UP},
        {XK_Down, LOS_KEY_DOWN},
        {XK_Left, LOS_KEY_LEFT},
        {XK_Right, LOS_KEY_RIGHT},
        {XK_Up, LOS_KEY_UP},
        {XK_KP_0, LOS_KEY_NUM_PAD_0},
        {XK_KP_1, LOS_KEY_NUM_PAD_1},
        {XK_KP_2, LOS_KEY_NUM_PAD_2},
        {XK_KP_3, LOS_KEY_NUM_PAD_3},
        {XK_KP_4, LOS_KEY_NUM_PAD_4},
        {XK_KP_5, LOS_KEY_NUM_PAD_5},
        {XK_KP_6, LOS_KEY_NUM_PAD_6},
        {XK_KP_7, LOS_KEY_NUM_PAD_7},
        {XK_KP_8, LOS_KEY_NUM_PAD_8},
        {XK_KP_9, LOS_KEY_NUM_PAD_9},
        {XK_KP_Add, LOS_KEY_ADD},
        {XK_KP_Decimal, LOS_KEY_DECIMAL},
        {XK_KP_Divide, LOS_KEY_DIVIDE},
        {XK_KP_Multiply, LOS_KEY_MULTIPLY},
        { XK_KP_Subtract,
          LOS_KEY_SUBTRACT }};

    const uint8_t window_mouse_look_up_table[3] = {LOS_MIDDLE_BUTTON, LOS_RIGHT_BUTTON,LOS_LEFT_BUTTON};
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
