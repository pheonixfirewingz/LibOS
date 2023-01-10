#include "Wayland.hpp"
// LIBOS LICENCE
//
// GNU Lesser General Public License Version 3.0
//
// Copyright Luke Shore (c) 2020, 2023
#if __has_include(<wayland-client.h>)
#    include <cassert>
#    include <cstring>
#    include <sys/mman.h>
#    include <vector>
#    define WAYLAND_CHECK(x, func) \
        if (!(x = func))           \
        {                          \
            error = true;          \
            return;                \
        }

WaylandWindow::WaylandWindow(const std::string title, losSize size) noexcept
{
    WAYLAND_CHECK(display, wl_display_connect(nullptr));
    WAYLAND_CHECK(registry, wl_display_get_registry(display));
    WAYLAND_CHECK(xkb_context, xkb_context_new(XKB_CONTEXT_NO_FLAGS));
    wl_registry_add_listener(registry, &registry_listener, this);
    wl_display_roundtrip(display);
    WAYLAND_CHECK(surface, wl_compositor_create_surface(compositor));
    floating_size.length_one = size.length_one;
    floating_size.length_two = size.length_two;
    WAYLAND_CHECK(context, libdecor_new(display, &lib_decor_iface));
    WAYLAND_CHECK(frame, libdecor_decorate(context, surface, &lib_decor_frame_iface, this));
    libdecor_frame_set_app_id(frame, std::string("LibOS:" + title).c_str());
    libdecor_frame_set_title(frame, title.c_str());
    libdecor_frame_map(frame);
    wl_surface_commit(surface);
}

void WaylandWindow::wm_base_ping(void *, struct xdg_wm_base *wm_base, uint32_t serial)
{
    xdg_wm_base_pong(wm_base, serial);
}

void WaylandWindow::registry_global(void *data, wl_registry *registry, uint32_t name, const char *interface, uint32_t)
{
    WaylandWindow *state = static_cast<WaylandWindow *>(data);
    if (strcmp(interface, wl_shm_interface.name) == 0)
        state->shm = static_cast<wl_shm *>(wl_registry_bind(registry, name, &wl_shm_interface, 1));
    else if (strcmp(interface, wl_compositor_interface.name) == 0)
        state->compositor = static_cast<wl_compositor *>(wl_registry_bind(registry, name, &wl_compositor_interface, 4));
    else if (strcmp(interface, xdg_wm_base_interface.name) == 0)
    {
        state->wm_base = static_cast<xdg_wm_base *>(wl_registry_bind(registry, name, &xdg_wm_base_interface, 1));
        xdg_wm_base_add_listener(state->wm_base, &state->wm_base_listener, state);
    }
    else if (strcmp(interface, wl_seat_interface.name) == 0)
    {
        state->seat = static_cast<wl_seat *>(wl_registry_bind(registry, name, &wl_seat_interface, 5));
        wl_seat_add_listener(state->seat, &state->seat_listener, state);
    }
}

void WaylandWindow::seat_capabilities(void *data, wl_seat *, uint32_t capabilities)
{
    WaylandWindow *state = static_cast<WaylandWindow *>(data);

    // bool have_pointer = capabilities & WL_SEAT_CAPABILITY_POINTER;

    /*if (have_pointer && state->wl_pointer == NULL)
    {
        state->wl_pointer = wl_seat_get_pointer(state->wl_seat);
        wl_pointer_add_listener(state->wl_pointer, &wl_pointer_listener, state);
    }
    else if (!have_pointer && state->wl_pointer != NULL)
    {
        wl_pointer_release(state->wl_pointer);
        state->wl_pointer = NULL;
    }*/

    bool have_keyboard = capabilities & WL_SEAT_CAPABILITY_KEYBOARD;

    if (have_keyboard && state->keyboard == NULL)
    {
        state->keyboard = wl_seat_get_keyboard(state->seat);
        wl_keyboard_add_listener(state->keyboard, &state->keyboard_listener, state);
    }
    else if (!have_keyboard && state->keyboard != NULL)
    {
        wl_keyboard_release(state->keyboard);
        state->keyboard = NULL;
    }
}

void WaylandWindow::keyboard_keymap(void *data, wl_keyboard *, uint32_t, int32_t fd, uint32_t size)
{
    WaylandWindow *window = static_cast<WaylandWindow *>(data);
    assert(format == WL_KEYBOARD_KEYMAP_FORMAT_XKB_V1);

    char *map_shm = static_cast<char *>(mmap(nullptr, size, PROT_READ, MAP_SHARED, fd, 0));
    assert(map_shm != MAP_FAILED);

    struct xkb_keymap *xkb_keymap = xkb_keymap_new_from_string(window->xkb_context, map_shm, XKB_KEYMAP_FORMAT_TEXT_V1,
                                                               XKB_KEYMAP_COMPILE_NO_FLAGS);
    munmap(map_shm, size);
    close(fd);

    struct xkb_state *xkb_state = xkb_state_new(xkb_keymap);
    xkb_keymap_unref(window->xkb_keymap);
    xkb_state_unref(window->xkb_state);
    window->xkb_keymap = xkb_keymap;
    window->xkb_state = xkb_state;
}

void WaylandWindow::keyboard_enter(void *data, wl_keyboard *, uint32_t, wl_surface *, wl_array *keys)
{
    WaylandWindow *window = static_cast<WaylandWindow *>(data);
#    pragma GCC diagnostic push
#    pragma GCC diagnostic ignored "-Wpointer-arith"
    std::vector<uint32_t> keys_v;
    keys_v.reserve(keys->size);
    memmove(keys_v.data(), keys->data, keys->size);
#    pragma GCC diagnostic pop
    for (uint32_t key : keys_v)
    {
        char buf[128];
        xkb_keysym_t sym = xkb_state_key_get_one_sym(window->xkb_state, key + 8);
        xkb_keysym_get_name(sym, buf, sizeof(buf));
        fprintf(stderr, "sym: %-12s (%d), ", buf, sym);
        xkb_state_key_get_utf8(window->xkb_state, key + 8, buf, sizeof(buf));
        fprintf(stderr, "utf8: '%s'\n", buf);
    }
}

void WaylandWindow::keyboard_modifiers(void *data, wl_keyboard *, uint32_t, uint32_t mods_depressed,
                                       uint32_t mods_latched, uint32_t mods_locked, uint32_t group)
{
    WaylandWindow *window = static_cast<WaylandWindow *>(data);
    xkb_state_update_mask(window->xkb_state, mods_depressed, mods_latched, mods_locked, 0, 0, group);
}

void WaylandWindow::keyboard_key(void *data, wl_keyboard *, uint32_t, uint32_t, uint32_t key, uint32_t state)
{
    WaylandWindow *window = static_cast<WaylandWindow *>(data);
    uint16_t index = 0;
    uint16_t *ret = window->upper_versions.find(xkb_state_key_get_one_sym(window->xkb_state, key + 8));
    index = !ret ? xkb_state_key_get_one_sym(window->xkb_state, key + 8) : *ret;
    window->keys[index] = state == WL_KEYBOARD_KEY_STATE_PRESSED ? true : false;
}

bool WaylandWindow::hasWindowClosed() const noexcept
{
    return should_close;
}

losResult WaylandWindow::losUpdateWindow() noexcept
{
    if (should_close)
        return LOS_WINDOW_CLOSE;
    if (!(libdecor_dispatch(context, 0) >= 0))
        return LOS_WINDOW_CLOSE;
    return LOS_SUCCESS;
}

bool WaylandWindow::losIsKeyDown(losKeyboardButton button) const noexcept
{
    return keys[window_key_look_up_table[button]];
}

bool WaylandWindow::losIsMouseDown(losMouseButton) const noexcept
{
    return false;
}

losResult WaylandWindow::losRequestClose() noexcept
{
    should_close = true;
    return LOS_SUCCESS;
}

losSize WaylandWindow::losRequestMousePosition() const noexcept
{
    uint16_t _x = x;
    uint16_t _y = y;
    return {_x, _y};
}

losSize WaylandWindow::losRequestMouseWheelDelta() const noexcept
{
    return {0u, 0u};
}

losSize WaylandWindow::losIsBeingPressed() const noexcept
{
    uint16_t _x = x;
    uint16_t _y = y;
    return {_x, _y};
}

void WaylandWindow::losDestroyWindow() noexcept
{
    libdecor_frame_close(frame);
}

losSize *WaylandWindow::getWindowSize() noexcept
{
    return &configured_size;
}

void *WaylandWindow::internalGet() noexcept
{
    return new losWindowWayland(display, surface);
}
#endif