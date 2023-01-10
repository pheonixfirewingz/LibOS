/*!
 * \file Window.h
 * \author Luke Shore <lukethemodded@gmail.com>
 * \version 1.0
 * \date 05/12/2022
 * \brief WINDOW is a simple cross-platform API for accessing the native platform window, keyboard and mouse io
 */
#pragma once
// LIBOS LICENCE
//
// GNU Lesser General Public License Version 3.0
//
// Copyright Luke Shore (c) 2020, 2023
/*! Importation of libraries*/
#include <libos/Defines.h>
#include <libos/Window.h>
/*!
 *  \brief losUsedWindowAPI is returned by losWhatApiIsWindowUsed to query the library what native api is current use
 */
typedef enum losUsedWindowAPI
{
    WIN32_API,  //!< window is in win32 mode
    WINRT_API,  //!< window is in winRT mode
    XCB_API,    //!< window is in xorg - xcb mode
    WAYLAND_API //!< window is in wayland mode
} losUsedWindowAPI;

#if __has_include(<wayland-client.h>)
/*!
 *  \brief losWindowWayland is the struct we pass to the library user if the underlying window is running wayland
 */
typedef struct losWindowWayland
{
    void *display; //!< display is a wl_display* under the hud
    void *surface; //!< surface is a wl_surface* under the hud
} losWindowWayland;
#endif
#if __has_include(<xcb/xcb.h>)
/*!
 *  \brief losWindowXCB is the struct we pass to the library user if the underlying window is running Xcb api
 */
typedef struct losWindowXCB
{
    void *connection; //!< connection is a xcb_connection_t* under the hud
    void *window;     //!< window is a xcb_window_t* under the hud
} losWindowXCB;
#endif
#if __has_include(<windows.h>)
/*!
 *  \brief losWindowWin32 is the struct we pass to the library user if the underlying window is running the win32 api
 */
typedef struct losWindowWin32
{
    void *window; //!< window is a HWND under the hud
    explicit losWindowWin32(void *window)
        : window(window){};
} losWindowWin32;
#endif
/*!
 *  \brief losGetWindowNativePointer gets the windows currently active api in use
 */
EXPORT_DLL losUsedWindowAPI losWhatApiIsWindowUsed(losWindow window);
/*!
 *  \brief losGetWindowNativePointer gets the windows native controls
 */
EXPORT_DLL void *losGetWindowNativePointer(losWindow window);
