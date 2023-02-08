#pragma once
// LIBOS LICENCE
//
// GNU Lesser General Public License Version 3.0
//
// Copyright Luke Shore (c) 2020, 2023
#include "../Interface/AbstractWindow.h"
#ifdef ON_UWP
#    include <winrt/windows.applicationmodel.h>
#endif
class WinRTWindow : public BaseWindow
{
#ifdef ON_UWP
    bool error = false;
#endif
  public:
#ifdef ON_UWP
    explicit WinRTWindow(const std::string title, losSize win_size) noexcept;
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
    explicit WinRTWindow(const std::string, losSize) noexcept
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
        return WINRT_API;
    }
};
