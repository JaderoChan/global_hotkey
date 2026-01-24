#include <kbdt/utils.h>

#include <windows.h>

KBDT_API int kbdt_keyboard_key_to_native(enum keyboard_key key)
{
    if ((key >= KBDKEY_0 && key <= KBDKEY_9) || (key >= KBDKEY_A && key <= KBDKEY_Z))
         return key;

    switch (key)
    {
        // Whitespace keys
        case KBDKEY_TAB:                return VK_TAB;
        case KBDKEY_SPACE:              return VK_SPACE;
        case KBDKEY_ENTER:              return VK_RETURN;

        // Navigation keys
        case KBDKEY_HOME:               return VK_HOME;
        case KBDKEY_END:                return VK_END;
        case KBDKEY_PAGE_UP:            return VK_PRIOR;
        case KBDKEY_PAGE_DOWN:          return VK_NEXT;
        case KBDKEY_LEFT:               return VK_LEFT;
        case KBDKEY_UP:                 return VK_UP;
        case KBDKEY_RIGHT:              return VK_RIGHT;
        case KBDKEY_DOWN:               return VK_DOWN;

        // Editing keys
        case KBDKEY_BACKSPACE:          return VK_BACK;
        case KBDKEY_INSERT:             return VK_INSERT;
        case KBDKEY_DELETE:             return VK_DELETE;
        case KBDKEY_CLEAR:              return VK_CLEAR;

        // Function keys
        case KBDKEY_F1:                 return VK_F1;
        case KBDKEY_F2:                 return VK_F2;
        case KBDKEY_F3:                 return VK_F3;
        case KBDKEY_F4:                 return VK_F4;
        case KBDKEY_F5:                 return VK_F5;
        case KBDKEY_F6:                 return VK_F6;
        case KBDKEY_F7:                 return VK_F7;
        case KBDKEY_F8:                 return VK_F8;
        case KBDKEY_F9:                 return VK_F9;
        case KBDKEY_F10:                return VK_F10;
        case KBDKEY_F11:                return VK_F11;
        case KBDKEY_F12:                return VK_F12;
        case KBDKEY_F13:                return VK_F13;
        case KBDKEY_F14:                return VK_F14;
        case KBDKEY_F15:                return VK_F15;
        case KBDKEY_F16:                return VK_F16;
        case KBDKEY_F17:                return VK_F17;
        case KBDKEY_F18:                return VK_F18;
        case KBDKEY_F19:                return VK_F19;
        case KBDKEY_F20:                return VK_F20;
        case KBDKEY_F21:                return VK_F21;
        case KBDKEY_F22:                return VK_F22;
        case KBDKEY_F23:                return VK_F23;
        case KBDKEY_F24:                return VK_F24;

        // Numpad number keys
        case KBDKEY_NUMPAD_0:           return VK_NUMPAD0;
        case KBDKEY_NUMPAD_1:           return VK_NUMPAD1;
        case KBDKEY_NUMPAD_2:           return VK_NUMPAD2;
        case KBDKEY_NUMPAD_3:           return VK_NUMPAD3;
        case KBDKEY_NUMPAD_4:           return VK_NUMPAD4;
        case KBDKEY_NUMPAD_5:           return VK_NUMPAD5;
        case KBDKEY_NUMPAD_6:           return VK_NUMPAD6;
        case KBDKEY_NUMPAD_7:           return VK_NUMPAD7;
        case KBDKEY_NUMPAD_8:           return VK_NUMPAD8;
        case KBDKEY_NUMPAD_9:           return VK_NUMPAD9;

        // Numpad operator keys
        case KBDKEY_NUMPAD_ADD:         return VK_ADD;
        case KBDKEY_NUMPAD_SUBTRACT:    return VK_SUBTRACT;
        case KBDKEY_NUMPAD_MULTIPLY:    return VK_MULTIPLY;
        case KBDKEY_NUMPAD_DIVIDE:      return VK_DIVIDE;
        case KBDKEY_NUMPAD_DECIMAL:     return VK_DECIMAL;
        case KBDKEY_NUMPAD_SEPARATOR:   return VK_SEPARATOR;
        case KBDKEY_NUMPAD_EQUAL:       return VK_OEM_NEC_EQUAL;    // Need check
        case KBDKEY_NUMPAD_ENTER:       return VK_RETURN;

        // Applications keys
        case KBDKEY_ESCAPE:             return VK_ESCAPE;
        case KBDKEY_PAUSE:              return VK_PAUSE;
        case KBDKEY_PLAY:               return VK_PLAY;
        case KBDKEY_HELP:               return VK_HELP;
        case KBDKEY_MENU:               return VK_APPS;

        // Media keys
        case KBDKEY_MEDIA_NEXT:         return VK_MEDIA_NEXT_TRACK;
        case KBDKEY_MEDIA_PREVIOUS:     return VK_MEDIA_PREV_TRACK;
        case KBDKEY_MEDIA_PLAY_PAUSE:   return VK_MEDIA_PLAY_PAUSE;
        case KBDKEY_MEDIA_STOP:         return VK_MEDIA_STOP;

        // Volume keys
        case KBDKEY_VOLUME_UP:          return VK_VOLUME_UP;
        case KBDKEY_VOLUME_DOWN:        return VK_VOLUME_DOWN;
        case KBDKEY_VOLUME_MUTE:        return VK_VOLUME_MUTE;

        // Lock keys
        case KBDKEY_CAPS_LOCK:          return VK_CAPITAL;
        case KBDKEY_NUM_LOCK:           return VK_NUMLOCK;
        case KBDKEY_SCROLL_LOCK:        return VK_SCROLL;

        // Device keys
        case KBDKEY_PRINT_SCREEN:       return VK_SNAPSHOT;
        case KBDKEY_SLEEP:              return VK_SLEEP;

        // OEM (Original Equipment Manufacturer) keys
        // The specific values of the following keys may vary on different devices.
        // All of the following uses the standard keyboard layout of the United States.
        case KBDKEY_LEFT_QUOTE:         return VK_OEM_3;
        case KBDKEY_MINUS:              return VK_OEM_MINUS;
        case KBDKEY_EQUAL:              return VK_OEM_PLUS;
        case KBDKEY_LEFT_BRACKET:       return VK_OEM_4;
        case KBDKEY_RIGHT_BRACKET:      return VK_OEM_6;
        case KBDKEY_SEMICOLON:          return VK_OEM_1;
        case KBDKEY_APOSTROPHE:         return VK_OEM_7;
        case KBDKEY_COMMA:              return VK_OEM_COMMA;
        case KBDKEY_PERIOD:             return VK_OEM_PERIOD;
        case KBDKEY_SLASH:              return VK_OEM_2;
        case KBDKEY_BACKSLASH:          return VK_OEM_5;
        case KBDKEY_ANGLE_BRACKET:      return VK_OEM_102;

        // Modifier keys
        case KBDKEY_MOD_META:           return 0;   // Not supported
        case KBDKEY_MOD_META_LEFT:      return VK_LWIN;
        case KBDKEY_MOD_META_RIGHT:     return VK_RWIN;
        case KBDKEY_MOD_CTRL:           return VK_CONTROL;
        case KBDKEY_MOD_CTRL_LEFT:      return VK_LCONTROL;
        case KBDKEY_MOD_CTRL_RIGHT:     return VK_RCONTROL;
        case KBDKEY_MOD_ALT:            return VK_MENU;
        case KBDKEY_MOD_ALT_LEFT:       return VK_LMENU;
        case KBDKEY_MOD_ALT_RIGHT:      return VK_RMENU;
        case KBDKEY_MOD_SHIFT:          return VK_SHIFT;
        case KBDKEY_MOD_SHIFT_LEFT:     return VK_LSHIFT;
        case KBDKEY_MOD_SHIFT_RIGHT:    return VK_RSHIFT;

        default:                        return 0;
    }
}

KBDT_API enum keyboard_key kbdt_keyboard_key_from_native(int native_key)
{
    if ((native_key >= '0' && native_key <= '9') || (native_key >= 'A' && native_key <= 'Z'))
         return native_key;

    switch (native_key)
    {
        // Whitespace keys
        case VK_TAB:                return KBDKEY_TAB;
        case VK_SPACE:              return KBDKEY_SPACE;
        case VK_RETURN:             return KBDKEY_ENTER;

        // Navigation keys
        case VK_HOME:               return KBDKEY_HOME;
        case VK_END:                return KBDKEY_END;
        case VK_PRIOR:              return KBDKEY_PAGE_UP;
        case VK_NEXT:               return KBDKEY_PAGE_DOWN;
        case VK_LEFT:               return KBDKEY_LEFT;
        case VK_UP:                 return KBDKEY_UP;
        case VK_RIGHT:              return KBDKEY_RIGHT;
        case VK_DOWN:               return KBDKEY_DOWN;

        // Editing keys
        case VK_BACK:               return KBDKEY_BACKSPACE;
        case VK_INSERT:             return KBDKEY_INSERT;
        case VK_DELETE:             return KBDKEY_DELETE;
        case VK_CLEAR:              return KBDKEY_CLEAR;

        // Function keys
        case VK_F1:                 return KBDKEY_F1;
        case VK_F2:                 return KBDKEY_F2;
        case VK_F3:                 return KBDKEY_F3;
        case VK_F4:                 return KBDKEY_F4;
        case VK_F5:                 return KBDKEY_F5;
        case VK_F6:                 return KBDKEY_F6;
        case VK_F7:                 return KBDKEY_F7;
        case VK_F8:                 return KBDKEY_F8;
        case VK_F9:                 return KBDKEY_F9;
        case VK_F10:                return KBDKEY_F10;
        case VK_F11:                return KBDKEY_F11;
        case VK_F12:                return KBDKEY_F12;
        case VK_F13:                return KBDKEY_F13;
        case VK_F14:                return KBDKEY_F14;
        case VK_F15:                return KBDKEY_F15;
        case VK_F16:                return KBDKEY_F16;
        case VK_F17:                return KBDKEY_F17;
        case VK_F18:                return KBDKEY_F18;
        case VK_F19:                return KBDKEY_F19;
        case VK_F20:                return KBDKEY_F20;
        case VK_F21:                return KBDKEY_F21;
        case VK_F22:                return KBDKEY_F22;
        case VK_F23:                return KBDKEY_F23;
        case VK_F24:                return KBDKEY_F24;

        // Numpad number keys
        case VK_NUMPAD0:            return KBDKEY_NUMPAD_0;
        case VK_NUMPAD1:            return KBDKEY_NUMPAD_1;
        case VK_NUMPAD2:            return KBDKEY_NUMPAD_2;
        case VK_NUMPAD3:            return KBDKEY_NUMPAD_3;
        case VK_NUMPAD4:            return KBDKEY_NUMPAD_4;
        case VK_NUMPAD5:            return KBDKEY_NUMPAD_5;
        case VK_NUMPAD6:            return KBDKEY_NUMPAD_6;
        case VK_NUMPAD7:            return KBDKEY_NUMPAD_7;
        case VK_NUMPAD8:            return KBDKEY_NUMPAD_8;
        case VK_NUMPAD9:            return KBDKEY_NUMPAD_9;

        // Numpad operator keys
        case VK_ADD:                return KBDKEY_NUMPAD_ADD;
        case VK_SUBTRACT:           return KBDKEY_NUMPAD_SUBTRACT;
        case VK_MULTIPLY:           return KBDKEY_NUMPAD_MULTIPLY;
        case VK_DIVIDE:             return KBDKEY_NUMPAD_DIVIDE;
        case VK_DECIMAL:            return KBDKEY_NUMPAD_DECIMAL;
        case VK_SEPARATOR:          return KBDKEY_NUMPAD_SEPARATOR;
        case VK_OEM_NEC_EQUAL:      return KBDKEY_NUMPAD_EQUAL;

        // Applications keys
        case VK_ESCAPE:             return KBDKEY_ESCAPE;
        case VK_PAUSE:              return KBDKEY_PAUSE;
        case VK_PLAY:               return KBDKEY_PLAY;
        case VK_HELP:               return KBDKEY_HELP;
        case VK_APPS:               return KBDKEY_MENU;

        // Media keys
        case VK_MEDIA_NEXT_TRACK:   return KBDKEY_MEDIA_NEXT;
        case VK_MEDIA_PREV_TRACK:   return KBDKEY_MEDIA_PREVIOUS;
        case VK_MEDIA_PLAY_PAUSE:   return KBDKEY_MEDIA_PLAY_PAUSE;
        case VK_MEDIA_STOP:         return KBDKEY_MEDIA_STOP;

        // Volume keys
        case VK_VOLUME_UP:          return KBDKEY_VOLUME_UP;
        case VK_VOLUME_DOWN:        return KBDKEY_VOLUME_DOWN;
        case VK_VOLUME_MUTE:        return KBDKEY_VOLUME_MUTE;

        // Lock keys
        case VK_CAPITAL:            return KBDKEY_CAPS_LOCK;
        case VK_NUMLOCK:            return KBDKEY_NUM_LOCK;
        case VK_SCROLL:             return KBDKEY_SCROLL_LOCK;

        // Device keys
        case VK_SNAPSHOT:           return KBDKEY_PRINT_SCREEN;
        case VK_SLEEP:              return KBDKEY_SLEEP;

        // OEM (Original Equipment Manufacturer) keys
        // The specific values of the following keys may vary on different devices.
        // All of the following uses the standard keyboard layout of the United States.
        case VK_OEM_3:              return KBDKEY_LEFT_QUOTE;
        case VK_OEM_MINUS:          return KBDKEY_MINUS;
        case VK_OEM_PLUS:           return KBDKEY_EQUAL;
        case VK_OEM_4:              return KBDKEY_LEFT_BRACKET;
        case VK_OEM_6:              return KBDKEY_RIGHT_BRACKET;
        case VK_OEM_1:              return KBDKEY_SEMICOLON;
        case VK_OEM_7:              return KBDKEY_APOSTROPHE;
        case VK_OEM_COMMA:          return KBDKEY_COMMA;
        case VK_OEM_PERIOD:         return KBDKEY_PERIOD;
        case VK_OEM_2:              return KBDKEY_SLASH;
        case VK_OEM_5:              return KBDKEY_BACKSLASH;
        case VK_OEM_102:            return KBDKEY_ANGLE_BRACKET;

        // Modifier keys
        case VK_LWIN:               return KBDKEY_MOD_META_LEFT;
        case VK_RWIN:               return KBDKEY_MOD_META_RIGHT;
        case VK_CONTROL:            return KBDKEY_MOD_CTRL;
        case VK_LCONTROL:           return KBDKEY_MOD_CTRL_LEFT;
        case VK_RCONTROL:           return KBDKEY_MOD_CTRL_RIGHT;
        case VK_MENU:               return KBDKEY_MOD_ALT;
        case VK_LMENU:              return KBDKEY_MOD_ALT_LEFT;
        case VK_RMENU:              return KBDKEY_MOD_ALT_RIGHT;
        case VK_SHIFT:              return KBDKEY_MOD_SHIFT;
        case VK_LSHIFT:             return KBDKEY_MOD_SHIFT_LEFT;
        case VK_RSHIFT:             return KBDKEY_MOD_SHIFT_RIGHT;

        default:                    return (enum keyboard_key) 0;
    }
}
