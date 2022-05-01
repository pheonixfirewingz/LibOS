#pragma once
#include "Defines.h"
DEFINE_HANDLE(losWindow)

typedef enum losMouseButton : uint8
{
    LOS_RIGHT_MOUSE_BUTTON = 0,
    LOS_MIDDLE_MOUSE_BUTTON = 1,
    LOS_LEFT_MOUSE_BUTTON = 2,
} losMouseButton;

typedef enum losKeyboardButton : uint16
{
    LOS_KEYBOARD_0,
    LOS_KEYBOARD_1,
    LOS_KEYBOARD_2,
    LOS_KEYBOARD_3,
    LOS_KEYBOARD_4,
    LOS_KEYBOARD_5,
    LOS_KEYBOARD_6,
    LOS_KEYBOARD_7,
    LOS_KEYBOARD_8,
    LOS_KEYBOARD_9,
    LOS_KEYBOARD_A,
    LOS_KEYBOARD_B,
    LOS_KEYBOARD_C,
    LOS_KEYBOARD_D,
    LOS_KEYBOARD_E,
    LOS_KEYBOARD_F,
    LOS_KEYBOARD_G,
    LOS_KEYBOARD_H,
    LOS_KEYBOARD_I,
    LOS_KEYBOARD_J,
    LOS_KEYBOARD_K,
    LOS_KEYBOARD_L,
    LOS_KEYBOARD_M,
    LOS_KEYBOARD_N,
    LOS_KEYBOARD_O,
    LOS_KEYBOARD_P,
    LOS_KEYBOARD_Q,
    LOS_KEYBOARD_R,
    LOS_KEYBOARD_S,
    LOS_KEYBOARD_T,
    LOS_KEYBOARD_U,
    LOS_KEYBOARD_V,
    LOS_KEYBOARD_W,
    LOS_KEYBOARD_X,
    LOS_KEYBOARD_Y,
    LOS_KEYBOARD_Z,
    LOS_KEYBOARD_APOSTROPHE,
    LOS_KEYBOARD_BACKSLASH,
    LOS_KEYBOARD_COMMA,
    LOS_KEYBOARD_GRAVE_ACCENT,
    LOS_KEYBOARD_LEFT_BRACKET,
    LOS_KEYBOARD_MINUS,
    LOS_KEYBOARD_PERIOD,
    LOS_KEYBOARD_RIGHT_BRACKET,
    LOS_KEYBOARD_SEMICOLON,
    LOS_KEYBOARD_SLASH,
    LOS_KEYBOARD_WORLD_2,
    LOS_KEYBOARD_BACKSPACE,
    LOS_KEYBOARD_DELETE,
    LOS_KEYBOARD_END,
    LOS_KEYBOARD_ENTER,
    LOS_KEYBOARD_ESCAPE,
    LOS_KEYBOARD_HOME,
    LOS_KEYBOARD_INSERT,
    LOS_KEYBOARD_MENU,
    LOS_KEYBOARD_PAGE_DOWN,
    LOS_KEYBOARD_PAGE_UP,
    LOS_KEYBOARD_PAUSE,
    LOS_KEYBOARD_SPACE,
    LOS_KEYBOARD_TAB,
    LOS_KEYBOARD_CAPS_LOCK,
    LOS_KEYBOARD_NUM_LOCK,
    LOS_KEYBOARD_SCROLL_LOCK,
    LOS_KEYBOARD_F1,
    LOS_KEYBOARD_F2,
    LOS_KEYBOARD_F3,
    LOS_KEYBOARD_F4,
    LOS_KEYBOARD_F5,
    LOS_KEYBOARD_F6,
    LOS_KEYBOARD_F7,
    LOS_KEYBOARD_F8,
    LOS_KEYBOARD_F9,
    LOS_KEYBOARD_F10,
    LOS_KEYBOARD_F11,
    LOS_KEYBOARD_F12,
    LOS_KEYBOARD_F13,
    LOS_KEYBOARD_F14,
    LOS_KEYBOARD_F15,
    LOS_KEYBOARD_F16,
    LOS_KEYBOARD_F17,
    LOS_KEYBOARD_F18,
    LOS_KEYBOARD_F19,
    LOS_KEYBOARD_F20,
    LOS_KEYBOARD_F21,
    LOS_KEYBOARD_F22,
    LOS_KEYBOARD_F23,
    LOS_KEYBOARD_F24,
    LOS_KEYBOARD_ALT,
    LOS_KEYBOARD_LEFT_ALT,
    LOS_KEYBOARD_RIGHT_ALT,
    LOS_KEYBOARD_CONTROL,
    LOS_KEYBOARD_LEFT_CONTROL,
    LOS_KEYBOARD_RIGHT_CONTROL,
    LOS_KEYBOARD_SHIFT,
    LOS_KEYBOARD_LEFT_SHIFT,
    LOS_KEYBOARD_RIGHT_SHIFT,
    LOS_KEYBOARD_LEFT_SUPER,
    LOS_KEYBOARD_RIGHT_SUPER,
    LOS_KEYBOARD_PRINT_SCREEN,
    LOS_KEYBOARD_DOWN,
    LOS_KEYBOARD_LEFT,
    LOS_KEYBOARD_RIGHT, 
    LOS_KEYBOARD_UP,
    LOS_KEYBOARD_NUMPAD_0,
    LOS_KEYBOARD_NUMPAD_1,
    LOS_KEYBOARD_NUMPAD_2,
    LOS_KEYBOARD_NUMPAD_3,
    LOS_KEYBOARD_NUMPAD_4,
    LOS_KEYBOARD_NUMPAD_5,
    LOS_KEYBOARD_NUMPAD_6,
    LOS_KEYBOARD_NUMPAD_7,
    LOS_KEYBOARD_NUMPAD_8,
    LOS_KEYBOARD_NUMPAD_9,
    LOS_KEYBOARD_ADD,
    LOS_KEYBOARD_DECIMAL,
    LOS_KEYBOARD_DIVIDE,
    LOS_KEYBOARD_EQUAL,
    LOS_KEYBOARD_MULTIPLY,
    LOS_KEYBOARD_SUBTRACT,
} losKeyboardButton;

typedef struct losWindowInfo
{
    bool subWindow;
    losSize windowSize;
    char* title;
    size title_size;
} losWindowInfo;

typedef struct losPostition
{
    uint64 x;
    uint64 y;
} losPostition;

losResult losCreateWindow(losWindow *, losWindowInfo &);
losResult losCreateKeyboard(losWindow);
losResult losCreateMouse(losWindow);
losResult losCreateTouch(losWindow);

losResult losUpdateWindow(losWindow);
bool losIsKeyDown(losWindow, losKeyboardButton);
bool losIsMouseDown(losWindow, losMouseButton);
losResult losRequestClose(losWindow);
losPostition losRequestMousePosition(losWindow);
losPostition losIsBeingPressed(losWindow);

losResult losDestoryKeyboard(losWindow);
losResult losDestoryMouse(losWindow);
losResult losDestoryTouch(losWindow);
losResult losDestoryWindow(losWindow);