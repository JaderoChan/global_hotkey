#include <keyboard_tools/key_utilitys.hpp>

#include <windows.h>

namespace kbdt
{

KEYBOARD_TOOLS_API int keyToNativeKey(Key key) noexcept
{
    if ((key >= Key_0 && key <= Key_9) || (key >= Key_A && key <= Key_Z))
        return key;

    switch (key)
    {
        // Whitespace keys
        case Key_Tab:               return VK_TAB;
        case Key_Space:             return VK_SPACE;
        case Key_Enter:             return VK_RETURN;

        // Navigation keys
        case Key_Home:              return VK_HOME;
        case Key_End:               return VK_END;
        case Key_Page_Up:           return VK_PRIOR;
        case Key_Page_Down:         return VK_NEXT;
        case Key_Left:              return VK_LEFT;
        case Key_Up:                return VK_UP;
        case Key_Right:             return VK_RIGHT;
        case Key_Down:              return VK_DOWN;

        // Editing keys
        case Key_Backspace:         return VK_BACK;
        case Key_Insert:            return VK_INSERT;
        case Key_Delete:            return VK_DELETE;
        case Key_Clear:             return VK_CLEAR;

        // Function keys
        case Key_F1:                return VK_F1;
        case Key_F2:                return VK_F2;
        case Key_F3:                return VK_F3;
        case Key_F4:                return VK_F4;
        case Key_F5:                return VK_F5;
        case Key_F6:                return VK_F6;
        case Key_F7:                return VK_F7;
        case Key_F8:                return VK_F8;
        case Key_F9:                return VK_F9;
        case Key_F10:               return VK_F10;
        case Key_F11:               return VK_F11;
        case Key_F12:               return VK_F12;
        case Key_F13:               return VK_F13;
        case Key_F14:               return VK_F14;
        case Key_F15:               return VK_F15;
        case Key_F16:               return VK_F16;
        case Key_F17:               return VK_F17;
        case Key_F18:               return VK_F18;
        case Key_F19:               return VK_F19;
        case Key_F20:               return VK_F20;
        case Key_F21:               return VK_F21;
        case Key_F22:               return VK_F22;
        case Key_F23:               return VK_F23;
        case Key_F24:               return VK_F24;

        // Numpad number keys
        case Key_Numpad_0:          return VK_NUMPAD0;
        case Key_Numpad_1:          return VK_NUMPAD1;
        case Key_Numpad_2:          return VK_NUMPAD2;
        case Key_Numpad_3:          return VK_NUMPAD3;
        case Key_Numpad_4:          return VK_NUMPAD4;
        case Key_Numpad_5:          return VK_NUMPAD5;
        case Key_Numpad_6:          return VK_NUMPAD6;
        case Key_Numpad_7:          return VK_NUMPAD7;
        case Key_Numpad_8:          return VK_NUMPAD8;
        case Key_Numpad_9:          return VK_NUMPAD9;

        // Numpad operator keys
        case Key_Numpad_Add:        return VK_ADD;
        case Key_Numpad_Subtract:   return VK_SUBTRACT;
        case Key_Numpad_Multiply:   return VK_MULTIPLY;
        case Key_Numpad_Divide:     return VK_DIVIDE;
        case Key_Numpad_Decimal:    return VK_DECIMAL;
        case Key_Numpad_Separator:  return VK_SEPARATOR;
        case Key_Numpad_Equal:      return VK_OEM_NEC_EQUAL;    // Need check
        case Key_Numpad_Enter:      return VK_RETURN;

        // Applications keys
        case Key_Escape:            return VK_ESCAPE;
        case Key_Pause:             return VK_PAUSE;
        case Key_Play:              return VK_PLAY;
        case Key_Help:              return VK_HELP;
        case Key_Menu:              return VK_APPS;

        // Media keys
        case Key_Media_Next:        return VK_MEDIA_NEXT_TRACK;
        case Key_Media_Previous:    return VK_MEDIA_PREV_TRACK;
        case Key_Media_Play_Pause:  return VK_MEDIA_PLAY_PAUSE;
        case Key_Media_Stop:        return VK_MEDIA_STOP;

        // Volume keys
        case Key_Volume_Up:         return VK_VOLUME_UP;
        case Key_Volume_Down:       return VK_VOLUME_DOWN;
        case Key_Volume_Mute:       return VK_VOLUME_MUTE;

        // Lock keys
        case Key_Caps_Lock:         return VK_CAPITAL;
        case Key_Num_Lock:          return VK_NUMLOCK;
        case Key_Scroll_Lock:       return VK_SCROLL;

        // Device keys
        case Key_Print_Screen:      return VK_SNAPSHOT;
        case Key_Sleep:             return VK_SLEEP;

        // OEM (Original Equipment Manufacturer) keys
        // The specific values of the following keys may vary on different devices.
        // All of the following uses the standard keyboard layout of the United States.
        case Key_Left_Quote:        return VK_OEM_3;
        case Key_Minus:             return VK_OEM_MINUS;
        case Key_Equal:             return VK_OEM_PLUS;
        case Key_Left_Bracket:      return VK_OEM_4;
        case Key_Right_Bracket:     return VK_OEM_6;
        case Key_Semicolon:         return VK_OEM_1;
        case Key_Apostrophe:        return VK_OEM_7;
        case Key_Comma:             return VK_OEM_COMMA;
        case Key_Period:            return VK_OEM_PERIOD;
        case Key_Slash:             return VK_OEM_2;
        case Key_Backslash:         return VK_OEM_5;
        case Key_Angle_Bracket:     return VK_OEM_102;

        // Modifier keys
        case Key_Meta:          return 0;   // Not supported
        case Key_Meta_Left:     return VK_LWIN;
        case Key_Meta_Right:    return VK_RWIN;
        case Key_Ctrl:          return VK_CONTROL;
        case Key_Ctrl_Left:     return VK_LCONTROL;
        case Key_Ctrl_Right:    return VK_RCONTROL;
        case Key_Alt:           return VK_MENU;
        case Key_Alt_Left:      return VK_LMENU;
        case Key_Alt_Right:     return VK_RMENU;
        case Key_Shift:         return VK_SHIFT;
        case Key_Shift_Left:    return VK_LSHIFT;
        case Key_Shift_Right:   return VK_RSHIFT;

        default:                    return 0;
    }
}

KEYBOARD_TOOLS_API Key keyFromNativeKey(int nativeKey) noexcept
{
    if ((nativeKey >= '0' && nativeKey <= '9') || (nativeKey >= 'A' && nativeKey <= 'Z'))
        return (Key) nativeKey;

    switch (nativeKey)
    {
        // Whitespace keys
        case VK_TAB:                return Key_Tab;
        case VK_SPACE:              return Key_Space;
        case VK_RETURN:             return Key_Enter;

        // Navigation keys
        case VK_HOME:               return Key_Home;
        case VK_END:                return Key_End;
        case VK_PRIOR:              return Key_Page_Up;
        case VK_NEXT:               return Key_Page_Down;
        case VK_LEFT:               return Key_Left;
        case VK_UP:                 return Key_Up;
        case VK_RIGHT:              return Key_Right;
        case VK_DOWN:               return Key_Down;

        // Editing keys
        case VK_BACK:               return Key_Backspace;
        case VK_INSERT:             return Key_Insert;
        case VK_DELETE:             return Key_Delete;
        case VK_CLEAR:              return Key_Clear;

        // Function keys
        case VK_F1:                 return Key_F1;
        case VK_F2:                 return Key_F2;
        case VK_F3:                 return Key_F3;
        case VK_F4:                 return Key_F4;
        case VK_F5:                 return Key_F5;
        case VK_F6:                 return Key_F6;
        case VK_F7:                 return Key_F7;
        case VK_F8:                 return Key_F8;
        case VK_F9:                 return Key_F9;
        case VK_F10:                return Key_F10;
        case VK_F11:                return Key_F11;
        case VK_F12:                return Key_F12;
        case VK_F13:                return Key_F13;
        case VK_F14:                return Key_F14;
        case VK_F15:                return Key_F15;
        case VK_F16:                return Key_F16;
        case VK_F17:                return Key_F17;
        case VK_F18:                return Key_F18;
        case VK_F19:                return Key_F19;
        case VK_F20:                return Key_F20;
        case VK_F21:                return Key_F21;
        case VK_F22:                return Key_F22;
        case VK_F23:                return Key_F23;
        case VK_F24:                return Key_F24;

        // Numpad number keys
        case VK_NUMPAD0:            return Key_Numpad_0;
        case VK_NUMPAD1:            return Key_Numpad_1;
        case VK_NUMPAD2:            return Key_Numpad_2;
        case VK_NUMPAD3:            return Key_Numpad_3;
        case VK_NUMPAD4:            return Key_Numpad_4;
        case VK_NUMPAD5:            return Key_Numpad_5;
        case VK_NUMPAD6:            return Key_Numpad_6;
        case VK_NUMPAD7:            return Key_Numpad_7;
        case VK_NUMPAD8:            return Key_Numpad_8;
        case VK_NUMPAD9:            return Key_Numpad_9;

        // Numpad operator keys
        case VK_ADD:                return Key_Numpad_Add;
        case VK_SUBTRACT:           return Key_Numpad_Subtract;
        case VK_MULTIPLY:           return Key_Numpad_Multiply;
        case VK_DIVIDE:             return Key_Numpad_Divide;
        case VK_DECIMAL:            return Key_Numpad_Decimal;
        case VK_SEPARATOR:          return Key_Numpad_Separator;
        case VK_OEM_NEC_EQUAL:      return Key_Numpad_Equal;

        // Applications keys
        case VK_ESCAPE:             return Key_Escape;
        case VK_PAUSE:              return Key_Pause;
        case VK_PLAY:               return Key_Play;
        case VK_HELP:               return Key_Help;
        case VK_APPS:               return Key_Menu;

        // Media keys
        case VK_MEDIA_NEXT_TRACK:   return Key_Media_Next;
        case VK_MEDIA_PREV_TRACK:   return Key_Media_Previous;
        case VK_MEDIA_PLAY_PAUSE:   return Key_Media_Play_Pause;
        case VK_MEDIA_STOP:         return Key_Media_Stop;

        // Volume keys
        case VK_VOLUME_UP:          return Key_Volume_Up;
        case VK_VOLUME_DOWN:        return Key_Volume_Down;
        case VK_VOLUME_MUTE:        return Key_Volume_Mute;

        // Lock keys
        case VK_CAPITAL:            return Key_Caps_Lock;
        case VK_NUMLOCK:            return Key_Num_Lock;
        case VK_SCROLL:             return Key_Scroll_Lock;

        // Device keys
        case VK_SNAPSHOT:           return Key_Print_Screen;
        case VK_SLEEP:              return Key_Sleep;

        // OEM (Original Equipment Manufacturer) keys
        // The specific values of the following keys may vary on different devices.
        // All of the following uses the standard keyboard layout of the United States.
        case VK_OEM_3:              return Key_Left_Quote;
        case VK_OEM_MINUS:          return Key_Minus;
        case VK_OEM_PLUS:           return Key_Equal;
        case VK_OEM_4:              return Key_Left_Bracket;
        case VK_OEM_6:              return Key_Right_Bracket;
        case VK_OEM_1:              return Key_Semicolon;
        case VK_OEM_7:              return Key_Apostrophe;
        case VK_OEM_COMMA:          return Key_Comma;
        case VK_OEM_PERIOD:         return Key_Period;
        case VK_OEM_2:              return Key_Slash;
        case VK_OEM_5:              return Key_Backslash;
        case VK_OEM_102:            return Key_Angle_Bracket;

        // Modifier keys
        case VK_LWIN:               return Key_Meta_Left;
        case VK_RWIN:               return Key_Meta_Right;
        case VK_CONTROL:            return Key_Ctrl;
        case VK_LCONTROL:           return Key_Ctrl_Left;
        case VK_RCONTROL:           return Key_Ctrl_Right;
        case VK_MENU:               return Key_Alt;
        case VK_LMENU:              return Key_Alt_Left;
        case VK_RMENU:              return Key_Alt_Right;
        case VK_SHIFT:              return Key_Shift;
        case VK_LSHIFT:             return Key_Shift_Left;
        case VK_RSHIFT:             return Key_Shift_Right;

        default:                    return (Key) 0;
    }
}

} // namespace kbdt
