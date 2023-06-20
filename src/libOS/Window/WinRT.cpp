#include "Android.hpp"
// LIBOS LICENCE
//
// GNU Lesser General Public License Version 3.0
//
// Copyright Luke Shore (c) 2020, 2023

WinRTWindow::WinRTWindow(const std::string, losSize) noexcept
{
    puts("[LIBOS] <INFO> -> creating WINRT API Window");
    error = true;
}

bool WinRTWindow::hasWindowClosed() const noexcept
{
    return true;
}

losResult WinRTWindow::losUpdateWindow() noexcept
{
    return LOS_ERROR_FEATURE_NOT_IMPLEMENTED;
}

bool WinRTWindow::losIsKeyDown(losKeyboardButton) const noexcept
{
    return false;
}

bool WinRTWindow::losIsMouseDown(losMouseButton) const noexcept
{
    return false;
}

losResult WinRTWindow::losRequestClose() noexcept
{
    return LOS_ERROR_FEATURE_NOT_IMPLEMENTED;
}

losSize WinRTWindow::losRequestMousePosition() const noexcept
{
    return {};
}

losSize WinRTWindow::losRequestMouseWheelDelta() const noexcept
{
    return {};
}

losSize WinRTWindow::losIsBeingPressed() const noexcept
{
    return {};
}

void WinRTWindow::losDestroyWindow() noexcept
{
}

void* WinRTWindow::internalGet() noexcept
{
    return nullptr;
}