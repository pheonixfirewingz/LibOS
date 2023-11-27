// LIBOS LICENCE
//
// GNU Lesser General Public License Version 3.0
//
// Copyright Luke Shore (c) 2020, 2023
#include "libdecor/libdecor.h"
#include "libos/NativeWindow.h"
#include "xdg-shell-client-protocol.h"
#include <cassert>
#include <cstring>
#include <libos/Error.h>
#include <libos/Window.h>
#include <linux/input-event-codes.h>
#include <map>
#include <sys/mman.h>
#include <vector>
#include <wayland-client.h>
#include <xkbcommon/xkbcommon-keysyms.h>
#include <xkbcommon/xkbcommon.h>

struct losWindow_T
{
    std::atomic_bool should_close{false}, keys[256]{}, buttons[3]{};
    int32_ts x = 0, y = 0;
    bool error = false;
    const std::map<const uint16_t, const uint16_t> upper_versions = {
        {XKB_KEY_A, XKB_KEY_a}, {XKB_KEY_B, XKB_KEY_b}, {XKB_KEY_C, XKB_KEY_c}, {XKB_KEY_D, XKB_KEY_d},
        {XKB_KEY_E, XKB_KEY_e}, {XKB_KEY_F, XKB_KEY_f}, {XKB_KEY_G, XKB_KEY_g}, {XKB_KEY_H, XKB_KEY_h},
        {XKB_KEY_I, XKB_KEY_i}, {XKB_KEY_J, XKB_KEY_j}, {XKB_KEY_K, XKB_KEY_k}, {XKB_KEY_L, XKB_KEY_l},
        {XKB_KEY_M, XKB_KEY_m}, {XKB_KEY_N, XKB_KEY_n}, {XKB_KEY_O, XKB_KEY_o}, {XKB_KEY_P, XKB_KEY_p},
        {XKB_KEY_Q, XKB_KEY_q}, {XKB_KEY_R, XKB_KEY_r}, {XKB_KEY_S, XKB_KEY_s}, {XKB_KEY_T, XKB_KEY_t},
        {XKB_KEY_U, XKB_KEY_u}, {XKB_KEY_V, XKB_KEY_v}, {XKB_KEY_W, XKB_KEY_w}, {XKB_KEY_X, XKB_KEY_x},
        {XKB_KEY_Y, XKB_KEY_y}, {XKB_KEY_Z, XKB_KEY_z}};

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
    wl_display *display{};
    wl_registry *registry{};
    wl_compositor *compositor{};
    wl_seat *seat{};
    xdg_wm_base *wm_base{};
    wl_surface *surface{};
    // lib decor
    libdecor_frame *frame{};
    libdecor *context{};
    losSize content_size;
    losSize configured_size;
    losSize floating_size;
    libdecor_window_state window_state;
    // keyboard
    wl_keyboard *keyboard{};
    struct xkb_state *xkb_state{};
    struct xkb_context *xkb_context{};
    struct xkb_keymap *xkb_keymap{};
    // mouse
    struct wl_pointer *pointer{};
    // temp
    wl_shm *shm{};
};

class Buffer
{
    wl_buffer *buffer;
    uint32_t* buf;
    int32_t fd;
    losSize size;
    bool initalized = false;
    int create_shm()
    {
        int retries = 100;
        do
        {
            std::string name("/wayland_libos_app_");
            name += std::to_string(rand());
            --retries;
            int fd_local = 0;
            if ((fd_local = shm_open(name.c_str(), 194, 0600)) >= 0)
            {
                shm_unlink(name.c_str());
                return fd_local;
            }
        } while (retries > 0 && errno == EEXIST);
        return -1;
    }
    
  public:
    explicit Buffer(losWindow window, wl_shm_format format) noexcept
        : size(losRequestWindowSize(window))
    {
        if ((fd = create_shm()) < 0)
            return;
        int ret;
        do
        {
            ret = ftruncate(fd, size.length_one * size.length_two * 4);
        } while (ret < 0 && errno == EINTR);
        if (ret < 0)
        {
            close(fd);
            return;
        }
        wl_shm_pool *pool = wl_shm_create_pool(window->shm, fd, (size.length_one * 4) * size.length_two);
        buffer = wl_shm_pool_create_buffer(pool, 0, size.length_one, size.length_two, size.length_one * 4, format);
        wl_shm_pool_destroy(pool);
        initalized = true;
    }

    bool paint(libdecor_window_state window_state)
    {
        if (!initalized)
            return false;
        buf = static_cast<uint32_t *>(
            mmap(nullptr, (size.length_one * 4) * size.length_two, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0));
        if (buf == MAP_FAILED)
        {
            close(fd);
            return false;
        }
        memset(buf, window_state & LIBDECOR_WINDOW_STATE_ACTIVE ? 0xffbcbcbc : 0xff8e8e8e,
               (size.length_one * 4) * size.length_two);
        return true;
    }

    wl_buffer *get()
    {
        return buffer;
    }

    ~Buffer()
    {
        close(fd);
        wl_buffer_destroy(buffer);
        munmap(buf, (size.length_one * 4) * size.length_two);
    }
};

void redraw(losWindow_T *window)
{
    static Buffer buffer(window, WL_SHM_FORMAT_XRGB8888);
    if (buffer.paint(window->window_state))
        wl_surface_attach(window->surface, buffer.get(), 0, 0);
    wl_surface_set_buffer_scale(window->surface, 1);
    wl_surface_damage_buffer(window->surface, 0, 0, window->configured_size.length_one * 4,
                             window->configured_size.length_two * 4);
    wl_surface_commit(window->surface);
}

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wmissing-field-initializers"
const static wl_pointer_listener pointer_listener = {
    .enter =
        [](void *data, wl_pointer *, uint32_t, struct wl_surface *, wl_fixed_t, wl_fixed_t) {
            auto *state = static_cast<losWindow>(data);
            memset(&state->keys, 0, 256);
            memset(&state->buttons, 0, 3);
        },
    .leave =
        [](void *data, wl_pointer *, uint32_t, wl_surface *) {
            auto *state = static_cast<losWindow>(data);
            memset(&state->keys, 0, 256);
            memset(&state->buttons, 0, 3);
        },
    .motion =
        [](void *data, wl_pointer *, uint32_t, wl_fixed_t surface_x, wl_fixed_t surface_y) {
            auto *state = static_cast<losWindow>(data);
            state->x = surface_x;
            state->y = surface_y;
        },
    .button =
        [](void *data, wl_pointer *, uint32_t, uint32_t, uint32_t button, uint32_t state_) {
            auto *state = static_cast<losWindow>(data);
            bool pressed = state_ == WL_POINTER_BUTTON_STATE_PRESSED;
            switch (button)
            {
            case BTN_LEFT:
                state->buttons[LOS_LEFT_BUTTON] = pressed;
                break;
            case BTN_RIGHT:
                state->buttons[LOS_RIGHT_BUTTON] = pressed;
                break;
            case BTN_MIDDLE:
                state->buttons[LOS_MIDDLE_BUTTON] = pressed;
                break;
            default:
                break;
            }
        },
    .axis = [](void *, wl_pointer *, uint32_t, uint32_t, wl_fixed_t) {},
    .frame = [](void *, wl_pointer *) {},
    .axis_source = [](void *, wl_pointer *, uint32_t) {},
    .axis_stop = [](void *, wl_pointer *, uint32_t, uint32_t) {},
    .axis_discrete = [](void *, wl_pointer *, uint32_t, int32_t) {},
};

static libdecor_interface lib_decor_iface = {
    .error =
        [](libdecor *, enum libdecor_error error, const char *message) {
            fprintf(stderr, "Caught error (%d): %s\n", error, message);
            exit(EXIT_FAILURE);
        },
};

static libdecor_frame_interface lib_decor_frame_iface = {
    .configure = [](libdecor_frame *frame, libdecor_configuration *configuration,
                    void *user_data)
    {
        auto *state = static_cast<losWindow>(user_data);
        int width = 0, height = 0;
        enum libdecor_window_state window_state;
        libdecor_state *lib_decor_state;
        
        if (!libdecor_configuration_get_window_state(configuration, &window_state))
            window_state = LIBDECOR_WINDOW_STATE_NONE;

        state->window_state = window_state;

        if (!libdecor_configuration_get_content_size(configuration, frame, &width, &height))
        {
            width = state->content_size.length_one;
            height = state->content_size.length_two;
        }

        width = (width == 0) ? state->floating_size.length_one : width;
        height = (height == 0) ? state->floating_size.length_two : height;

        state->configured_size.length_one = width;
        state->configured_size.length_two = height;

        lib_decor_state = libdecor_state_new(width, height);
        libdecor_frame_commit(frame, lib_decor_state, configuration);
        libdecor_state_free(lib_decor_state);

        if (libdecor_frame_is_floating(state->frame))
        {
            state->floating_size.length_one = width;
            state->floating_size.length_two = height;
        }

        redraw(state);
    },
    .close = [](libdecor_frame *, void *user_data)
    {
        auto *state = static_cast<losWindow>(user_data);
        state->should_close = true;
    },
    .commit = [](libdecor_frame *, void *user_data)
    {
        auto *state = static_cast<losWindow>(user_data);
        wl_surface_commit(state->surface);
    },
    .dismiss_popup = [](libdecor_frame *, const char *, void *)
    {
        puts("ERROR: not implemented");
        exit(EXIT_FAILURE);
    },
};
#pragma GCC diagnostic pop

const wl_keyboard_listener keyboard_listener = {
    .keymap = [](void *data, wl_keyboard *, uint32_t format, int32_t fd, uint32_t size)
    {
        auto *window = static_cast<losWindow>(data);
        assert(format == WL_KEYBOARD_KEYMAP_FORMAT_XKB_V1);
        (void)format;

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
    },
    .enter =
        [](void *data, wl_keyboard *, uint32_t, wl_surface *, wl_array *keys) {
            auto *window = static_cast<losWindow>(data);
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpointer-arith"
            std::vector<uint32_t> keys_v;
            keys_v.reserve(keys->size);
            memmove(keys_v.data(), keys->data, keys->size);
#pragma GCC diagnostic pop
            for (uint32_t key : keys_v)
            {
                char buf[128];
                xkb_keysym_t sym = xkb_state_key_get_one_sym(window->xkb_state, key + 8);
                xkb_keysym_get_name(sym, buf, sizeof(buf));
                fprintf(stderr, "sym: %-12s (%d), ", buf, sym);
                xkb_state_key_get_utf8(window->xkb_state, key + 8, buf, sizeof(buf));
                fprintf(stderr, "utf8: '%s'\n", buf);
            }
        },
    .leave = [](void *, wl_keyboard *, uint32_t, wl_surface *){},
    .key =
        [](void *data, wl_keyboard *, uint32_t, uint32_t, uint32_t key, uint32_t state) {
            auto *window = static_cast<losWindow>(data);
            uint16_t index;
            if (auto ret = window->upper_versions.find(xkb_state_key_get_one_sym(window->xkb_state, key + 8));
                ret == window->upper_versions.end())
                index = xkb_state_key_get_one_sym(window->xkb_state, key + 8);
            else
                index = ret->second;
            window->keys[index] = state == WL_KEYBOARD_KEY_STATE_PRESSED;
        },
    .modifiers =
        [](void *data, wl_keyboard *, uint32_t, uint32_t mods_depressed, uint32_t mods_latched, uint32_t mods_locked,
           uint32_t group) {
            auto *window = static_cast<losWindow>(data);
            xkb_state_update_mask(window->xkb_state, mods_depressed, mods_latched, mods_locked, 0, 0, group);
        },
    .repeat_info = [](void *, wl_keyboard *, int32_t, int32_t){},
};

const static wl_seat_listener seat_listener = {
    .capabilities = [](void *data, wl_seat *, uint32_t capabilities)
    {
        auto *state = static_cast<losWindow>(data);

        bool have_pointer = capabilities & WL_SEAT_CAPABILITY_POINTER;

        if (have_pointer && state->pointer == NULL)
        {
            state->pointer = wl_seat_get_pointer(state->seat);
            wl_pointer_add_listener(state->pointer, &pointer_listener, state);
        }
        else if (!have_pointer && state->pointer != NULL)
        {
            wl_pointer_release(state->pointer);
            state->pointer = NULL;
        }

        bool have_keyboard = capabilities & WL_SEAT_CAPABILITY_KEYBOARD;

        if (have_keyboard && state->keyboard == NULL)
        {
            state->keyboard = wl_seat_get_keyboard(state->seat);
            wl_keyboard_add_listener(state->keyboard, &keyboard_listener, state);
        }
        else if (!have_keyboard && state->keyboard != NULL)
        {
            wl_keyboard_release(state->keyboard);
            state->keyboard = NULL;
        }
    },
    .name = [](void *, wl_seat *, const char *) {},
};

const static wl_registry_listener registry_listener = {
    .global =
        [](void *data, wl_registry *registry, uint32_t name, const char *interface, uint32_t) {
            auto *state = static_cast<losWindow>(data);
            if (strcmp(interface, wl_shm_interface.name) == 0)
                state->shm = static_cast<wl_shm *>(wl_registry_bind(registry, name, &wl_shm_interface, 1));
            else if (strcmp(interface, wl_compositor_interface.name) == 0)
                state->compositor =
                    static_cast<wl_compositor *>(wl_registry_bind(registry, name, &wl_compositor_interface, 4));
            else if (strcmp(interface, xdg_wm_base_interface.name) == 0)
            {
                state->wm_base =
                    static_cast<xdg_wm_base *>(wl_registry_bind(registry, name, &xdg_wm_base_interface, 1));
                const static xdg_wm_base_listener wm_base_listener = {
                    .ping = [](void *, struct xdg_wm_base *wm_base,
                               uint32_t serial) { xdg_wm_base_pong(wm_base, serial); },
                };

                xdg_wm_base_add_listener(state->wm_base, &wm_base_listener, state);
            }
            else if (strcmp(interface, wl_seat_interface.name) == 0)
            {
                state->seat = static_cast<wl_seat *>(wl_registry_bind(registry, name, &wl_seat_interface, 5));
                wl_seat_add_listener(state->seat, &seat_listener, state);
            }
        },
    .global_remove = [](void *, wl_registry *, uint32_t) {},
};

#define WAYLAND_CHECK(x, func)           \
    if (!(x = func))                     \
    {                                    \
        window->error = true;            \
        losPrintLastSystemError();       \
        return LOS_ERROR_COULD_NOT_INIT; \
    }

losResult losCreateWindow(losWindow *window_in, losWindowInfo info)
{
    *window_in = new losWindow_T();
    auto *window = *window_in;
#ifdef WITH_DEBUG
    losPrintInfo("[LIBOS] <INFO> -> creating WAYLAND API Window");
#endif
    WAYLAND_CHECK(window->display, wl_display_connect(nullptr))
    WAYLAND_CHECK(window->registry, wl_display_get_registry(window->display))
    WAYLAND_CHECK(window->xkb_context, xkb_context_new(XKB_CONTEXT_NO_FLAGS))

    wl_registry_add_listener(window->registry, &registry_listener, window);
    wl_display_roundtrip(window->display);
    WAYLAND_CHECK(window->surface, wl_compositor_create_surface(window->compositor))
    window->floating_size.length_one = info.window_size.length_one;
    window->floating_size.length_two = info.window_size.length_two;
    WAYLAND_CHECK(window->context, libdecor_new(window->display, &lib_decor_iface))
    WAYLAND_CHECK(window->frame, libdecor_decorate(window->context, window->surface, &lib_decor_frame_iface, window))
    libdecor_frame_set_app_id(window->frame,
                              std::string("LibOS:" + std::string(info.title, 0, info.title_size)).c_str());
    libdecor_frame_set_title(window->frame, std::string(info.title, 0, info.title_size).c_str());
    libdecor_frame_map(window->frame);
    wl_surface_commit(window->surface);
    return  LOS_SUCCESS;
}

losResult losUpdateWindow(losWindow window)
{
    if (window->should_close || window->context == nullptr)
        return LOS_WINDOW_CLOSE;
    if (libdecor_dispatch(window->context, 0) < 0)
        return LOS_WINDOW_CLOSE;
    return LOS_SUCCESS;
}

uint8_t losIsKeyDown(losWindow window, losKeyboardButton key)
{
    return window->keys[window->window_key_look_up_table[key]];
}

uint8_t losIsMouseDown(losWindow window, losMouseButton button)
{
    return window->buttons[button];
}

losResult losRequestClose(losWindow window)
{
    window->should_close = true;
    return LOS_SUCCESS;
}

losSize losRequestMousePosition(losWindow window)
{
    return {window->x, window->y};
}

losSize losRequestMouseWheelDelta(losWindow*)
{
    return {0u, 0u};
}

losSize losIsBeingPressed(losWindow)
{
    return {0u, 0u};
}

void losDestroyWindow(losWindow window)
{
    if (window->frame != nullptr)
        libdecor_frame_close(window->frame);
    delete window;
}

losSize losRequestWindowSize(losWindow window)
{
    return window->configured_size;
}

losUsedWindowAPI losWhatApiIsWindowUsed(losWindow)
{
    return WAYLAND_API;
}

void *losGetWindowNativePointer(losWindow window)
{
    return new losWindowWayland(window->display, window->surface);
}
