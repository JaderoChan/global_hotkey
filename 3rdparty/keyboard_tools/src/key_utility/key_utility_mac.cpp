#include <keyboard_tools/key_utility.hpp>

#include <Carbon/Carbon.h>

namespace kbdt
{

KEYBOARD_TOOLS_API uint32_t keyToNativeKey(Key key) noexcept
{
    switch (key)
    {
        // Number keys
        case Key_0:                 return kVK_ANSI_0;
        case Key_1:                 return kVK_ANSI_1;
        case Key_2:                 return kVK_ANSI_2;
        case Key_3:                 return kVK_ANSI_3;
        case Key_4:                 return kVK_ANSI_4;
        case Key_5:                 return kVK_ANSI_5;
        case Key_6:                 return kVK_ANSI_6;
        case Key_7:                 return kVK_ANSI_7;
        case Key_8:                 return kVK_ANSI_8;
        case Key_9:                 return kVK_ANSI_9;

        // ASCII letter keys
        case Key_A:                 return kVK_ANSI_A;
        case Key_B:                 return kVK_ANSI_B;
        case Key_C:                 return kVK_ANSI_C;
        case Key_D:                 return kVK_ANSI_D;
        case Key_E:                 return kVK_ANSI_E;
        case Key_F:                 return kVK_ANSI_F;
        case Key_G:                 return kVK_ANSI_G;
        case Key_H:                 return kVK_ANSI_H;
        case Key_I:                 return kVK_ANSI_I;
        case Key_J:                 return kVK_ANSI_J;
        case Key_K:                 return kVK_ANSI_K;
        case Key_L:                 return kVK_ANSI_L;
        case Key_M:                 return kVK_ANSI_M;
        case Key_N:                 return kVK_ANSI_N;
        case Key_O:                 return kVK_ANSI_O;
        case Key_P:                 return kVK_ANSI_P;
        case Key_Q:                 return kVK_ANSI_Q;
        case Key_R:                 return kVK_ANSI_R;
        case Key_S:                 return kVK_ANSI_S;
        case Key_T:                 return kVK_ANSI_T;
        case Key_U:                 return kVK_ANSI_U;
        case Key_V:                 return kVK_ANSI_V;
        case Key_W:                 return kVK_ANSI_W;
        case Key_X:                 return kVK_ANSI_X;
        case Key_Y:                 return kVK_ANSI_Y;
        case Key_Z:                 return kVK_ANSI_Z;

        // Whitespace keys
        case Key_Tab:               return kVK_Tab;
        case Key_Space:             return kVK_Space;
        case Key_Enter:             return kVK_Return;

        // Navigation keys
        case Key_Home:              return kVK_Home;
        case Key_End:               return kVK_End;
        case Key_Page_Up:           return kVK_PageUp;
        case Key_Page_Down:         return kVK_PageDown;
        case Key_Left:              return kVK_LeftArrow;
        case Key_Up:                return kVK_UpArrow;
        case Key_Right:             return kVK_RightArrow;
        case Key_Down:              return kVK_DownArrow;

        // Editing keys
        case Key_Backspace:         return kVK_Delete;
        case Key_Insert:            return 0;   // Not supported
        case Key_Delete:            return kVK_ForwardDelete;
        case Key_Clear:             return kVK_ANSI_KeypadClear;

        // Function keys
        case Key_F1:                return kVK_F1;
        case Key_F2:                return kVK_F2;
        case Key_F3:                return kVK_F3;
        case Key_F4:                return kVK_F4;
        case Key_F5:                return kVK_F5;
        case Key_F6:                return kVK_F6;
        case Key_F7:                return kVK_F7;
        case Key_F8:                return kVK_F8;
        case Key_F9:                return kVK_F9;
        case Key_F10:               return kVK_F10;
        case Key_F11:               return kVK_F11;
        case Key_F12:               return kVK_F12;
        case Key_F13:               return kVK_F13;
        case Key_F14:               return kVK_F14;
        case Key_F15:               return kVK_F15;
        case Key_F16:               return kVK_F16;
        case Key_F17:               return kVK_F17;
        case Key_F18:               return kVK_F18;
        case Key_F19:               return kVK_F19;
        case Key_F20:               return kVK_F20;

        // Numpad number keys
        case Key_Numpad_0:          return kVK_ANSI_Keypad0;
        case Key_Numpad_1:          return kVK_ANSI_Keypad1;
        case Key_Numpad_2:          return kVK_ANSI_Keypad2;
        case Key_Numpad_3:          return kVK_ANSI_Keypad3;
        case Key_Numpad_4:          return kVK_ANSI_Keypad4;
        case Key_Numpad_5:          return kVK_ANSI_Keypad5;
        case Key_Numpad_6:          return kVK_ANSI_Keypad6;
        case Key_Numpad_7:          return kVK_ANSI_Keypad7;
        case Key_Numpad_8:          return kVK_ANSI_Keypad8;
        case Key_Numpad_9:          return kVK_ANSI_Keypad9;

        // Numpad operator keys
        case Key_Numpad_Add:        return kVK_ANSI_KeypadPlus;
        case Key_Numpad_Subtract:   return kVK_ANSI_KeypadMinus;
        case Key_Numpad_Multiply:   return kVK_ANSI_KeypadMultiply;
        case Key_Numpad_Divide:     return kVK_ANSI_KeypadDivide;
        case Key_Numpad_Decimal:    return kVK_ANSI_KeypadDecimal;
        case Key_Numpad_Separator:  return 0;   // Not supported
        case Key_Numpad_Equal:      return kVK_ANSI_KeypadEquals;
        case Key_Numpad_Enter:      return kVK_ANSI_KeypadEnter;

        // Application keys
        case Key_Escape:            return kVK_Escape;
        case Key_Help:              return kVK_Help;

        // Media keys
        // Not supported

        // Volume keys
        case Key_Volume_Up:         return kVK_VolumeUp;
        case Key_Volume_Down:       return kVK_VolumeDown;
        case Key_Volume_Mute:       return kVK_Mute;

        // Lock keys
        case Key_Caps_Lock:         return kVK_CapsLock;

        // OEM (Original Equipment Manufacturer) keys
        // The specific values of the following keys may vary on different devices.
        // All of the following uses the standard keyboard layout of the United States.
        case Key_Left_Quote:        return kVK_ANSI_Grave;
        case Key_Minus:             return kVK_ANSI_Minus;
        case Key_Equal:             return kVK_ANSI_Equal;
        case Key_Left_Bracket:      return kVK_ANSI_LeftBracket;
        case Key_Right_Bracket:     return kVK_ANSI_RightBracket;
        case Key_Semicolon:         return kVK_ANSI_Semicolon;
        case Key_Apostrophe:        return kVK_ANSI_Quote;
        case Key_Comma:             return kVK_ANSI_Comma;
        case Key_Period:            return kVK_ANSI_Period;
        case Key_Slash:             return kVK_ANSI_Slash;
        case Key_Backslash:         return kVK_ANSI_Backslash;
        case Key_Angle_Bracket:     return 0;   // Not supported

        // Modifiers keys
        case Key_Meta:          return kVK_Command;
        case Key_Meta_Left:     return 0;   // Not supported
        case Key_Meta_Right:    return kVK_RightCommand;
        case Key_Ctrl:          return kVK_Control;
        case Key_Ctrl_Left:     return 0;   // Not supported
        case Key_Ctrl_Right:    return kVK_RightControl;
        case Key_Alt:           return kVK_Option;
        case Key_Alt_Left:      return 0;   // Not supported
        case Key_Alt_Right:     return kVK_RightOption;
        case Key_Shift:         return kVK_Shift;
        case Key_Shift_Left:    return 0;   // Not supported
        case Key_Shift_Right:   return kVK_RightShift;

        default:                    return 0;
    }
}

KEYBOARD_TOOLS_API Key keyFromNativeKey(uint32_t nativeKey) noexcept
{
    switch (nativeKey)
    {
        // Number keys
        case kVK_ANSI_0:                return Key_0;
        case kVK_ANSI_1:                return Key_1;
        case kVK_ANSI_2:                return Key_2;
        case kVK_ANSI_3:                return Key_3;
        case kVK_ANSI_4:                return Key_4;
        case kVK_ANSI_5:                return Key_5;
        case kVK_ANSI_6:                return Key_6;
        case kVK_ANSI_7:                return Key_7;
        case kVK_ANSI_8:                return Key_8;
        case kVK_ANSI_9:                return Key_9;

        // ASCII letter keys
        case kVK_ANSI_A:                return Key_A;
        case kVK_ANSI_B:                return Key_B;
        case kVK_ANSI_C:                return Key_C;
        case kVK_ANSI_D:                return Key_D;
        case kVK_ANSI_E:                return Key_E;
        case kVK_ANSI_F:                return Key_F;
        case kVK_ANSI_G:                return Key_G;
        case kVK_ANSI_H:                return Key_H;
        case kVK_ANSI_I:                return Key_I;
        case kVK_ANSI_J:                return Key_J;
        case kVK_ANSI_K:                return Key_K;
        case kVK_ANSI_L:                return Key_L;
        case kVK_ANSI_M:                return Key_M;
        case kVK_ANSI_N:                return Key_N;
        case kVK_ANSI_O:                return Key_O;
        case kVK_ANSI_P:                return Key_P;
        case kVK_ANSI_Q:                return Key_Q;
        case kVK_ANSI_R:                return Key_R;
        case kVK_ANSI_S:                return Key_S;
        case kVK_ANSI_T:                return Key_T;
        case kVK_ANSI_U:                return Key_U;
        case kVK_ANSI_V:                return Key_V;
        case kVK_ANSI_W:                return Key_W;
        case kVK_ANSI_X:                return Key_X;
        case kVK_ANSI_Y:                return Key_Y;
        case kVK_ANSI_Z:                return Key_Z;

        // Whitespace keys
        case kVK_Tab:                   return Key_Tab;
        case kVK_Space:                 return Key_Space;
        case kVK_Return:                return Key_Enter;

        // Navigation keys
        case kVK_Home:                  return Key_Home;
        case kVK_End:                   return Key_End;
        case kVK_PageUp:                return Key_Page_Up;
        case kVK_PageDown:              return Key_Page_Down;
        case kVK_LeftArrow:             return Key_Left;
        case kVK_UpArrow:               return Key_Up;
        case kVK_RightArrow:            return Key_Right;
        case kVK_DownArrow:             return Key_Down;

        // Editing keys
        case kVK_Delete:                return Key_Backspace;
        case kVK_ForwardDelete:         return Key_Delete;
        case kVK_ANSI_KeypadClear:      return Key_Clear;

        // Function keys
        case kVK_F1:                    return Key_F1;
        case kVK_F2:                    return Key_F2;
        case kVK_F3:                    return Key_F3;
        case kVK_F4:                    return Key_F4;
        case kVK_F5:                    return Key_F5;
        case kVK_F6:                    return Key_F6;
        case kVK_F7:                    return Key_F7;
        case kVK_F8:                    return Key_F8;
        case kVK_F9:                    return Key_F9;
        case kVK_F10:                   return Key_F10;
        case kVK_F11:                   return Key_F11;
        case kVK_F12:                   return Key_F12;
        case kVK_F13:                   return Key_F13;
        case kVK_F14:                   return Key_F14;
        case kVK_F15:                   return Key_F15;
        case kVK_F16:                   return Key_F16;
        case kVK_F17:                   return Key_F17;
        case kVK_F18:                   return Key_F18;
        case kVK_F19:                   return Key_F19;
        case kVK_F20:                   return Key_F20;

        // Numpad number keys
        case kVK_ANSI_Keypad0:          return Key_Numpad_0;
        case kVK_ANSI_Keypad1:          return Key_Numpad_1;
        case kVK_ANSI_Keypad2:          return Key_Numpad_2;
        case kVK_ANSI_Keypad3:          return Key_Numpad_3;
        case kVK_ANSI_Keypad4:          return Key_Numpad_4;
        case kVK_ANSI_Keypad5:          return Key_Numpad_5;
        case kVK_ANSI_Keypad6:          return Key_Numpad_6;
        case kVK_ANSI_Keypad7:          return Key_Numpad_7;
        case kVK_ANSI_Keypad8:          return Key_Numpad_8;
        case kVK_ANSI_Keypad9:          return Key_Numpad_9;

        // Numpad operator keys
        case kVK_ANSI_KeypadPlus:       return Key_Numpad_Add;
        case kVK_ANSI_KeypadMinus:      return Key_Numpad_Subtract;
        case kVK_ANSI_KeypadMultiply:   return Key_Numpad_Multiply;
        case kVK_ANSI_KeypadDivide:     return Key_Numpad_Divide;
        case kVK_ANSI_KeypadDecimal:    return Key_Numpad_Decimal;
        case kVK_ANSI_KeypadEquals:     return Key_Numpad_Equal;
        case kVK_ANSI_KeypadEnter:      return Key_Numpad_Enter;

        // Application keys
        case kVK_Escape:                return Key_Escape;
        case kVK_Help:                  return Key_Help;

        // Volume keys
        case kVK_VolumeUp:              return Key_Volume_Up;
        case kVK_VolumeDown:            return Key_Volume_Down;
        case kVK_Mute:                  return Key_Volume_Mute;

        // Lock keys
        case kVK_CapsLock:              return Key_Caps_Lock;

        // OEM (Original Equipment Manufacturer) keys
        // The specific values of the following keys may vary on different devices.
        // All of the following uses the standard keyboard layout of the United States.
        case kVK_ANSI_Grave:            return Key_Left_Quote;
        case kVK_ANSI_Minus:            return Key_Minus;
        case kVK_ANSI_Equal:            return Key_Equal;
        case kVK_ANSI_LeftBracket:      return Key_Left_Bracket;
        case kVK_ANSI_RightBracket:     return Key_Right_Bracket;
        case kVK_ANSI_Semicolon:        return Key_Semicolon;
        case kVK_ANSI_Quote:            return Key_Apostrophe;
        case kVK_ANSI_Comma:            return Key_Comma;
        case kVK_ANSI_Period:           return Key_Period;
        case kVK_ANSI_Slash:            return Key_Slash;
        case kVK_ANSI_Backslash:        return Key_Backslash;

        // Modifiers keys
        case kVK_Command:               return Key_Meta;
        case kVK_RightCommand:          return Key_Meta_Right;
        case kVK_Control:               return Key_Ctrl;
        case kVK_RightControl:          return Key_Ctrl_Right;
        case kVK_Option:                return Key_Alt;
        case kVK_RightOption:           return Key_Alt_Right;
        case kVK_Shift:                 return Key_Shift;
        case kVK_RightShift:            return Key_Shift_Right;

        default:                        return (Key) 0;
    }
}

} // namespace kbdt
