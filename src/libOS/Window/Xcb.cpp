#include "Xcb.hpp"
// LIBOS LICENCE
//
// GNU Lesser General Public License Version 3.0
//
// Copyright Luke Shore (c) 2020, 2023
#if __has_include(<xcb/xcb.h>)

XcbWindow::XcbWindow(const std::string title, losSize win_size) noexcept
{
    configured_size = win_size;
    uint32_t mask = XCB_CW_BACK_PIXEL | XCB_CW_EVENT_MASK;
    con = xcb_connect(nullptr, nullptr);
    screen = xcb_setup_roots_iterator(xcb_get_setup(con)).data;
    uint32_t values[2];
    values[0] = screen->white_pixel;
    values[1] = XCB_EVENT_MASK_KEY_RELEASE | XCB_EVENT_MASK_KEY_PRESS | XCB_EVENT_MASK_BUTTON_RELEASE |
                XCB_EVENT_MASK_BUTTON_PRESS | XCB_EVENT_MASK_EXPOSURE | XCB_EVENT_MASK_POINTER_MOTION |
                XCB_EVENT_MASK_ENTER_WINDOW | XCB_EVENT_MASK_LEAVE_WINDOW;

    win = xcb_generate_id(con);

    xcb_create_window(con, XCB_COPY_FROM_PARENT, win, screen->root, 0, 0, win_size.length_one, win_size.length_two, 0,
                      XCB_WINDOW_CLASS_INPUT_OUTPUT, screen->root_visual, mask, values);

    xcb_intern_atom_cookie_t cookie = xcb_intern_atom(con, 1, 12, "WM_PROTOCOLS");
    xcb_intern_atom_reply_t *reply = xcb_intern_atom_reply(con, cookie, nullptr);

    xcb_intern_atom_cookie_t cookie2 = xcb_intern_atom(con, 0, 16, "WM_DELETE_WINDOW");
    atom_wm_delete_window = xcb_intern_atom_reply(con, cookie2, nullptr);

    xcb_change_property(con, XCB_PROP_MODE_REPLACE, win, (*reply).atom, 4, 32, 1, &(*atom_wm_delete_window).atom);
    free(reply);

    xcb_change_property(con, XCB_PROP_MODE_REPLACE, win, XCB_ATOM_WM_NAME, XCB_ATOM_STRING, 8, title.size(),
                        title.c_str());

    /* Map the window on the screen */
    xcb_map_window(con, win);

    const uint32_t coords[] = {300, 150};
    xcb_configure_window(con, win, XCB_CONFIG_WINDOW_X | XCB_CONFIG_WINDOW_Y, coords);
    /* Make sure commands are sent before we pause, so window is shown */
    xcb_flush(con);
    symbols = xcb_key_symbols_alloc(con);
}

bool XcbWindow::hasWindowClosed() const noexcept
{
    return should_close;
}

losResult XcbWindow::losUpdateWindow() noexcept
{
    if (should_close)
        return LOS_WINDOW_CLOSE;

    xcb_generic_event_t *event = xcb_poll_for_event(con);
    if (event == nullptr)
        return LOS_SUCCESS;

    if (!isInWindow)
    {
        if (!isRest)
        {
            for (uint8_t i = 0; i < 255; i++)
                keys[i] = false;

            for (uint8_t i = 0; i < 2; i++)
                buttons[i] = false;
            isRest = true;
        }
    }

    switch (event->response_type & ~0x80)
    {
    case XCB_KEY_PRESS: {
        if (isInWindow)
            keys[reinterpret_cast<xcb_key_press_event_t *>(event)->detail] = true;
    }
    break;
    case XCB_KEY_RELEASE: {
        if (isInWindow)
            keys[reinterpret_cast<xcb_key_release_event_t *>(event)->detail] = false;
    }
    break;
    case XCB_BUTTON_PRESS: {
        if (isInWindow)
        {
            auto button = reinterpret_cast<xcb_button_press_event_t *>(event);
            buttons[button->detail] = true;
        }
    }
    break;
    case XCB_BUTTON_RELEASE: {
        if (isInWindow)
        {
            auto button = reinterpret_cast<xcb_button_release_event_t *>(event);
            buttons[button->detail] = false;
        }
    }
    break;
    case XCB_MOTION_NOTIFY: {
        if (isInWindow)
        {
            auto mouse_move = reinterpret_cast<xcb_motion_notify_event_t *>(event);
            x = mouse_move->event_x;
            y = mouse_move->event_y;
        }
    }
    break;
     case XCB_RESIZE_REQUEST: {
        auto resize = (xcb_resize_request_event_t*) event;
        if (resize->width > 0) configured_size.length_one = resize->width;
        if (resize->height > 0) configured_size.length_two = resize->height;
        break;
    }
    case XCB_ENTER_NOTIFY: {
        isInWindow = true;
        isRest = false;
    }
    break;
    case XCB_LEAVE_NOTIFY: {
        isInWindow = false;
    }
    break;
    case XCB_CLIENT_MESSAGE:
        if ((*(xcb_client_message_event_t *)event).data.data32[0] == (*atom_wm_delete_window).atom)
        {
            should_close = true;
            return LOS_WINDOW_CLOSE;
        }
        break;
    default:
        break;
    }
    free(event);
    return LOS_SUCCESS;
}

bool XcbWindow::losIsKeyDown(losKeyboardButton button) const noexcept
{
    return keys[window_key_look_up_table[button]];
}

bool XcbWindow::losIsMouseDown(losMouseButton button) const noexcept
{
    return buttons[button];
}

losResult XcbWindow::losRequestClose() noexcept
{
    should_close = true;
    return LOS_SUCCESS;
}

losSize XcbWindow::losRequestMousePosition() const noexcept
{
    uint16_t _x = x;
    uint16_t _y = y;
    return {_x, _y};
}

losSize XcbWindow::losRequestMouseWheelDelta() const noexcept
{
    return {0u, 0u};
}

losSize XcbWindow::losIsBeingPressed() const noexcept
{
    uint16_t _x = x;
    uint16_t _y = y;
    return {_x, _y};
}

void XcbWindow::losDestroyWindow() noexcept
{
    xcb_destroy_window(con, win);
    xcb_disconnect(con);
    xcb_key_symbols_free(symbols);
    free(atom_wm_delete_window);
}

losSize* XcbWindow::getWindowSize() noexcept
{
    return &configured_size;
}

void* XcbWindow::internalGet() noexcept
{
    return new losWindowXCB(con,&win);
}
#endif