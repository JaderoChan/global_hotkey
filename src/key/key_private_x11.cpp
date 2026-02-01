#include "key_private_x11.hpp"

#include <X11/Xlib.h>
#include <X11/Xutil.h>

namespace gbhk
{

int modifiersToX11Modifiers(const Modifiers& modifiers) noexcept
{
    int mod = 0;
    if (modifiers.has(META))
        mod |= Mod4Mask;
    if (modifiers.has(CTRL))
        mod |= ControlMask;
    if (modifiers.has(ALT))
        mod |= Mod1Mask;
    if (modifiers.has(SHIFT))
        mod |= ShiftMask;
    return mod;
}

int keyToX11Keysym(const Key& key) noexcept
{
    switch (key)
    {
        // Number keys
        case Key_0:                 return XK_0;
        case Key_1:                 return XK_1;
        case Key_2:                 return XK_2;
        case Key_3:                 return XK_3;
        case Key_4:                 return XK_4;
        case Key_5:                 return XK_5;
        case Key_6:                 return XK_6;
        case Key_7:                 return XK_7;
        case Key_8:                 return XK_8;
        case Key_9:                 return XK_9;

        // ASCII letter keys
        case Key_A:                 return XK_A;
        case Key_B:                 return XK_B;
        case Key_C:                 return XK_C;
        case Key_D:                 return XK_D;
        case Key_E:                 return XK_E;
        case Key_F:                 return XK_F;
        case Key_G:                 return XK_G;
        case Key_H:                 return XK_H;
        case Key_I:                 return XK_I;
        case Key_J:                 return XK_J;
        case Key_K:                 return XK_K;
        case Key_L:                 return XK_L;
        case Key_M:                 return XK_M;
        case Key_N:                 return XK_N;
        case Key_O:                 return XK_O;
        case Key_P:                 return XK_P;
        case Key_Q:                 return XK_Q;
        case Key_R:                 return XK_R;
        case Key_S:                 return XK_S;
        case Key_T:                 return XK_T;
        case Key_U:                 return XK_U;
        case Key_V:                 return XK_V;
        case Key_W:                 return XK_W;
        case Key_X:                 return XK_X;
        case Key_Y:                 return XK_Y;
        case Key_Z:                 return XK_Z;

        // Whitespace keys
        case Key_Tab:               return XK_Tab;
        case Key_Space:             return XK_space;
        case Key_Enter:             return XK_Return;

        // Navigation keys
        case Key_Home:              return XK_Home;
        case Key_End:               return XK_End;
        case Key_Page_Up:           return XK_Page_Up;
        case Key_Page_Down:         return XK_Page_Down;
        case Key_Left:              return XK_Left;
        case Key_Up:                return XK_Up;
        case Key_Right:             return XK_Right;
        case Key_Down:              return XK_Down;

        // Editing keys
        case Key_Backspace:         return XK_BackSpace;
        case Key_Insert:            return XK_Insert;
        case Key_Delete:            return XK_Delete;
        case Key_Clear:             return XK_Clear;

        // Function keys
        case Key_F1:                return XK_F1;
        case Key_F2:                return XK_F2;
        case Key_F3:                return XK_F3;
        case Key_F4:                return XK_F4;
        case Key_F5:                return XK_F5;
        case Key_F6:                return XK_F6;
        case Key_F7:                return XK_F7;
        case Key_F8:                return XK_F8;
        case Key_F9:                return XK_F9;
        case Key_F10:               return XK_F10;
        case Key_F11:               return XK_F11;
        case Key_F12:               return XK_F12;
        case Key_F13:               return XK_F13;
        case Key_F14:               return XK_F14;
        case Key_F15:               return XK_F15;
        case Key_F16:               return XK_F16;
        case Key_F17:               return XK_F17;
        case Key_F18:               return XK_F18;
        case Key_F19:               return XK_F19;
        case Key_F20:               return XK_F20;
        case Key_F21:               return XK_F21;
        case Key_F22:               return XK_F22;
        case Key_F23:               return XK_F23;
        case Key_F24:               return XK_F24;

        // Numpad number keys
        case Key_Numpad_0:          return XK_KP_0;
        case Key_Numpad_1:          return XK_KP_1;
        case Key_Numpad_2:          return XK_KP_2;
        case Key_Numpad_3:          return XK_KP_3;
        case Key_Numpad_4:          return XK_KP_4;
        case Key_Numpad_5:          return XK_KP_5;
        case Key_Numpad_6:          return XK_KP_6;
        case Key_Numpad_7:          return XK_KP_7;
        case Key_Numpad_8:          return XK_KP_8;
        case Key_Numpad_9:          return XK_KP_9;

        // Numpad operator keys
        case Key_Numpad_Add:        return XK_KP_Add;
        case Key_Numpad_Subtract:   return XK_KP_Subtract;
        case Key_Numpad_Multiply:   return XK_KP_Multiply;
        case Key_Numpad_Divide:     return XK_KP_Divide;
        case Key_Numpad_Decimal:    return XK_KP_Decimal;
        case Key_Numpad_Separator:  return XK_KP_Separator;
        case Key_Numpad_Equal:      return XK_KP_Equal;
        case Key_Numpad_Enter:      return XK_KP_Enter;

        // Application keys
        case Key_Escape:            return XK_Escape;
        case Key_Pause:             return XK_Pause;
        case Key_Play:              return 0;   // Not supported
        case Key_Help:              return XK_Help;
        case Key_Menu:              return XK_Menu;

        // Lock keys
        case Key_Caps_Lock:         return XK_Caps_Lock;
        case Key_Num_Lock:          return XK_Num_Lock;
        case Key_Scroll_Lock:       return XK_Scroll_Lock;

        // Device keys
        case Key_Print_Screen:      return XK_Print;
        case Key_Sleep:             return 0;   // Not supported

        // OEM (Original Equipment Manufacturer) keys
        // The specific values of the following keys may vary on different devices.
        // All of the following uses the standard keyboard layout of the United States.
        case Key_Left_Quote:        return XK_grave;
        case Key_Minus:             return XK_minus;
        case Key_Equal:             return XK_equal;
        case Key_Left_Bracket:      return XK_bracketleft;
        case Key_Right_Bracket:     return XK_bracketright;
        case Key_Semicolon:         return XK_semicolon;
        case Key_Apostrophe:        return XK_apostrophe;
        case Key_Comma:             return XK_comma;
        case Key_Period:            return XK_period;
        case Key_Slash:             return XK_slash;
        case Key_Backslash:         return XK_backslash;
        case Key_Angle_Bracket:     return XK_less; // Need to check

        // Modifier keys
        case Key_Mod_Meta:          return 0;   // Not supported
        case Key_Mod_Meta_Left:     return XK_Meta_L;
        case Key_Mod_Meta_Right:    return XK_Meta_R;
        case Key_Mod_Ctrl:          return 0;   // Not supported
        case Key_Mod_Ctrl_Left:     return XK_Control_L;
        case Key_Mod_Ctrl_Right:    return XK_Control_R;
        case Key_Mod_Alt:           return 0;   // Not supported
        case Key_Mod_Alt_Left:      return XK_Alt_L;
        case Key_Mod_Alt_Right:     return XK_Alt_R;
        case Key_Mod_Shift:         return 0;   // Not supported
        case Key_Mod_Shift_Left:    return XK_Shift_L;
        case Key_Mod_Shift_Right:   return XK_Shift_R;

        default:                    return 0;
    }
}

Modifiers modifiersFromX11Modifiers(int x11Modifiers) noexcept
{
    Modifiers mod;
    if (x11Modifiers & Mod4Mask)
        mod.add(META);
    if (x11Modifiers & ControlMask)
        mod.add(CTRL);
    if (x11Modifiers & Mod1Mask)
        mod.add(ALT);
    if (x11Modifiers & ShiftMask)
        mod.add(SHIFT);
    return mod;
}

Key keyFromX11Keysym(int x11Keysym) noexcept
{
    switch (x11Keysym)
    {
        // Number keys
        case XK_0:                  return Key_0;
        case XK_1:                  return Key_1;
        case XK_2:                  return Key_2;
        case XK_3:                  return Key_3;
        case XK_4:                  return Key_4;
        case XK_5:                  return Key_5;
        case XK_6:                  return Key_6;
        case XK_7:                  return Key_7;
        case XK_8:                  return Key_8;
        case XK_9:                  return Key_9;

        // ASCII letter keys
        case XK_A:                  return Key_A;
        case XK_B:                  return Key_B;
        case XK_C:                  return Key_C;
        case XK_D:                  return Key_D;
        case XK_E:                  return Key_E;
        case XK_F:                  return Key_F;
        case XK_G:                  return Key_G;
        case XK_H:                  return Key_H;
        case XK_I:                  return Key_I;
        case XK_J:                  return Key_J;
        case XK_K:                  return Key_K;
        case XK_L:                  return Key_L;
        case XK_M:                  return Key_M;
        case XK_N:                  return Key_N;
        case XK_O:                  return Key_O;
        case XK_P:                  return Key_P;
        case XK_Q:                  return Key_Q;
        case XK_R:                  return Key_R;
        case XK_S:                  return Key_S;
        case XK_T:                  return Key_T;
        case XK_U:                  return Key_U;
        case XK_V:                  return Key_V;
        case XK_W:                  return Key_W;
        case XK_X:                  return Key_X;
        case XK_Y:                  return Key_Y;
        case XK_Z:                  return Key_Z;

        // Whitespace keys
        case XK_Tab:                return Key_Tab;
        case XK_space:              return Key_Space;
        case XK_Return:             return Key_Enter;

        // Navigation keys
        case XK_Home:               return Key_Home;
        case XK_End:                return Key_End;
        case XK_Page_Up:            return Key_Page_Up;
        case XK_Page_Down:          return Key_Page_Down;
        case XK_Left:               return Key_Left;
        case XK_Up:                 return Key_Up;
        case XK_Right:              return Key_Right;
        case XK_Down:               return Key_Down;

        // Editing keys
        case XK_BackSpace:          return Key_Backspace;
        case XK_Insert:             return Key_Insert;
        case XK_Delete:             return Key_Delete;
        case XK_Clear:              return Key_Clear;

        // Function keys
        case XK_F1:                 return Key_F1;
        case XK_F2:                 return Key_F2;
        case XK_F3:                 return Key_F3;
        case XK_F4:                 return Key_F4;
        case XK_F5:                 return Key_F5;
        case XK_F6:                 return Key_F6;
        case XK_F7:                 return Key_F7;
        case XK_F8:                 return Key_F8;
        case XK_F9:                 return Key_F9;
        case XK_F10:                return Key_F10;
        case XK_F11:                return Key_F11;
        case XK_F12:                return Key_F12;
        case XK_F13:                return Key_F13;
        case XK_F14:                return Key_F14;
        case XK_F15:                return Key_F15;
        case XK_F16:                return Key_F16;
        case XK_F17:                return Key_F17;
        case XK_F18:                return Key_F18;
        case XK_F19:                return Key_F19;
        case XK_F20:                return Key_F20;
        case XK_F21:                return Key_F21;
        case XK_F22:                return Key_F22;
        case XK_F23:                return Key_F23;
        case XK_F24:                return Key_F24;

        // Numpad number keys
        case XK_KP_0:               return Key_Numpad_0;
        case XK_KP_1:               return Key_Numpad_1;
        case XK_KP_2:               return Key_Numpad_2;
        case XK_KP_3:               return Key_Numpad_3;
        case XK_KP_4:               return Key_Numpad_4;
        case XK_KP_5:               return Key_Numpad_5;
        case XK_KP_6:               return Key_Numpad_6;
        case XK_KP_7:               return Key_Numpad_7;
        case XK_KP_8:               return Key_Numpad_8;
        case XK_KP_9:               return Key_Numpad_9;

        // Numpad operator keys
        case XK_KP_Add:             return Key_Numpad_Add;
        case XK_KP_Subtract:        return Key_Numpad_Subtract;
        case XK_KP_Multiply:        return Key_Numpad_Multiply;
        case XK_KP_Divide:          return Key_Numpad_Divide;
        case XK_KP_Decimal:         return Key_Numpad_Decimal;
        case XK_KP_Separator:       return Key_Numpad_Separator;
        case XK_KP_Equal:           return Key_Numpad_Equal;
        case XK_KP_Enter:           return Key_Numpad_Enter;

        // Application keys
        case XK_Escape:             return Key_Escape;
        case XK_Pause:              return Key_Pause;
        case XK_Help:               return Key_Help;
        case XK_Menu:               return Key_Menu;

        // Lock keys
        case XK_Caps_Lock:          return Key_Caps_Lock;
        case XK_Num_Lock:           return Key_Num_Lock;
        case XK_Scroll_Lock:        return Key_Scroll_Lock;

        // Device keys
        case XK_Print:              return Key_Print_Screen;

        // OEM (Original Equipment Manufacturer) keys
        // The specific values of the following keys may vary on different devices.
        // All of the following uses the standard keyboard layout of the United States.
        case XK_grave:              return Key_Left_Quote;
        case XK_minus:              return Key_Minus;
        case XK_equal:              return Key_Equal;
        case XK_bracketleft:        return Key_Left_Bracket;
        case XK_bracketright:       return Key_Right_Bracket;
        case XK_semicolon:          return Key_Semicolon;
        case XK_apostrophe:         return Key_Apostrophe;
        case XK_comma:              return Key_Comma;
        case XK_period:             return Key_Period;
        case XK_slash:              return Key_Slash;
        case XK_backslash:          return Key_Backslash;
        case XK_less:               return Key_Angle_Bracket; // Need to check

        // Modifier keys
        case XK_Meta_L:             return Key_Mod_Meta_Left;
        case XK_Meta_R:             return Key_Mod_Meta_Right;
        case XK_Control_L:          return Key_Mod_Ctrl_Left;
        case XK_Control_R:          return Key_Mod_Ctrl_Right;
        case XK_Alt_L:              return Key_Mod_Alt_Left;
        case XK_Alt_R:              return Key_Mod_Alt_Right;
        case XK_Shift_L:            return Key_Mod_Shift_Left;
        case XK_Shift_R:            return Key_Mod_Shift_Right;

        default:                    return Key();
    }
}

} // namespace gbhk
