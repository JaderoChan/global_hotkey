#ifndef KEYBOARD_TOOLS_KEY_UTILITYS_HPP
#define KEYBOARD_TOOLS_KEY_UTILITYS_HPP

#include <cstdint>  // uint32_t
#include <string>

#include "defines.hpp"

namespace kbdt
{

enum Key : int32_t
{
    // Number keys
    Key_0       = 0x0030,
    Key_1       = 0x0031,
    Key_2       = 0x0032,
    Key_3       = 0x0033,
    Key_4       = 0x0034,
    Key_5       = 0x0035,
    Key_6       = 0x0036,
    Key_7       = 0x0037,
    Key_8       = 0x0038,
    Key_9       = 0x0039,

    // ASCII letter keys
    Key_A       = 0x0041,
    Key_B       = 0x0042,
    Key_C       = 0x0043,
    Key_D       = 0x0044,
    Key_E       = 0x0045,
    Key_F       = 0x0046,
    Key_G       = 0x0047,
    Key_H       = 0x0048,
    Key_I       = 0x0049,
    Key_J       = 0x004a,
    Key_K       = 0x004b,
    Key_L       = 0x004c,
    Key_M       = 0x004d,
    Key_N       = 0x004e,
    Key_O       = 0x004f,
    Key_P       = 0x0050,
    Key_Q       = 0x0051,
    Key_R       = 0x0052,
    Key_S       = 0x0053,
    Key_T       = 0x0054,
    Key_U       = 0x0055,
    Key_V       = 0x0056,
    Key_W       = 0x0057,
    Key_X       = 0x0058,
    Key_Y       = 0x0059,
    Key_Z       = 0x005a,

    // Whitespace keys
    Key_Tab     = 0x8000,
    Key_Space,
    Key_Enter,

    // Navigation keys
    Key_Home,
    Key_End,
    Key_Page_Up,
    Key_Page_Down,
    Key_Left,
    Key_Up,
    Key_Right,
    Key_Down,

    // Editing keys
    Key_Backspace,
    Key_Insert,
    Key_Delete,
    Key_Clear,

    // Function keys
    Key_F1,
    Key_F2,
    Key_F3,
    Key_F4,
    Key_F5,
    Key_F6,
    Key_F7,
    Key_F8,
    Key_F9,
    Key_F10,
    Key_F11,
    Key_F12,
    Key_F13,
    Key_F14,
    Key_F15,
    Key_F16,
    Key_F17,
    Key_F18,
    Key_F19,
    Key_F20,
    Key_F21,
    Key_F22,
    Key_F23,
    Key_F24,

    // Numpad number keys
    Key_Numpad_0,
    Key_Numpad_1,
    Key_Numpad_2,
    Key_Numpad_3,
    Key_Numpad_4,
    Key_Numpad_5,
    Key_Numpad_6,
    Key_Numpad_7,
    Key_Numpad_8,
    Key_Numpad_9,

    // Numpad operator keys
    Key_Numpad_Add,         // '+' on numpad
    Key_Numpad_Subtract,    // '-' on numpad
    Key_Numpad_Multiply,    // '*' or '×' on numpad
    Key_Numpad_Divide,      // '/' or '÷' on numpad
    Key_Numpad_Decimal,     // '.' on numpad
    Key_Numpad_Separator,   // ',' on numpad
    Key_Numpad_Equal,       // '=' on numpad
    Key_Numpad_Enter,       // Enter on numpad

    // Applications keys
    Key_Escape,
    Key_Pause,              // Not is Key_Media_Play_Pause
    Key_Play,               // Not is Key_Media_Play_Pause
    Key_Help,
    Key_Menu,

    // Media keys
    Key_Media_Next,
    Key_Media_Previous,
    Key_Media_Play_Pause,
    Key_Media_Stop,

    // Volume keys
    Key_Volume_Up,
    Key_Volume_Down,
    Key_Volume_Mute,

    // Lock keys
    Key_Caps_Lock,
    Key_Num_Lock,
    Key_Scroll_Lock,

    // Device keys
    Key_Print_Screen,
    Key_Sleep,

    // OEM (Original Equipment Manufacturer) keys
    // The specific values of the following keys may vary on different devices.
    // All of the following uses the standard keyboard layout of the United States.
    Key_Left_Quote,         // '`~' on US standard keyboard
    Key_Minus,              // '-_' on US standard keyboard
    Key_Equal,              // '=+' on US standard keyboard
    Key_Left_Bracket,       // '[{' on US standard keyboard
    Key_Right_Bracket,      // ']}' on US standard keyboard
    Key_Semicolon,          // ';:' on US standard keyboard
    Key_Apostrophe,         // ''"' on US standard keyboard
    Key_Comma,              // ',<' on US standard keyboard
    Key_Period,             // '.>' on US standard keyboard
    Key_Slash,              // '/?' on US standard keyboard
    Key_Backslash,          // '\|' on US standard keyboard
    Key_Angle_Bracket,      // '<>' or '\|' on RT 102-key keyboard

    // Modifier keys
    Key_Meta,
    Key_Meta_Left,
    Key_Meta_Right,
    Key_Ctrl,
    Key_Ctrl_Left,
    Key_Ctrl_Right,
    Key_Alt,
    Key_Alt_Left,
    Key_Alt_Right,
    Key_Shift,
    Key_Shift_Left,
    Key_Shift_Right,

    // Alias
    Key_Return              = Key_Enter,
    Key_Esc                 = Key_Escape,
    Key_Numpad_Plus         = Key_Numpad_Add,
    Key_Numpad_Minus        = Key_Numpad_Subtract,
    Key_Numpad_Return       = Key_Numpad_Enter,
    Key_Standby             = Key_Sleep,
    Key_Grave               = Key_Left_Quote,

    Key_Option              = Key_Alt,
    Key_Option_Left         = Key_Alt_Left,
    Key_Option_Right        = Key_Alt_Right
};

KEYBOARD_TOOLS_API const char* keyToStr(Key key) noexcept;

KEYBOARD_TOOLS_API Key keyFromStr(const char* str) noexcept;

KEYBOARD_TOOLS_API Key keyFromStr(const std::string& str) noexcept;

KEYBOARD_TOOLS_API int keyToNativeKey(Key key) noexcept;

KEYBOARD_TOOLS_API Key keyFromNativeKey(int nativeKey) noexcept;

} // namespace kbdt

#endif // !KEYBOARD_TOOLS_KEY_UTILITYS_HPP
