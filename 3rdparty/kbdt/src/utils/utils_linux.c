#include <kbdt/utils.h>

#include <linux/input-event-codes.h>

KBDT_API int kbdt_keyboard_key_to_native(enum keyboard_key key)
{
    switch (key)
    {
        // Number keys
        case KBDKEY_0:                  return KEY_0;
        case KBDKEY_1:                  return KEY_1;
        case KBDKEY_2:                  return KEY_2;
        case KBDKEY_3:                  return KEY_3;
        case KBDKEY_4:                  return KEY_4;
        case KBDKEY_5:                  return KEY_5;
        case KBDKEY_6:                  return KEY_6;
        case KBDKEY_7:                  return KEY_7;
        case KBDKEY_8:                  return KEY_8;
        case KBDKEY_9:                  return KEY_9;

        // ASCII letter keys
        case KBDKEY_A:                  return KEY_A;
        case KBDKEY_B:                  return KEY_B;
        case KBDKEY_C:                  return KEY_C;
        case KBDKEY_D:                  return KEY_D;
        case KBDKEY_E:                  return KEY_E;
        case KBDKEY_F:                  return KEY_F;
        case KBDKEY_G:                  return KEY_G;
        case KBDKEY_H:                  return KEY_H;
        case KBDKEY_I:                  return KEY_I;
        case KBDKEY_J:                  return KEY_J;
        case KBDKEY_K:                  return KEY_K;
        case KBDKEY_L:                  return KEY_L;
        case KBDKEY_M:                  return KEY_M;
        case KBDKEY_N:                  return KEY_N;
        case KBDKEY_O:                  return KEY_O;
        case KBDKEY_P:                  return KEY_P;
        case KBDKEY_Q:                  return KEY_Q;
        case KBDKEY_R:                  return KEY_R;
        case KBDKEY_S:                  return KEY_S;
        case KBDKEY_T:                  return KEY_T;
        case KBDKEY_U:                  return KEY_U;
        case KBDKEY_V:                  return KEY_V;
        case KBDKEY_W:                  return KEY_W;
        case KBDKEY_X:                  return KEY_X;
        case KBDKEY_Y:                  return KEY_Y;
        case KBDKEY_Z:                  return KEY_Z;

        // Whitespace keys
        case KBDKEY_TAB:                return KEY_TAB;
        case KBDKEY_SPACE:              return KEY_SPACE;
        case KBDKEY_ENTER:              return KEY_ENTER;

        // Navigation keys
        case KBDKEY_HOME:               return KEY_HOME;
        case KBDKEY_END:                return KEY_END;
        case KBDKEY_PAGE_UP:            return KEY_PAGEUP;
        case KBDKEY_PAGE_DOWN:          return KEY_PAGEDOWN;
        case KBDKEY_LEFT:               return KEY_LEFT;
        case KBDKEY_UP:                 return KEY_UP;
        case KBDKEY_RIGHT:              return KEY_RIGHT;
        case KBDKEY_DOWN:               return KEY_DOWN;

        // Editing keys
        case KBDKEY_BACKSPACE:          return KEY_BACKSPACE;
        case KBDKEY_INSERT:             return KEY_INSERT;
        case KBDKEY_DELETE:             return KEY_DELETE;
        case KBDKEY_CLEAR:              return KEY_CLEAR;

        // Function keys
        case KBDKEY_F1:                 return KEY_F1;
        case KBDKEY_F2:                 return KEY_F2;
        case KBDKEY_F3:                 return KEY_F3;
        case KBDKEY_F4:                 return KEY_F4;
        case KBDKEY_F5:                 return KEY_F5;
        case KBDKEY_F6:                 return KEY_F6;
        case KBDKEY_F7:                 return KEY_F7;
        case KBDKEY_F8:                 return KEY_F8;
        case KBDKEY_F9:                 return KEY_F9;
        case KBDKEY_F10:                return KEY_F10;
        case KBDKEY_F11:                return KEY_F11;
        case KBDKEY_F12:                return KEY_F12;
        case KBDKEY_F13:                return KEY_F13;
        case KBDKEY_F14:                return KEY_F14;
        case KBDKEY_F15:                return KEY_F15;
        case KBDKEY_F16:                return KEY_F16;
        case KBDKEY_F17:                return KEY_F17;
        case KBDKEY_F18:                return KEY_F18;
        case KBDKEY_F19:                return KEY_F19;
        case KBDKEY_F20:                return KEY_F20;
        case KBDKEY_F21:                return KEY_F21;
        case KBDKEY_F22:                return KEY_F22;
        case KBDKEY_F23:                return KEY_F23;
        case KBDKEY_F24:                return KEY_F24;

        // Numpad number keys
        case KBDKEY_NUMPAD_0:           return KEY_KP0;
        case KBDKEY_NUMPAD_1:           return KEY_KP1;
        case KBDKEY_NUMPAD_2:           return KEY_KP2;
        case KBDKEY_NUMPAD_3:           return KEY_KP3;
        case KBDKEY_NUMPAD_4:           return KEY_KP4;
        case KBDKEY_NUMPAD_5:           return KEY_KP5;
        case KBDKEY_NUMPAD_6:           return KEY_KP6;
        case KBDKEY_NUMPAD_7:           return KEY_KP7;
        case KBDKEY_NUMPAD_8:           return KEY_KP8;
        case KBDKEY_NUMPAD_9:           return KEY_KP9;

        // Numpad operator keys
        case KBDKEY_NUMPAD_ADD:         return KEY_KPPLUS;
        case KBDKEY_NUMPAD_SUBTRACT:    return KEY_KPMINUS;
        case KBDKEY_NUMPAD_MULTIPLY:    return KEY_KPASTERISK;
        case KBDKEY_NUMPAD_DIVIDE:      return KEY_KPSLASH;
        case KBDKEY_NUMPAD_DECIMAL:     return KEY_KPDOT;
        case KBDKEY_NUMPAD_SEPARATOR:   return KEY_KPCOMMA;
        case KBDKEY_NUMPAD_EQUAL:       return KEY_KPEQUAL;
        case KBDKEY_NUMPAD_ENTER:       return KEY_KPENTER;

        // Application keys
        case KBDKEY_ESCAPE:             return KEY_ESC;
        case KBDKEY_PAUSE:              return KEY_PAUSE;
        case KBDKEY_PLAY:               return KEY_PLAY;
        case KBDKEY_HELP:               return KEY_HELP;
        case KBDKEY_MENU:               return KEY_MENU;

        // Media keys
        case KBDKEY_MEDIA_NEXT:         return KEY_NEXTSONG;
        case KBDKEY_MEDIA_PREVIOUS:     return KEY_PREVIOUSSONG;
        case KBDKEY_MEDIA_PLAY_PAUSE:   return KEY_PLAYPAUSE;
        case KBDKEY_MEDIA_STOP:         return KEY_STOPCD;

        // Volume keys
        case KBDKEY_VOLUME_UP:          return KEY_VOLUMEUP;
        case KBDKEY_VOLUME_DOWN:        return KEY_VOLUMEDOWN;
        case KBDKEY_VOLUME_MUTE:        return KEY_MUTE;

        // Lock keys
        case KBDKEY_CAPS_LOCK:          return KEY_CAPSLOCK;
        case KBDKEY_NUM_LOCK:           return KEY_NUMLOCK;
        case KBDKEY_SCROLL_LOCK:        return KEY_SCROLLLOCK;

        // Device keys
        case KBDKEY_PRINT_SCREEN:       return KEY_PRINT;    // Need to check
        case KBDKEY_SLEEP:              return KEY_SLEEP;

        // OEM (Original Equipment Manufacturer) keys
        // The specific values of the following keys may vary on different devices.
        // All of the following uses the standard keyboard layout of the United States.
        case KBDKEY_LEFT_QUOTE:         return KEY_GRAVE;
        case KBDKEY_MINUS:              return KEY_MINUS;
        case KBDKEY_EQUAL:              return KEY_EQUAL;
        case KBDKEY_LEFT_BRACKET:       return KEY_LEFTBRACE;
        case KBDKEY_RIGHT_BRACKET:      return KEY_RIGHTBRACE;
        case KBDKEY_SEMICOLON:          return KEY_SEMICOLON;
        case KBDKEY_APOSTROPHE:         return KEY_APOSTROPHE;
        case KBDKEY_COMMA:              return KEY_COMMA;
        case KBDKEY_PERIOD:             return KEY_DOT;
        case KBDKEY_SLASH:              return KEY_SLASH;
        case KBDKEY_BACKSLASH:          return KEY_BACKSLASH;
        case KBDKEY_ANGLE_BRACKET:      return KEY_102ND;     // Need to check

        // Modifier keys
        case KBDKEY_MOD_META:           return 0;   // Not supported
        case KBDKEY_MOD_META_LEFT:      return KEY_LEFTMETA;
        case KBDKEY_MOD_META_RIGHT:     return KEY_RIGHTMETA;
        case KBDKEY_MOD_CTRL:           return 0;   // Not supported
        case KBDKEY_MOD_CTRL_LEFT:      return KEY_LEFTCTRL;
        case KBDKEY_MOD_CTRL_RIGHT:     return KEY_RIGHTCTRL;
        case KBDKEY_MOD_ALT:            return 0;   // Not supported
        case KBDKEY_MOD_ALT_LEFT:       return KEY_LEFTALT;
        case KBDKEY_MOD_ALT_RIGHT:      return KEY_RIGHTALT;
        case KBDKEY_MOD_SHIFT:          return 0;   // Not supported
        case KBDKEY_MOD_SHIFT_LEFT:     return KEY_LEFTSHIFT;
        case KBDKEY_MOD_SHIFT_RIGHT:    return KEY_RIGHTSHIFT;

        default:                        return 0;
    }
}

KBDT_API enum keyboard_key kbdt_keyboard_key_from_native(int native_key)
{
    switch (native_key)
    {
        // Number keys
        case KEY_0:                 return KBDKEY_0;
        case KEY_1:                 return KBDKEY_1;
        case KEY_2:                 return KBDKEY_2;
        case KEY_3:                 return KBDKEY_3;
        case KEY_4:                 return KBDKEY_4;
        case KEY_5:                 return KBDKEY_5;
        case KEY_6:                 return KBDKEY_6;
        case KEY_7:                 return KBDKEY_7;
        case KEY_8:                 return KBDKEY_8;
        case KEY_9:                 return KBDKEY_9;

        // ASCII letter keys
        case KEY_A:                 return KBDKEY_A;
        case KEY_B:                 return KBDKEY_B;
        case KEY_C:                 return KBDKEY_C;
        case KEY_D:                 return KBDKEY_D;
        case KEY_E:                 return KBDKEY_E;
        case KEY_F:                 return KBDKEY_F;
        case KEY_G:                 return KBDKEY_G;
        case KEY_H:                 return KBDKEY_H;
        case KEY_I:                 return KBDKEY_I;
        case KEY_J:                 return KBDKEY_J;
        case KEY_K:                 return KBDKEY_K;
        case KEY_L:                 return KBDKEY_L;
        case KEY_M:                 return KBDKEY_M;
        case KEY_N:                 return KBDKEY_N;
        case KEY_O:                 return KBDKEY_O;
        case KEY_P:                 return KBDKEY_P;
        case KEY_Q:                 return KBDKEY_Q;
        case KEY_R:                 return KBDKEY_R;
        case KEY_S:                 return KBDKEY_S;
        case KEY_T:                 return KBDKEY_T;
        case KEY_U:                 return KBDKEY_U;
        case KEY_V:                 return KBDKEY_V;
        case KEY_W:                 return KBDKEY_W;
        case KEY_X:                 return KBDKEY_X;
        case KEY_Y:                 return KBDKEY_Y;
        case KEY_Z:                 return KBDKEY_Z;

        // Whitespace keys
        case KEY_TAB:               return KBDKEY_TAB;
        case KEY_SPACE:             return KBDKEY_SPACE;
        case KEY_ENTER:             return KBDKEY_ENTER;

        // Navigation keys
        case KEY_HOME:              return KBDKEY_HOME;
        case KEY_END:               return KBDKEY_END;
        case KEY_PAGEUP:            return KBDKEY_PAGE_UP;
        case KEY_PAGEDOWN:          return KBDKEY_PAGE_DOWN;
        case KEY_LEFT:              return KBDKEY_LEFT;
        case KEY_UP:                return KBDKEY_UP;
        case KEY_RIGHT:             return KBDKEY_RIGHT;
        case KEY_DOWN:              return KBDKEY_DOWN;

        // Editing keys
        case KEY_BACKSPACE:         return KBDKEY_BACKSPACE;
        case KEY_INSERT:            return KBDKEY_INSERT;
        case KEY_DELETE:            return KBDKEY_DELETE;
        case KEY_CLEAR:             return KBDKEY_CLEAR;

        // Function keys
        case KEY_F1:                return KBDKEY_F1;
        case KEY_F2:                return KBDKEY_F2;
        case KEY_F3:                return KBDKEY_F3;
        case KEY_F4:                return KBDKEY_F4;
        case KEY_F5:                return KBDKEY_F5;
        case KEY_F6:                return KBDKEY_F6;
        case KEY_F7:                return KBDKEY_F7;
        case KEY_F8:                return KBDKEY_F8;
        case KEY_F9:                return KBDKEY_F9;
        case KEY_F10:               return KBDKEY_F10;
        case KEY_F11:               return KBDKEY_F11;
        case KEY_F12:               return KBDKEY_F12;
        case KEY_F13:               return KBDKEY_F13;
        case KEY_F14:               return KBDKEY_F14;
        case KEY_F15:               return KBDKEY_F15;
        case KEY_F16:               return KBDKEY_F16;
        case KEY_F17:               return KBDKEY_F17;
        case KEY_F18:               return KBDKEY_F18;
        case KEY_F19:               return KBDKEY_F19;
        case KEY_F20:               return KBDKEY_F20;
        case KEY_F21:               return KBDKEY_F21;
        case KEY_F22:               return KBDKEY_F22;
        case KEY_F23:               return KBDKEY_F23;
        case KEY_F24:               return KBDKEY_F24;

        // Numpad number keys
        case KEY_KP0:               return KBDKEY_NUMPAD_0;
        case KEY_KP1:               return KBDKEY_NUMPAD_1;
        case KEY_KP2:               return KBDKEY_NUMPAD_2;
        case KEY_KP3:               return KBDKEY_NUMPAD_3;
        case KEY_KP4:               return KBDKEY_NUMPAD_4;
        case KEY_KP5:               return KBDKEY_NUMPAD_5;
        case KEY_KP6:               return KBDKEY_NUMPAD_6;
        case KEY_KP7:               return KBDKEY_NUMPAD_7;
        case KEY_KP8:               return KBDKEY_NUMPAD_8;
        case KEY_KP9:               return KBDKEY_NUMPAD_9;

        // Numpad operator keys
        case KEY_KPPLUS:            return KBDKEY_NUMPAD_ADD;
        case KEY_KPMINUS:           return KBDKEY_NUMPAD_SUBTRACT;
        case KEY_KPASTERISK:        return KBDKEY_NUMPAD_MULTIPLY;
        case KEY_KPSLASH:           return KBDKEY_NUMPAD_DIVIDE;
        case KEY_KPDOT:             return KBDKEY_NUMPAD_DECIMAL;
        case KEY_KPCOMMA:           return KBDKEY_NUMPAD_SEPARATOR;
        case KEY_KPEQUAL:           return KBDKEY_NUMPAD_EQUAL;
        case KEY_KPENTER:           return KBDKEY_NUMPAD_ENTER;

        // Application keys
        case KEY_ESC:               return KBDKEY_ESCAPE;
        case KEY_PAUSE:             return KBDKEY_PAUSE;
        case KEY_PLAY:              return KBDKEY_PLAY;
        case KEY_HELP:              return KBDKEY_HELP;
        case KEY_MENU:              return KBDKEY_MENU;

        // Media keys
        case KEY_NEXTSONG:          return KBDKEY_MEDIA_NEXT;
        case KEY_PREVIOUSSONG:      return KBDKEY_MEDIA_PREVIOUS;
        case KEY_PLAYPAUSE:         return KBDKEY_MEDIA_PLAY_PAUSE;
        case KEY_STOPCD:            return KBDKEY_MEDIA_STOP;

        // Volume keys
        case KEY_VOLUMEUP:          return KBDKEY_VOLUME_UP;
        case KEY_VOLUMEDOWN:        return KBDKEY_VOLUME_DOWN;
        case KEY_MUTE:              return KBDKEY_VOLUME_MUTE;

        // Lock keys
        case KEY_CAPSLOCK:          return KBDKEY_CAPS_LOCK;
        case KEY_NUMLOCK:           return KBDKEY_NUM_LOCK;
        case KEY_SCROLLLOCK:        return KBDKEY_SCROLL_LOCK;

        // Device keys
        case KEY_PRINT:             return KBDKEY_PRINT_SCREEN;    // Need to check
        case KEY_SLEEP:             return KBDKEY_SLEEP;

        // OEM (Original Equipment Manufacturer) keys
        // The specific values of the following keys may vary on different devices.
        // All of the following uses the standard keyboard layout of the United States.
        case KEY_GRAVE:             return KBDKEY_LEFT_QUOTE;
        case KEY_MINUS:             return KBDKEY_MINUS;
        case KEY_EQUAL:             return KBDKEY_EQUAL;
        case KEY_LEFTBRACE:         return KBDKEY_LEFT_BRACKET;
        case KEY_RIGHTBRACE:        return KBDKEY_RIGHT_BRACKET;
        case KEY_SEMICOLON:         return KBDKEY_SEMICOLON;
        case KEY_APOSTROPHE:        return KBDKEY_APOSTROPHE;
        case KEY_COMMA:             return KBDKEY_COMMA;
        case KEY_DOT:               return KBDKEY_PERIOD;
        case KEY_SLASH:             return KBDKEY_SLASH;
        case KEY_BACKSLASH:         return KBDKEY_BACKSLASH;
        case KEY_102ND:             return KBDKEY_ANGLE_BRACKET;   // Need to check

        // Modifier keys
        case KEY_LEFTMETA:          return KBDKEY_MOD_META_LEFT;
        case KEY_RIGHTMETA:         return KBDKEY_MOD_META_RIGHT;
        case KEY_LEFTCTRL:          return KBDKEY_MOD_CTRL_LEFT;
        case KEY_RIGHTCTRL:         return KBDKEY_MOD_CTRL_RIGHT;
        case KEY_LEFTALT:           return KBDKEY_MOD_ALT_LEFT;
        case KEY_RIGHTALT:          return KBDKEY_MOD_ALT_RIGHT;
        case KEY_LEFTSHIFT:         return KBDKEY_MOD_SHIFT_LEFT;
        case KEY_RIGHTSHIFT:        return KBDKEY_MOD_SHIFT_RIGHT;

        default:                    return (enum keyboard_key) 0;
    }
}
