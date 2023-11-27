#include "Android.hpp"
#include <libos/Defines.h>
// LIBOS LICENCE
//
// GNU Lesser General Public License Version 3.0
//
// Copyright Luke Shore (c) 2020, 2023

AndroidWindow::AndroidWindow(const std::string, losSize) noexcept
{
    puts("[LIBOS] <INFO> -> creating Android API Window");
    error = true;
    ANativeWindow_acquire(window);
}


losSize AndroidWindow::getWindowSize() noexcept {
    return {ANativeWindow_getWidth(window),ANativeWindow_getHeight(window)};
}

bool AndroidWindow::hasWindowClosed() const noexcept
{
    return true;
}

losResult AndroidWindow::losUpdateWindow() noexcept
{
    return LOS_ERROR_FEATURE_NOT_IMPLEMENTED;
}

bool AndroidWindow::losIsKeyDown(losKeyboardButton) const noexcept
{
    return false;
}

bool AndroidWindow::losIsMouseDown(losMouseButton) const noexcept
{
    return false;
}

losResult AndroidWindow::losRequestClose() noexcept
{
    return LOS_ERROR_FEATURE_NOT_IMPLEMENTED;
}

losSize AndroidWindow::losRequestMousePosition() const noexcept
{
    return {};
}

losSize AndroidWindow::losRequestMouseWheelDelta() const noexcept
{
    return {};
}

losSize AndroidWindow::losIsBeingPressed() const noexcept
{
    return {};
}

void AndroidWindow::losDestroyWindow() noexcept
{
    ANativeWindow_release(window);
}

void* AndroidWindow::internalGet() noexcept
{
    return nullptr;
}