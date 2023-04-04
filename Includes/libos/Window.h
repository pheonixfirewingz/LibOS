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
/*!
 *  \brief this is the Library object that is used to Control the Native  as the
 * user only holds a pointer to the library object that is controlled by the library.
 */
typedef struct losWindow_T *losWindow;
/*!
 *  \brief losMouseButton is the programmatically encoded form of the platforms mouse buttons
 */
typedef enum losMouseButton
{
    LOS_RIGHT_BUTTON,
    LOS_MIDDLE_BUTTON,
    LOS_LEFT_BUTTON,
} losMouseButton;
/*!
 *  \brief losKeyboardButton is the programmatically encoded form of the platforms keyboard keys
 */
typedef enum losKeyboardButton
{
    LOS_KEY_0,             //!< UTF-8 (0)
    LOS_KEY_1,             //!< UTF-8 (1)
    LOS_KEY_2,             //!< UTF-8 (2)
    LOS_KEY_3,             //!< UTF-8 (3)
    LOS_KEY_4,             //!< UTF-8 (4)
    LOS_KEY_5,             //!< UTF-8 (5)
    LOS_KEY_6,             //!< UTF-8 (6)
    LOS_KEY_7,             //!< UTF-8 (7)
    LOS_KEY_8,             //!< UTF-8 (8)
    LOS_KEY_9,             //!< UTF-8 (9)
    LOS_KEY_A,             //!< UTF-8 (a)
    LOS_KEY_B,             //!< UTF-8 (b)
    LOS_KEY_C,             //!< UTF-8 (c)
    LOS_KEY_D,             //!< UTF-8 (d)
    LOS_KEY_E,             //!< UTF-8 (e)
    LOS_KEY_F,             //!< UTF-8 (f)
    LOS_KEY_G,             //!< UTF-8 (g)
    LOS_KEY_H,             //!< UTF-8 (h)
    LOS_KEY_I,             //!< UTF-8 (i)
    LOS_KEY_J,             //!< UTF-8 (j)
    LOS_KEY_K,             //!< UTF-8 (k)
    LOS_KEY_L,             //!< UTF-8 (l)
    LOS_KEY_M,             //!< UTF-8 (m)
    LOS_KEY_N,             //!< UTF-8 (n)
    LOS_KEY_O,             //!< UTF-8 (o)
    LOS_KEY_P,             //!< UTF-8 (p)
    LOS_KEY_Q,             //!< UTF-8 (q)
    LOS_KEY_R,             //!< UTF-8 (r)
    LOS_KEY_S,             //!< UTF-8 (s)
    LOS_KEY_T,             //!< UTF-8 (t)
    LOS_KEY_U,             //!< UTF-8 (u)
    LOS_KEY_V,             //!< UTF-8 (v)
    LOS_KEY_W,             //!< UTF-8 (w)
    LOS_KEY_X,             //!< UTF-8 (x)
    LOS_KEY_Y,             //!< UTF-8 (y)
    LOS_KEY_Z,             //!< UTF-8 (z)
    LOS_KEY_APOSTROPHE,    //!< UTF-8 (')
    LOS_KEY_BACKSLASH,     //!< UTF-8 (\)
    LOS_KEY_COMMA,         //!< UTF-8 (,)
    LOS_KEY_LEFT_BRACKET,  //!< UTF-8 ('(')
    LOS_KEY_MINUS,         //!< UTF-8 (-)
    LOS_KEY_PERIOD,        //!< UTF-8 (.)
    LOS_KEY_RIGHT_BRACKET, //!< UTF-8 (')')
    LOS_KEY_SEMICOLON,     //!< UTF-8 (;)
    LOS_KEY_SLASH,         //!< UTF-8 (/)
    LOS_KEY_BACKSPACE,     //!< UTF-8 ('<-')
    LOS_KEY_DELETE,        //!< UTF-8 (delete)
    LOS_KEY_END,           //!< UTF-8 (end)
    LOS_KEY_ENTER,         //!< UTF-8 (return)
    LOS_KEY_ESCAPE,        //!< UTF-8 (esc)
    LOS_KEY_HOME,          //!< UTF-8 (home)
    LOS_KEY_INSERT,        //!< UTF-8 (insert)
    LOS_KEY_PAGE_DOWN,     //!< UTF-8 (/\)
    LOS_KEY_PAGE_UP,       //!< UTF-8 (\/)
    LOS_KEY_PAUSE,         //!< UTF-8 (pause)
    LOS_KEY_SPACE,         //!< UTF-8 (' ')
    LOS_KEY_TAB,           //!< UTF-8 ('   ')
    LOS_KEY_CAPS_LOCK,     //!< UTF-8 (cap_lock)
    LOS_KEY_NUM_LOCK,      //!< UTF-8 (num_lock)
    LOS_KEY_SCROLL_LOCK,   //!< UTF-8 (scroll_lock)
    LOS_KEY_F1,            //!< UTF-8 (f1)
    LOS_KEY_F2,            //!< UTF-8 (f2)
    LOS_KEY_F3,            //!< UTF-8 (f3)
    LOS_KEY_F4,            //!< UTF-8 (f4)
    LOS_KEY_F5,            //!< UTF-8 (f5)
    LOS_KEY_F6,            //!< UTF-8 (f6)
    LOS_KEY_F7,            //!< UTF-8 (f7)
    LOS_KEY_F8,            //!< UTF-8 (f8)
    LOS_KEY_F9,            //!< UTF-8 (f9)
    LOS_KEY_F10,           //!< UTF-8 (f10)
    LOS_KEY_F11,           //!< UTF-8 (f11)
    LOS_KEY_F12,           //!< UTF-8 (f12)
    LOS_KEY_ALT,           //!< UTF-8 (alt left)
    LOS_KEY_LEFT_ALT,      //!< UTF-8 (alt left)
    LOS_KEY_RIGHT_ALT,     //!< UTF-8 (alt right)
    LOS_KEY_CONTROL,       //!< UTF-8 (control left)
    LOS_KEY_LEFT_CONTROL,  //!< UTF-8 (control left)
    LOS_KEY_RIGHT_CONTROL, //!< UTF-8 (control right)
    LOS_KEY_SHIFT,         //!< UTF-8 (shift left)
    LOS_KEY_LEFT_SHIFT,    //!< UTF-8 (shift left)
    LOS_KEY_RIGHT_SHIFT,   //!< UTF-8 (shift right)
    LOS_KEY_LEFT_SUPER,    //!< UTF-8 (win left)
    LOS_KEY_RIGHT_SUPER,   //!< UTF-8 (win right)
    LOS_KEY_PRINT_SCREEN,  //!< UTF-8 (print_screen)
    LOS_KEY_DOWN,          //!< UTF-8 (\/)
    LOS_KEY_LEFT,          //!< UTF-8 ('->')
    LOS_KEY_RIGHT,         //!< UTF-8 ('<-')
    LOS_KEY_UP,            //!< UTF-8 (/\)
    LOS_KEY_NUM_PAD_0,     //!< UTF-8 (0)
    LOS_KEY_NUM_PAD_1,     //!< UTF-8 (1)
    LOS_KEY_NUM_PAD_2,     //!< UTF-8 (2)
    LOS_KEY_NUM_PAD_3,     //!< UTF-8 (3)
    LOS_KEY_NUM_PAD_4,     //!< UTF-8 (4)
    LOS_KEY_NUM_PAD_5,     //!< UTF-8 (5)
    LOS_KEY_NUM_PAD_6,     //!< UTF-8 (6)
    LOS_KEY_NUM_PAD_7,     //!< UTF-8 (7)
    LOS_KEY_NUM_PAD_8,     //!< UTF-8 (8)
    LOS_KEY_NUM_PAD_9,     //!< UTF-8 (9)
    LOS_KEY_ADD,           //!< UTF-8 (+)
    LOS_KEY_DECIMAL,       //!< UTF-8 (.)
    LOS_KEY_DIVIDE,        //!< UTF-8 (/)
    LOS_KEY_EQUAL,         //!< UTF-8 (=)
    LOS_KEY_MULTIPLY,      //!< UTF-8 (*)
    LOS_KEY_SUBTRACT,      //!< UTF-8 (-)
} losKeyboardButton;
/*!
 *  \brief losWindowInfo is the window creation struct we pass this into the creation function for the library to create
 * a window object
 */
typedef struct losWindowInfo
{
    losSize window_size;    //!< this size of the window (width|height)
    const char *title;      //!< this is the title string
    size_t title_size; //!< this is the title string size
} losWindowInfo;
/*!
 * \param[out] window_handle
 * \param[in] window_info
 * \return losResult
 * \brief losCreateWindow is to create the library object but dose not release the object to the library user
 */
EXPORT_DLL enum losResult losCreateWindow(losWindow *window_handle, losWindowInfo window_info);
/*!
 * \param[in] window_handle
 * \return losResult
 * \brief losUpdateWindow is used to pump the event loop of the native window 1 call = 1 pump
 * \note the way to check if the window should close check to see if function returns "LOS_WINDOW_CLOSE"
 */
EXPORT_DLL enum losResult losUpdateWindow(losWindow window_handle);
/*!
 * \param[in] window_handle
 * \return losSize
 * \brief losUpdateWindow is used to get the current window size of the window at the time of the request
 */
EXPORT_DLL struct losSize losRequestWindowSize(losWindow window_handle);
/*!
 * \param[in] window_handle
 * \param[in] button
 * \return uint8_t
 * \brief losIsKeyDown is used for the library user to ask if a keyboard button is active
 */
EXPORT_DLL uint8_t losIsKeyDown(losWindow window_handle, losKeyboardButton button);
/*!
 * \param[in] window_handle
 * \param[in] button
 * \return uint8_t
 * \brief losIsMouseDown is used for the library user to ask if a mouse button is active
 */
EXPORT_DLL uint8_t losIsMouseDown(losWindow window_handle, losMouseButton button);
/*!
 * \param[in] window_handle
 * \return losResult
 * \brief losIsMouseDown is used for the library user to ask the window to close
 */
EXPORT_DLL enum losResult losRequestClose(losWindow window_handle);
/*!
 * \param[in] window_handle
 * \return losSize
 * \brief losRequestMousePosition is used for the library user to ask where the mouse is in the window
 */
EXPORT_DLL struct losSize losRequestMousePosition(losWindow window_handle);
/*!
 * \param[in] window_handle
 * \return losSize
 * \brief losRequestMouseWheelDelta is used for the library user to ask for the mouse wheel delta
 * \note this is note implemented on all platforms as of V1.0.0
 */
EXPORT_DLL struct losSize losRequestMouseWheelDelta(losWindow window_handle);
/*!
 * \param[in] window_handle
 * \return losSize
 * \brief losRequestMousePosition is used for the library user to ask where the screen is being touched
 * \note this is note implemented on platforms as of V1.0.0
 */
EXPORT_DLL struct losSize losIsBeingPressed(losWindow window_handle);
/*!
 * \param[in] window_handle
 * \brief losDestroyWindow is to close the library object and safely delete it
 */
EXPORT_DLL void losDestroyWindow(losWindow window_handle);
