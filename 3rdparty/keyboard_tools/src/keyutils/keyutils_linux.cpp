#include <kbdt/keyutils.hpp>

#include <linux/input-event-codes.h>

namespace kbdt
{

KBDT_API int keyToNativeKey(Key key) noexcept
{
    switch (key)
    {
        // Number keys
        case Key_0:                 return KEY_0;
        case Key_1:                 return KEY_1;
        case Key_2:                 return KEY_2;
        case Key_3:                 return KEY_3;
        case Key_4:                 return KEY_4;
        case Key_5:                 return KEY_5;
        case Key_6:                 return KEY_6;
        case Key_7:                 return KEY_7;
        case Key_8:                 return KEY_8;
        case Key_9:                 return KEY_9;

        // ASCII letter keys
        case Key_A:                 return KEY_A;
        case Key_B:                 return KEY_B;
        case Key_C:                 return KEY_C;
        case Key_D:                 return KEY_D;
        case Key_E:                 return KEY_E;
        case Key_F:                 return KEY_F;
        case Key_G:                 return KEY_G;
        case Key_H:                 return KEY_H;
        case Key_I:                 return KEY_I;
        case Key_J:                 return KEY_J;
        case Key_K:                 return KEY_K;
        case Key_L:                 return KEY_L;
        case Key_M:                 return KEY_M;
        case Key_N:                 return KEY_N;
        case Key_O:                 return KEY_O;
        case Key_P:                 return KEY_P;
        case Key_Q:                 return KEY_Q;
        case Key_R:                 return KEY_R;
        case Key_S:                 return KEY_S;
        case Key_T:                 return KEY_T;
        case Key_U:                 return KEY_U;
        case Key_V:                 return KEY_V;
        case Key_W:                 return KEY_W;
        case Key_X:                 return KEY_X;
        case Key_Y:                 return KEY_Y;
        case Key_Z:                 return KEY_Z;

        // Whitespace keys
        case Key_Tab:               return KEY_TAB;
        case Key_Space:             return KEY_SPACE;
        case Key_Enter:             return KEY_ENTER;

        // Navigation keys
        case Key_Home:              return KEY_HOME;
        case Key_End:               return KEY_END;
        case Key_Page_Up:           return KEY_PAGEUP;
        case Key_Page_Down:         return KEY_PAGEDOWN;
        case Key_Left:              return KEY_LEFT;
        case Key_Up:                return KEY_UP;
        case Key_Right:             return KEY_RIGHT;
        case Key_Down:              return KEY_DOWN;

        // Editing keys
        case Key_Backspace:         return KEY_BACKSPACE;
        case Key_Insert:            return KEY_INSERT;
        case Key_Delete:            return KEY_DELETE;
        case Key_Clear:             return KEY_CLEAR;

        // Function keys
        case Key_F1:                return KEY_F1;
        case Key_F2:                return KEY_F2;
        case Key_F3:                return KEY_F3;
        case Key_F4:                return KEY_F4;
        case Key_F5:                return KEY_F5;
        case Key_F6:                return KEY_F6;
        case Key_F7:                return KEY_F7;
        case Key_F8:                return KEY_F8;
        case Key_F9:                return KEY_F9;
        case Key_F10:               return KEY_F10;
        case Key_F11:               return KEY_F11;
        case Key_F12:               return KEY_F12;
        case Key_F13:               return KEY_F13;
        case Key_F14:               return KEY_F14;
        case Key_F15:               return KEY_F15;
        case Key_F16:               return KEY_F16;
        case Key_F17:               return KEY_F17;
        case Key_F18:               return KEY_F18;
        case Key_F19:               return KEY_F19;
        case Key_F20:               return KEY_F20;
        case Key_F21:               return KEY_F21;
        case Key_F22:               return KEY_F22;
        case Key_F23:               return KEY_F23;
        case Key_F24:               return KEY_F24;

        // Numpad number keys
        case Key_Numpad_0:          return KEY_KP0;
        case Key_Numpad_1:          return KEY_KP1;
        case Key_Numpad_2:          return KEY_KP2;
        case Key_Numpad_3:          return KEY_KP3;
        case Key_Numpad_4:          return KEY_KP4;
        case Key_Numpad_5:          return KEY_KP5;
        case Key_Numpad_6:          return KEY_KP6;
        case Key_Numpad_7:          return KEY_KP7;
        case Key_Numpad_8:          return KEY_KP8;
        case Key_Numpad_9:          return KEY_KP9;

        // Numpad operator keys
        case Key_Numpad_Add:        return KEY_KPPLUS;
        case Key_Numpad_Subtract:   return KEY_KPMINUS;
        case Key_Numpad_Multiply:   return KEY_KPASTERISK;
        case Key_Numpad_Divide:     return KEY_KPSLASH;
        case Key_Numpad_Decimal:    return KEY_KPDOT;
        case Key_Numpad_Separator:  return KEY_KPCOMMA;
        case Key_Numpad_Equal:      return KEY_KPEQUAL;
        case Key_Numpad_Enter:      return KEY_KPENTER;

        // Application keys
        case Key_Escape:            return KEY_ESC;
        case Key_Pause:             return KEY_PAUSE;
        case Key_Play:              return KEY_PLAY;
        case Key_Help:              return KEY_HELP;
        case Key_Menu:              return KEY_MENU;

        // Media keys
        case Key_Media_Next:        return KEY_NEXTSONG;
        case Key_Media_Previous:    return KEY_PREVIOUSSONG;
        case Key_Media_Play_Pause:  return KEY_PLAYPAUSE;
        case Key_Media_Stop:        return KEY_STOPCD;

        // Volume keys
        case Key_Volume_Up:         return KEY_VOLUMEUP;
        case Key_Volume_Down:       return KEY_VOLUMEDOWN;
        case Key_Volume_Mute:       return KEY_MUTE;

        // Lock keys
        case Key_Caps_Lock:         return KEY_CAPSLOCK;
        case Key_Num_Lock:          return KEY_NUMLOCK;
        case Key_Scroll_Lock:       return KEY_SCROLLLOCK;

        // Device keys
        case Key_Print_Screen:      return KEY_PRINT;    // Need to check
        case Key_Sleep:             return KEY_SLEEP;

        // OEM (Original Equipment Manufacturer) keys
        // The specific values of the following keys may vary on different devices.
        // All of the following uses the standard keyboard layout of the United States.
        case Key_Left_Quote:        return KEY_GRAVE;
        case Key_Minus:             return KEY_MINUS;
        case Key_Equal:             return KEY_EQUAL;
        case Key_Left_Bracket:      return KEY_LEFTBRACE;
        case Key_Right_Bracket:     return KEY_RIGHTBRACE;
        case Key_Semicolon:         return KEY_SEMICOLON;
        case Key_Apostrophe:        return KEY_APOSTROPHE;
        case Key_Comma:             return KEY_COMMA;
        case Key_Period:            return KEY_DOT;
        case Key_Slash:             return KEY_SLASH;
        case Key_Backslash:         return KEY_BACKSLASH;
        case Key_Angle_Bracket:     return KEY_102ND;     // Need to check

        // Modifier keys
        case Key_Meta:          return 0;   // Not supported
        case Key_Meta_Left:     return KEY_LEFTMETA;
        case Key_Meta_Right:    return KEY_RIGHTMETA;
        case Key_Ctrl:          return 0;   // Not supported
        case Key_Ctrl_Left:     return KEY_LEFTCTRL;
        case Key_Ctrl_Right:    return KEY_RIGHTCTRL;
        case Key_Alt:           return 0;   // Not supported
        case Key_Alt_Left:      return KEY_LEFTALT;
        case Key_Alt_Right:     return KEY_RIGHTALT;
        case Key_Shift:         return 0;   // Not supported
        case Key_Shift_Left:    return KEY_LEFTSHIFT;
        case Key_Shift_Right:   return KEY_RIGHTSHIFT;

        default:                    return 0;
    }
}

KBDT_API Key keyFromNativeKey(int nativeKey) noexcept
{
    switch (nativeKey)
    {
        // Number keys
        case KEY_0:                 return Key_0;
        case KEY_1:                 return Key_1;
        case KEY_2:                 return Key_2;
        case KEY_3:                 return Key_3;
        case KEY_4:                 return Key_4;
        case KEY_5:                 return Key_5;
        case KEY_6:                 return Key_6;
        case KEY_7:                 return Key_7;
        case KEY_8:                 return Key_8;
        case KEY_9:                 return Key_9;

        // ASCII letter keys
        case KEY_A:                 return Key_A;
        case KEY_B:                 return Key_B;
        case KEY_C:                 return Key_C;
        case KEY_D:                 return Key_D;
        case KEY_E:                 return Key_E;
        case KEY_F:                 return Key_F;
        case KEY_G:                 return Key_G;
        case KEY_H:                 return Key_H;
        case KEY_I:                 return Key_I;
        case KEY_J:                 return Key_J;
        case KEY_K:                 return Key_K;
        case KEY_L:                 return Key_L;
        case KEY_M:                 return Key_M;
        case KEY_N:                 return Key_N;
        case KEY_O:                 return Key_O;
        case KEY_P:                 return Key_P;
        case KEY_Q:                 return Key_Q;
        case KEY_R:                 return Key_R;
        case KEY_S:                 return Key_S;
        case KEY_T:                 return Key_T;
        case KEY_U:                 return Key_U;
        case KEY_V:                 return Key_V;
        case KEY_W:                 return Key_W;
        case KEY_X:                 return Key_X;
        case KEY_Y:                 return Key_Y;
        case KEY_Z:                 return Key_Z;

        // Whitespace keys
        case KEY_TAB:               return Key_Tab;
        case KEY_SPACE:             return Key_Space;
        case KEY_ENTER:             return Key_Enter;

        // Navigation keys
        case KEY_HOME:              return Key_Home;
        case KEY_END:               return Key_End;
        case KEY_PAGEUP:            return Key_Page_Up;
        case KEY_PAGEDOWN:          return Key_Page_Down;
        case KEY_LEFT:              return Key_Left;
        case KEY_UP:                return Key_Up;
        case KEY_RIGHT:             return Key_Right;
        case KEY_DOWN:              return Key_Down;

        // Editing keys
        case KEY_BACKSPACE:         return Key_Backspace;
        case KEY_INSERT:            return Key_Insert;
        case KEY_DELETE:            return Key_Delete;
        case KEY_CLEAR:             return Key_Clear;

        // Function keys
        case KEY_F1:                return Key_F1;
        case KEY_F2:                return Key_F2;
        case KEY_F3:                return Key_F3;
        case KEY_F4:                return Key_F4;
        case KEY_F5:                return Key_F5;
        case KEY_F6:                return Key_F6;
        case KEY_F7:                return Key_F7;
        case KEY_F8:                return Key_F8;
        case KEY_F9:                return Key_F9;
        case KEY_F10:               return Key_F10;
        case KEY_F11:               return Key_F11;
        case KEY_F12:               return Key_F12;
        case KEY_F13:               return Key_F13;
        case KEY_F14:               return Key_F14;
        case KEY_F15:               return Key_F15;
        case KEY_F16:               return Key_F16;
        case KEY_F17:               return Key_F17;
        case KEY_F18:               return Key_F18;
        case KEY_F19:               return Key_F19;
        case KEY_F20:               return Key_F20;
        case KEY_F21:               return Key_F21;
        case KEY_F22:               return Key_F22;
        case KEY_F23:               return Key_F23;
        case KEY_F24:               return Key_F24;

        // Numpad number keys
        case KEY_KP0:               return Key_Numpad_0;
        case KEY_KP1:               return Key_Numpad_1;
        case KEY_KP2:               return Key_Numpad_2;
        case KEY_KP3:               return Key_Numpad_3;
        case KEY_KP4:               return Key_Numpad_4;
        case KEY_KP5:               return Key_Numpad_5;
        case KEY_KP6:               return Key_Numpad_6;
        case KEY_KP7:               return Key_Numpad_7;
        case KEY_KP8:               return Key_Numpad_8;
        case KEY_KP9:               return Key_Numpad_9;

        // Numpad operator keys
        case KEY_KPPLUS:            return Key_Numpad_Add;
        case KEY_KPMINUS:           return Key_Numpad_Subtract;
        case KEY_KPASTERISK:        return Key_Numpad_Multiply;
        case KEY_KPSLASH:           return Key_Numpad_Divide;
        case KEY_KPDOT:             return Key_Numpad_Decimal;
        case KEY_KPCOMMA:           return Key_Numpad_Separator;
        case KEY_KPEQUAL:           return Key_Numpad_Equal;
        case KEY_KPENTER:           return Key_Numpad_Enter;

        // Application keys
        case KEY_ESC:               return Key_Escape;
        case KEY_PAUSE:             return Key_Pause;
        case KEY_PLAY:              return Key_Play;
        case KEY_HELP:              return Key_Help;
        case KEY_MENU:              return Key_Menu;

        // Media keys
        case KEY_NEXTSONG:          return Key_Media_Next;
        case KEY_PREVIOUSSONG:      return Key_Media_Previous;
        case KEY_PLAYPAUSE:         return Key_Media_Play_Pause;
        case KEY_STOPCD:            return Key_Media_Stop;

        // Volume keys
        case KEY_VOLUMEUP:          return Key_Volume_Up;
        case KEY_VOLUMEDOWN:        return Key_Volume_Down;
        case KEY_MUTE:              return Key_Volume_Mute;

        // Lock keys
        case KEY_CAPSLOCK:          return Key_Caps_Lock;
        case KEY_NUMLOCK:           return Key_Num_Lock;
        case KEY_SCROLLLOCK:        return Key_Scroll_Lock;

        // Device keys
        case KEY_PRINT:             return Key_Print_Screen;    // Need to check
        case KEY_SLEEP:             return Key_Sleep;

        // OEM (Original Equipment Manufacturer) keys
        // The specific values of the following keys may vary on different devices.
        // All of the following uses the standard keyboard layout of the United States.
        case KEY_GRAVE:             return Key_Left_Quote;
        case KEY_MINUS:             return Key_Minus;
        case KEY_EQUAL:             return Key_Equal;
        case KEY_LEFTBRACE:         return Key_Left_Bracket;
        case KEY_RIGHTBRACE:        return Key_Right_Bracket;
        case KEY_SEMICOLON:         return Key_Semicolon;
        case KEY_APOSTROPHE:        return Key_Apostrophe;
        case KEY_COMMA:             return Key_Comma;
        case KEY_DOT:               return Key_Period;
        case KEY_SLASH:             return Key_Slash;
        case KEY_BACKSLASH:         return Key_Backslash;
        case KEY_102ND:             return Key_Angle_Bracket;   // Need to check

        // Modifier keys
        case KEY_LEFTMETA:          return Key_Meta_Left;
        case KEY_RIGHTMETA:         return Key_Meta_Right;
        case KEY_LEFTCTRL:          return Key_Ctrl_Left;
        case KEY_RIGHTCTRL:         return Key_Ctrl_Right;
        case KEY_LEFTALT:           return Key_Alt_Left;
        case KEY_RIGHTALT:          return Key_Alt_Right;
        case KEY_LEFTSHIFT:         return Key_Shift_Left;
        case KEY_RIGHTSHIFT:        return Key_Shift_Right;

        default:                    return (Key) 0;
    }
}

} // namespace kbdt
