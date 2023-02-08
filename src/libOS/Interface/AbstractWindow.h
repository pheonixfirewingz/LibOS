#pragma once
// LIBOS LICENCE
//
// GNU Lesser General Public License Version 3.0
//
// Copyright Luke Shore (c) 2020, 2023
#include <libos/Window.h>
#include <libos/NativeWindow.h>
#include <string>
struct BaseWindow
{
    virtual losUsedWindowAPI getType() const noexcept = 0;
    virtual bool hasWindowClosed() const noexcept = 0;
    virtual losResult losUpdateWindow() noexcept = 0;
    virtual bool losIsKeyDown(losKeyboardButton) const noexcept = 0;
    virtual bool losIsMouseDown(losMouseButton) const noexcept = 0;
    virtual losResult losRequestClose() noexcept = 0;
    virtual losSize losRequestMousePosition() const noexcept = 0;
    virtual losSize losRequestMouseWheelDelta() const noexcept = 0;
    virtual losSize losIsBeingPressed() const noexcept = 0;
    virtual void losDestroyWindow() noexcept = 0;
    virtual void *internalGet() noexcept = 0;
    virtual losSize getWindowSize() noexcept = 0;
    virtual bool hasFailed() const noexcept = 0;
    virtual ~BaseWindow() = default;
};