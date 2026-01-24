#include <kbdt/utils.h>

#include <Carbon/Carbon.h>

KBDT_API int kbdt_keyboard_key_to_native(enum keyboard_key key)
{
    switch (key)
    {
        // Number keys
        case KBDKEY_0:                  return kVK_ANSI_0;
        case KBDKEY_1:                  return kVK_ANSI_1;
        case KBDKEY_2:                  return kVK_ANSI_2;
        case KBDKEY_3:                  return kVK_ANSI_3;
        case KBDKEY_4:                  return kVK_ANSI_4;
        case KBDKEY_5:                  return kVK_ANSI_5;
        case KBDKEY_6:                  return kVK_ANSI_6;
        case KBDKEY_7:                  return kVK_ANSI_7;
        case KBDKEY_8:                  return kVK_ANSI_8;
        case KBDKEY_9:                  return kVK_ANSI_9;

        // ASCII letter keys
        case KBDKEY_A:                  return kVK_ANSI_A;
        case KBDKEY_B:                  return kVK_ANSI_B;
        case KBDKEY_C:                  return kVK_ANSI_C;
        case KBDKEY_D:                  return kVK_ANSI_D;
        case KBDKEY_E:                  return kVK_ANSI_E;
        case KBDKEY_F:                  return kVK_ANSI_F;
        case KBDKEY_G:                  return kVK_ANSI_G;
        case KBDKEY_H:                  return kVK_ANSI_H;
        case KBDKEY_I:                  return kVK_ANSI_I;
        case KBDKEY_J:                  return kVK_ANSI_J;
        case KBDKEY_K:                  return kVK_ANSI_K;
        case KBDKEY_L:                  return kVK_ANSI_L;
        case KBDKEY_M:                  return kVK_ANSI_M;
        case KBDKEY_N:                  return kVK_ANSI_N;
        case KBDKEY_O:                  return kVK_ANSI_O;
        case KBDKEY_P:                  return kVK_ANSI_P;
        case KBDKEY_Q:                  return kVK_ANSI_Q;
        case KBDKEY_R:                  return kVK_ANSI_R;
        case KBDKEY_S:                  return kVK_ANSI_S;
        case KBDKEY_T:                  return kVK_ANSI_T;
        case KBDKEY_U:                  return kVK_ANSI_U;
        case KBDKEY_V:                  return kVK_ANSI_V;
        case KBDKEY_W:                  return kVK_ANSI_W;
        case KBDKEY_X:                  return kVK_ANSI_X;
        case KBDKEY_Y:                  return kVK_ANSI_Y;
        case KBDKEY_Z:                  return kVK_ANSI_Z;

        // Whitespace keys
        case KBDKEY_TAB:                return kVK_Tab;
        case KBDKEY_SPACE:              return kVK_Space;
        case KBDKEY_ENTER:              return kVK_Return;

        // Navigation keys
        case KBDKEY_HOME:               return kVK_Home;
        case KBDKEY_END:                return kVK_End;
        case KBDKEY_PAGE_UP:            return kVK_PageUp;
        case KBDKEY_PAGE_DOWN:          return kVK_PageDown;
        case KBDKEY_LEFT:               return kVK_LeftArrow;
        case KBDKEY_UP:                 return kVK_UpArrow;
        case KBDKEY_RIGHT:              return kVK_RightArrow;
        case KBDKEY_DOWN:               return kVK_DownArrow;

        // Editing keys
        case KBDKEY_BACKSPACE:          return kVK_Delete;
        case KBDKEY_INSERT:             return 0;   // Not supported
        case KBDKEY_DELETE:             return kVK_ForwardDelete;
        case KBDKEY_CLEAR:              return kVK_ANSI_KeypadClear;

        // Function keys
        case KBDKEY_F1:                 return kVK_F1;
        case KBDKEY_F2:                 return kVK_F2;
        case KBDKEY_F3:                 return kVK_F3;
        case KBDKEY_F4:                 return kVK_F4;
        case KBDKEY_F5:                 return kVK_F5;
        case KBDKEY_F6:                 return kVK_F6;
        case KBDKEY_F7:                 return kVK_F7;
        case KBDKEY_F8:                 return kVK_F8;
        case KBDKEY_F9:                 return kVK_F9;
        case KBDKEY_F10:                return kVK_F10;
        case KBDKEY_F11:                return kVK_F11;
        case KBDKEY_F12:                return kVK_F12;
        case KBDKEY_F13:                return kVK_F13;
        case KBDKEY_F14:                return kVK_F14;
        case KBDKEY_F15:                return kVK_F15;
        case KBDKEY_F16:                return kVK_F16;
        case KBDKEY_F17:                return kVK_F17;
        case KBDKEY_F18:                return kVK_F18;
        case KBDKEY_F19:                return kVK_F19;
        case KBDKEY_F20:                return kVK_F20;

        // Numpad number keys
        case KBDKEY_NUMPAD_0:           return kVK_ANSI_Keypad0;
        case KBDKEY_NUMPAD_1:           return kVK_ANSI_Keypad1;
        case KBDKEY_NUMPAD_2:           return kVK_ANSI_Keypad2;
        case KBDKEY_NUMPAD_3:           return kVK_ANSI_Keypad3;
        case KBDKEY_NUMPAD_4:           return kVK_ANSI_Keypad4;
        case KBDKEY_NUMPAD_5:           return kVK_ANSI_Keypad5;
        case KBDKEY_NUMPAD_6:           return kVK_ANSI_Keypad6;
        case KBDKEY_NUMPAD_7:           return kVK_ANSI_Keypad7;
        case KBDKEY_NUMPAD_8:           return kVK_ANSI_Keypad8;
        case KBDKEY_NUMPAD_9:           return kVK_ANSI_Keypad9;

        // Numpad operator keys
        case KBDKEY_NUMPAD_ADD:         return kVK_ANSI_KeypadPlus;
        case KBDKEY_NUMPAD_SUBTRACT:    return kVK_ANSI_KeypadMinus;
        case KBDKEY_NUMPAD_MULTIPLY:    return kVK_ANSI_KeypadMultiply;
        case KBDKEY_NUMPAD_DIVIDE:      return kVK_ANSI_KeypadDivide;
        case KBDKEY_NUMPAD_DECIMAL:     return kVK_ANSI_KeypadDecimal;
        case KBDKEY_NUMPAD_SEPARATOR:   return 0;   // Not supported
        case KBDKEY_NUMPAD_EQUAL:       return kVK_ANSI_KeypadEquals;
        case KBDKEY_NUMPAD_ENTER:       return kVK_ANSI_KeypadEnter;

        // Application keys
        case KBDKEY_ESCAPE:             return kVK_Escape;
        case KBDKEY_HELP:               return kVK_Help;

        // Media keys
        // Not supported

        // Volume keys
        case KBDKEY_VOLUME_UP:          return kVK_VolumeUp;
        case KBDKEY_VOLUME_DOWN:        return kVK_VolumeDown;
        case KBDKEY_VOLUME_MUTE:        return kVK_Mute;

        // Lock keys
        case KBDKEY_CAPS_LOCK:          return kVK_CapsLock;

        // OEM (Original Equipment Manufacturer) keys
        // The specific values of the following keys may vary on different devices.
        // All of the following uses the standard keyboard layout of the United States.
        case KBDKEY_LEFT_QUOTE:         return kVK_ANSI_Grave;
        case KBDKEY_MINUS:              return kVK_ANSI_Minus;
        case KBDKEY_EQUAL:              return kVK_ANSI_Equal;
        case KBDKEY_LEFT_BRACKET:       return kVK_ANSI_LeftBracket;
        case KBDKEY_RIGHT_BRACKET:      return kVK_ANSI_RightBracket;
        case KBDKEY_SEMICOLON:          return kVK_ANSI_Semicolon;
        case KBDKEY_APOSTROPHE:         return kVK_ANSI_Quote;
        case KBDKEY_COMMA:              return kVK_ANSI_Comma;
        case KBDKEY_PERIOD:             return kVK_ANSI_Period;
        case KBDKEY_SLASH:              return kVK_ANSI_Slash;
        case KBDKEY_BACKSLASH:          return kVK_ANSI_Backslash;
        case KBDKEY_ANGLE_BRACKET:      return 0;   // Not supported

        // Modifiers keys
        case KBDKEY_MOD_META:           return kVK_Command;
        case KBDKEY_MOD_META_LEFT:      return 0;   // Not supported
        case KBDKEY_MOD_META_RIGHT:     return kVK_RightCommand;
        case KBDKEY_MOD_CTRL:           return kVK_Control;
        case KBDKEY_MOD_CTRL_LEFT:      return 0;   // Not supported
        case KBDKEY_MOD_CTRL_RIGHT:     return kVK_RightControl;
        case KBDKEY_MOD_ALT:            return kVK_Option;
        case KBDKEY_MOD_ALT_LEFT:       return 0;   // Not supported
        case KBDKEY_MOD_ALT_RIGHT:      return kVK_RightOption;
        case KBDKEY_MOD_SHIFT:          return kVK_Shift;
        case KBDKEY_MOD_SHIFT_LEFT:     return 0;   // Not supported
        case KBDKEY_MOD_SHIFT_RIGHT:    return kVK_RightShift;

        default:                        return 0;
    }
}

KBDT_API enum keyboard_key kbdt_keyboard_key_from_native(int native_key)
{
    switch (native_key)
    {
        // Number keys
        case kVK_ANSI_0:                return KBDKEY_0;
        case kVK_ANSI_1:                return KBDKEY_1;
        case kVK_ANSI_2:                return KBDKEY_2;
        case kVK_ANSI_3:                return KBDKEY_3;
        case kVK_ANSI_4:                return KBDKEY_4;
        case kVK_ANSI_5:                return KBDKEY_5;
        case kVK_ANSI_6:                return KBDKEY_6;
        case kVK_ANSI_7:                return KBDKEY_7;
        case kVK_ANSI_8:                return KBDKEY_8;
        case kVK_ANSI_9:                return KBDKEY_9;

        // ASCII letter keys
        case kVK_ANSI_A:                return KBDKEY_A;
        case kVK_ANSI_B:                return KBDKEY_B;
        case kVK_ANSI_C:                return KBDKEY_C;
        case kVK_ANSI_D:                return KBDKEY_D;
        case kVK_ANSI_E:                return KBDKEY_E;
        case kVK_ANSI_F:                return KBDKEY_F;
        case kVK_ANSI_G:                return KBDKEY_G;
        case kVK_ANSI_H:                return KBDKEY_H;
        case kVK_ANSI_I:                return KBDKEY_I;
        case kVK_ANSI_J:                return KBDKEY_J;
        case kVK_ANSI_K:                return KBDKEY_K;
        case kVK_ANSI_L:                return KBDKEY_L;
        case kVK_ANSI_M:                return KBDKEY_M;
        case kVK_ANSI_N:                return KBDKEY_N;
        case kVK_ANSI_O:                return KBDKEY_O;
        case kVK_ANSI_P:                return KBDKEY_P;
        case kVK_ANSI_Q:                return KBDKEY_Q;
        case kVK_ANSI_R:                return KBDKEY_R;
        case kVK_ANSI_S:                return KBDKEY_S;
        case kVK_ANSI_T:                return KBDKEY_T;
        case kVK_ANSI_U:                return KBDKEY_U;
        case kVK_ANSI_V:                return KBDKEY_V;
        case kVK_ANSI_W:                return KBDKEY_W;
        case kVK_ANSI_X:                return KBDKEY_X;
        case kVK_ANSI_Y:                return KBDKEY_Y;
        case kVK_ANSI_Z:                return KBDKEY_Z;

        // Whitespace keys
        case kVK_Tab:                   return KBDKEY_TAB;
        case kVK_Space:                 return KBDKEY_SPACE;
        case kVK_Return:                return KBDKEY_ENTER;

        // Navigation keys
        case kVK_Home:                  return KBDKEY_HOME;
        case kVK_End:                   return KBDKEY_END;
        case kVK_PageUp:                return KBDKEY_PAGE_UP;
        case kVK_PageDown:              return KBDKEY_PAGE_DOWN;
        case kVK_LeftArrow:             return KBDKEY_LEFT;
        case kVK_UpArrow:               return KBDKEY_UP;
        case kVK_RightArrow:            return KBDKEY_RIGHT;
        case kVK_DownArrow:             return KBDKEY_DOWN;

        // Editing keys
        case kVK_Delete:                return KBDKEY_BACKSPACE;
        case kVK_ForwardDelete:         return KBDKEY_DELETE;
        case kVK_ANSI_KeypadClear:      return KBDKEY_CLEAR;

        // Function keys
        case kVK_F1:                    return KBDKEY_F1;
        case kVK_F2:                    return KBDKEY_F2;
        case kVK_F3:                    return KBDKEY_F3;
        case kVK_F4:                    return KBDKEY_F4;
        case kVK_F5:                    return KBDKEY_F5;
        case kVK_F6:                    return KBDKEY_F6;
        case kVK_F7:                    return KBDKEY_F7;
        case kVK_F8:                    return KBDKEY_F8;
        case kVK_F9:                    return KBDKEY_F9;
        case kVK_F10:                   return KBDKEY_F10;
        case kVK_F11:                   return KBDKEY_F11;
        case kVK_F12:                   return KBDKEY_F12;
        case kVK_F13:                   return KBDKEY_F13;
        case kVK_F14:                   return KBDKEY_F14;
        case kVK_F15:                   return KBDKEY_F15;
        case kVK_F16:                   return KBDKEY_F16;
        case kVK_F17:                   return KBDKEY_F17;
        case kVK_F18:                   return KBDKEY_F18;
        case kVK_F19:                   return KBDKEY_F19;
        case kVK_F20:                   return KBDKEY_F20;

        // Numpad number keys
        case kVK_ANSI_Keypad0:          return KBDKEY_NUMPAD_0;
        case kVK_ANSI_Keypad1:          return KBDKEY_NUMPAD_1;
        case kVK_ANSI_Keypad2:          return KBDKEY_NUMPAD_2;
        case kVK_ANSI_Keypad3:          return KBDKEY_NUMPAD_3;
        case kVK_ANSI_Keypad4:          return KBDKEY_NUMPAD_4;
        case kVK_ANSI_Keypad5:          return KBDKEY_NUMPAD_5;
        case kVK_ANSI_Keypad6:          return KBDKEY_NUMPAD_6;
        case kVK_ANSI_Keypad7:          return KBDKEY_NUMPAD_7;
        case kVK_ANSI_Keypad8:          return KBDKEY_NUMPAD_8;
        case kVK_ANSI_Keypad9:          return KBDKEY_NUMPAD_9;

        // Numpad operator keys
        case kVK_ANSI_KeypadPlus:       return KBDKEY_NUMPAD_ADD;
        case kVK_ANSI_KeypadMinus:      return KBDKEY_NUMPAD_SUBTRACT;
        case kVK_ANSI_KeypadMultiply:   return KBDKEY_NUMPAD_MULTIPLY;
        case kVK_ANSI_KeypadDivide:     return KBDKEY_NUMPAD_DIVIDE;
        case kVK_ANSI_KeypadDecimal:    return KBDKEY_NUMPAD_DECIMAL;
        case kVK_ANSI_KeypadEquals:     return KBDKEY_NUMPAD_EQUAL;
        case kVK_ANSI_KeypadEnter:      return KBDKEY_NUMPAD_ENTER;

        // Application keys
        case kVK_Escape:                return KBDKEY_ESCAPE;
        case kVK_Help:                  return KBDKEY_HELP;

        // Volume keys
        case kVK_VolumeUp:              return KBDKEY_VOLUME_UP;
        case kVK_VolumeDown:            return KBDKEY_VOLUME_DOWN;
        case kVK_Mute:                  return KBDKEY_VOLUME_MUTE;

        // Lock keys
        case kVK_CapsLock:              return KBDKEY_CAPS_LOCK;

        // OEM (Original Equipment Manufacturer) keys
        // The specific values of the following keys may vary on different devices.
        // All of the following uses the standard keyboard layout of the United States.
        case kVK_ANSI_Grave:            return KBDKEY_LEFT_QUOTE;
        case kVK_ANSI_Minus:            return KBDKEY_MINUS;
        case kVK_ANSI_Equal:            return KBDKEY_EQUAL;
        case kVK_ANSI_LeftBracket:      return KBDKEY_LEFT_BRACKET;
        case kVK_ANSI_RightBracket:     return KBDKEY_RIGHT_BRACKET;
        case kVK_ANSI_Semicolon:        return KBDKEY_SEMICOLON;
        case kVK_ANSI_Quote:            return KBDKEY_APOSTROPHE;
        case kVK_ANSI_Comma:            return KBDKEY_COMMA;
        case kVK_ANSI_Period:           return KBDKEY_PERIOD;
        case kVK_ANSI_Slash:            return KBDKEY_SLASH;
        case kVK_ANSI_Backslash:        return KBDKEY_BACKSLASH;

        // Modifiers keys
        case kVK_Command:               return KBDKEY_MOD_META;
        case kVK_RightCommand:          return KBDKEY_MOD_META_RIGHT;
        case kVK_Control:               return KBDKEY_MOD_CTRL;
        case kVK_RightControl:          return KBDKEY_MOD_CTRL_RIGHT;
        case kVK_Option:                return KBDKEY_MOD_ALT;
        case kVK_RightOption:           return KBDKEY_MOD_ALT_RIGHT;
        case kVK_Shift:                 return KBDKEY_MOD_SHIFT;
        case kVK_RightShift:            return KBDKEY_MOD_SHIFT_RIGHT;

        default:                        return (enum keyboard_key) 0;
    }
}
