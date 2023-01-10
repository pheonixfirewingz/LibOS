#include "Wayland.hpp"
// LIBOS LICENCE
//
// GNU Lesser General Public License Version 3.0
//
// Copyright Luke Shore (c) 2020, 2023
#include <libos/Defines.h>
#include <string.h>
#include <sys/mman.h>

class Buffer
{
    wl_buffer *buffer;
    uint32_t* buf;
    int32_t fd;
    losSize *size;
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
    explicit Buffer(WaylandWindow *window, wl_shm_format format) noexcept
        : size(window->getWindowSize())
    {
        if ((fd = create_shm()) < 0)
            return;
        int ret;
        do
        {
            ret = ftruncate(fd, size->length_one * size->length_two * 4);
        } while (ret < 0 && errno == EINTR);
        if (ret < 0)
        {
            close(fd);
            return;
        }
        wl_shm_pool *pool = wl_shm_create_pool(window->shm, fd, (size->length_one * 4) * size->length_two);
        buffer = wl_shm_pool_create_buffer(pool, 0, size->length_one, size->length_two, size->length_one * 4, format);
        wl_shm_pool_destroy(pool);
        initalized = true;
    }

    bool paint(libdecor_window_state window_state)
    {
        if (!initalized)
            return false;
        buf = static_cast<uint32_t *>(
            mmap(NULL, (size->length_one * 4) * size->length_two, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0));
        if (buf == MAP_FAILED)
        {
            close(fd);
            return false;
        }
        memset(buf, window_state & LIBDECOR_WINDOW_STATE_ACTIVE ? 0xffbcbcbc : 0xff8e8e8e,
               (size->length_one * 4) * size->length_two);
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
		munmap(buf, (size->length_one * 4) * size->length_two);
    }
};

void WaylandWindow::redraw(WaylandWindow *window)
{
    Buffer buffer(window, WL_SHM_FORMAT_XRGB8888);
    if (buffer.paint(window->window_state))
        wl_surface_attach(window->surface, buffer.get(), 0, 0);
    wl_surface_set_buffer_scale(window->surface, 1);
    wl_surface_damage_buffer(window->surface, 0, 0, window->configured_size.length_one * 4,
                             window->configured_size.length_two * 4);
    wl_surface_commit(window->surface);
}

void WaylandWindow::lib_decor_handle_error(libdecor *, enum libdecor_error error, const char *message)
{
    fprintf(stderr, "Caught error (%d): %s\n", error, message);
    exit(EXIT_FAILURE);
}

void WaylandWindow::lib_decor_handle_close(libdecor_frame *, void *user_data)
{
    WaylandWindow *state = static_cast<WaylandWindow *>(user_data);
    state->should_close = true;
}

void WaylandWindow::lib_decor_handle_commit(libdecor_frame *, void *user_data)
{
    WaylandWindow *state = static_cast<WaylandWindow *>(user_data);
    wl_surface_commit(state->surface);
}

void WaylandWindow::lib_decor_handle_dismiss_popup(libdecor_frame *, const char *, void *)
{
    puts("ERROR: not implemented");
    exit(EXIT_FAILURE);
}

void WaylandWindow::lib_decor_handle_configure(libdecor_frame *frame, libdecor_configuration *configuration,
                                               void *user_data)
{
    WaylandWindow *state = static_cast<WaylandWindow *>(user_data);
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
}