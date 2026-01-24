#include <kbdt/utils.h>

#include <ctype.h>  // isspace, isalnum, toupper
#include <stddef.h>
#include <string.h> // strlen

#define IS_SPACE(c) isspace((unsigned char) c)
#define IS_ALNUM(c) isalnum((unsigned char) c)
#define TO_UPPER(c) toupper((unsigned char) c)

#ifdef _KBDT_WIN
    #define META_TEXT   "Win"
    #define ALT_TEXT    "Alt"
#elif defined(_KBDT_MAC)
    #define META_TEXT   "Command"
    #define ALT_TEXT    "Option"
#elif defined(_KBDT_LINUX)
    #define META_TEXT   "Super"
    #define ALT_TEXT    "Alt"
#else
    #define META_TEXT   "Meta"
    #define ALT_TEXT    "Alt"
#endif // _KBDT_WIN

#define CTRL_TEXT       "Ctrl"
#define SHIFT_TEXT      "Shift"

// Check whether two strings is equal, ignoring case, spaces and underscores.
static bool is_equal_str(const char* str1, const char* str2)
{
    size_t len1 = strlen(str1);
    size_t len2 = strlen(str2);
    size_t i = 0, j = 0;
    while (i < len1 || j < len2)
    {
        while (i < len1 && (IS_SPACE(str1[i]) || str1[i] == '_')) i++;
        while (j < len2 && (IS_SPACE(str2[j]) || str2[j] == '_')) j++;

        if (i >= len1 || j >= len2)
            return (i >= len1 && j >= len2);

        char c1 = TO_UPPER(str1[i]);
        char c2 = TO_UPPER(str2[j]);
        if (c1 != c2)
            return false;

        i++;
        j++;
    }

    return true;
}

#define IS_META(str, prefix) \
(is_equal_str(str, prefix "win") || is_equal_str(str, prefix "windows") || \
is_equal_str(str, prefix "cmd") || is_equal_str(str, prefix "command") || is_equal_str(str, prefix "\xE2\x8C\x98") || \
is_equal_str(str, prefix "super") || is_equal_str(str, prefix "meta"))

#define IS_CTRL(str, prefix) \
(is_equal_str(str, prefix "ctrl") || is_equal_str(str, prefix "control") || \
is_equal_str(str, prefix "^") || is_equal_str(str, prefix "\xE2\x8C\x83"))

#define IS_ALT(str, prefix) \
(is_equal_str(str, prefix "alt") || is_equal_str(str, prefix "option") || is_equal_str(str, prefix "\xE2\x8C\xA5"))

#define IS_SHIFT(str, prefix) \
(is_equal_str(str, prefix "shift") || is_equal_str(str, prefix "\xE2\x87\xAA"))

KBDT_API const char* kbdt_keyboard_key_to_str(enum keyboard_key key)
{
    switch (key)
    {
        // Number keys
        case KBDKEY_0:                  return "0";
        case KBDKEY_1:                  return "1";
        case KBDKEY_2:                  return "2";
        case KBDKEY_3:                  return "3";
        case KBDKEY_4:                  return "4";
        case KBDKEY_5:                  return "5";
        case KBDKEY_6:                  return "6";
        case KBDKEY_7:                  return "7";
        case KBDKEY_8:                  return "8";
        case KBDKEY_9:                  return "9";

        // ASCII letter keys
        case KBDKEY_A:                  return "A";
        case KBDKEY_B:                  return "B";
        case KBDKEY_C:                  return "C";
        case KBDKEY_D:                  return "D";
        case KBDKEY_E:                  return "E";
        case KBDKEY_F:                  return "F";
        case KBDKEY_G:                  return "G";
        case KBDKEY_H:                  return "H";
        case KBDKEY_I:                  return "I";
        case KBDKEY_J:                  return "J";
        case KBDKEY_K:                  return "K";
        case KBDKEY_L:                  return "L";
        case KBDKEY_M:                  return "M";
        case KBDKEY_N:                  return "N";
        case KBDKEY_O:                  return "O";
        case KBDKEY_P:                  return "P";
        case KBDKEY_Q:                  return "Q";
        case KBDKEY_R:                  return "R";
        case KBDKEY_S:                  return "S";
        case KBDKEY_T:                  return "T";
        case KBDKEY_U:                  return "U";
        case KBDKEY_V:                  return "V";
        case KBDKEY_W:                  return "W";
        case KBDKEY_X:                  return "X";
        case KBDKEY_Y:                  return "Y";
        case KBDKEY_Z:                  return "Z";

        // Whitespace keys
        case KBDKEY_TAB:                return "Tab";
        case KBDKEY_SPACE:              return "Space";
        case KBDKEY_ENTER:              return "Enter";

        // Navigation keys
        case KBDKEY_HOME:               return "Home";
        case KBDKEY_END:                return "End";
        case KBDKEY_PAGE_UP:            return "Page Up";
        case KBDKEY_PAGE_DOWN:          return "Page Down";
        case KBDKEY_LEFT:               return "Left";
        case KBDKEY_UP:                 return "Up";
        case KBDKEY_RIGHT:              return "Right";
        case KBDKEY_DOWN:               return "Down";

        // Editing keys
        case KBDKEY_BACKSPACE:          return "Backspace";
        case KBDKEY_INSERT:             return "Insert";
        case KBDKEY_DELETE:             return "Delete";
        case KBDKEY_CLEAR:              return "Clear";

        // Function keys
        case KBDKEY_F1:                 return "F1";
        case KBDKEY_F2:                 return "F2";
        case KBDKEY_F3:                 return "F3";
        case KBDKEY_F4:                 return "F4";
        case KBDKEY_F5:                 return "F5";
        case KBDKEY_F6:                 return "F6";
        case KBDKEY_F7:                 return "F7";
        case KBDKEY_F8:                 return "F8";
        case KBDKEY_F9:                 return "F9";
        case KBDKEY_F10:                return "F10";
        case KBDKEY_F11:                return "F11";
        case KBDKEY_F12:                return "F12";
        case KBDKEY_F13:                return "F13";
        case KBDKEY_F14:                return "F14";
        case KBDKEY_F15:                return "F15";
        case KBDKEY_F16:                return "F16";
        case KBDKEY_F17:                return "F17";
        case KBDKEY_F18:                return "F18";
        case KBDKEY_F19:                return "F19";
        case KBDKEY_F20:                return "F20";
        case KBDKEY_F21:                return "F21";
        case KBDKEY_F22:                return "F22";
        case KBDKEY_F23:                return "F23";
        case KBDKEY_F24:                return "F24";

        // Numpad number keys
        case KBDKEY_NUMPAD_0:           return "Numpad 0";
        case KBDKEY_NUMPAD_1:           return "Numpad 1";
        case KBDKEY_NUMPAD_2:           return "Numpad 2";
        case KBDKEY_NUMPAD_3:           return "Numpad 3";
        case KBDKEY_NUMPAD_4:           return "Numpad 4";
        case KBDKEY_NUMPAD_5:           return "Numpad 5";
        case KBDKEY_NUMPAD_6:           return "Numpad 6";
        case KBDKEY_NUMPAD_7:           return "Numpad 7";
        case KBDKEY_NUMPAD_8:           return "Numpad 8";
        case KBDKEY_NUMPAD_9:           return "Numpad 9";

        // Numpad operator keys
        case KBDKEY_NUMPAD_ADD:         return "Numpad Add";
        case KBDKEY_NUMPAD_SUBTRACT:    return "Numpad Subtract";
        case KBDKEY_NUMPAD_MULTIPLY:    return "Numpad Multiply";
        case KBDKEY_NUMPAD_DIVIDE:      return "Numpad Divide";
        case KBDKEY_NUMPAD_DECIMAL:     return "Numpad Decimal";
        case KBDKEY_NUMPAD_SEPARATOR:   return "Numpad Separator";
        case KBDKEY_NUMPAD_EQUAL:       return "Numpad Equal";
        case KBDKEY_NUMPAD_ENTER:       return "Numpad Enter";

        // Applications keys
        case KBDKEY_ESCAPE:             return "Esc";
        case KBDKEY_PAUSE:              return "Pause";
        case KBDKEY_PLAY:               return "Play";
        case KBDKEY_HELP:               return "Help";
        case KBDKEY_MENU:               return "Menu";

        // Media keys
        case KBDKEY_MEDIA_NEXT:         return "Media Next";
        case KBDKEY_MEDIA_PREVIOUS:     return "Media Previous";
        case KBDKEY_MEDIA_PLAY_PAUSE:   return "Media Play/Pause";
        case KBDKEY_MEDIA_STOP:         return "Media Stop";

        // Volume keys
        case KBDKEY_VOLUME_UP:          return "Volume Up";
        case KBDKEY_VOLUME_DOWN:        return "Volume Down";
        case KBDKEY_VOLUME_MUTE:        return "Volume Mute";

        // Lock keys
        case KBDKEY_CAPS_LOCK:          return "CapsLock";
        case KBDKEY_NUM_LOCK:           return "NumLock";
        case KBDKEY_SCROLL_LOCK:        return "ScrollLock";

        // Device keys
        case KBDKEY_PRINT_SCREEN:       return "Print Screen";
        case KBDKEY_SLEEP:              return "Sleep";

        // OEM (Original Equipment Manufacturer) keys
        // The specific values of the following keys may vary on different devices.
        // All of the following uses the standard keyboard layout of the United States.
        case KBDKEY_LEFT_QUOTE:         return "`";
        case KBDKEY_MINUS:              return "-";
        case KBDKEY_EQUAL:              return "=";
        case KBDKEY_LEFT_BRACKET:       return "[";
        case KBDKEY_RIGHT_BRACKET:      return "]";
        case KBDKEY_SEMICOLON:          return ";";
        case KBDKEY_APOSTROPHE:         return "'";
        case KBDKEY_COMMA:              return ",";
        case KBDKEY_PERIOD:             return ".";
        case KBDKEY_SLASH:              return "/";
        case KBDKEY_BACKSLASH:          return "\\";
        case KBDKEY_ANGLE_BRACKET:      return "<>";

        // Modifier keys
        case KBDKEY_MOD_META:           return META_TEXT;
        case KBDKEY_MOD_META_LEFT:      return ("Left " META_TEXT);
        case KBDKEY_MOD_META_RIGHT:     return ("Right " META_TEXT);
        case KBDKEY_MOD_CTRL:           return CTRL_TEXT;
        case KBDKEY_MOD_CTRL_LEFT:      return ("Left " CTRL_TEXT);
        case KBDKEY_MOD_CTRL_RIGHT:     return ("Right " CTRL_TEXT);
        case KBDKEY_MOD_ALT:            return ALT_TEXT;
        case KBDKEY_MOD_ALT_LEFT:       return ("Left " ALT_TEXT);
        case KBDKEY_MOD_ALT_RIGHT:      return ("Right " ALT_TEXT);
        case KBDKEY_MOD_SHIFT:          return SHIFT_TEXT;
        case KBDKEY_MOD_SHIFT_LEFT:     return ("Left " SHIFT_TEXT);
        case KBDKEY_MOD_SHIFT_RIGHT:    return ("Right " SHIFT_TEXT);

        default:                        return "";
    }
}

KBDT_API enum keyboard_key kbdt_keyboard_key_from_str(const char* str)
{
    if (str == NULL || strlen(str) == 0)
        return (enum keyboard_key) 0;

    if (strlen(str) == 1 && IS_ALNUM(str[0]))
        return (enum keyboard_key) str[0];

    // Whitespace keys
    if (is_equal_str(str, "tab"))       return KBDKEY_TAB;
    if (is_equal_str(str, "space"))     return KBDKEY_SPACE;
    if (is_equal_str(str, "enter") || is_equal_str(str, "return"))
        return KBDKEY_ENTER;

    // Navigation keys
    if (is_equal_str(str, "home"))      return KBDKEY_HOME;
    if (is_equal_str(str, "end"))       return KBDKEY_END;
    if (is_equal_str(str, "page up") || is_equal_str(str, "pgup"))
        return KBDKEY_PAGE_UP;
    if (is_equal_str(str, "page down") || is_equal_str(str, "pgdn"))
        return KBDKEY_PAGE_DOWN;
    if (is_equal_str(str, "left") || is_equal_str(str, "arrow left"))
        return KBDKEY_LEFT;
    if (is_equal_str(str, "up") || is_equal_str(str, "arrow up"))
        return KBDKEY_UP;
    if (is_equal_str(str, "right") || is_equal_str(str, "arrow right"))
        return KBDKEY_RIGHT;
    if (is_equal_str(str, "down") || is_equal_str(str, "arrow down"))
        return KBDKEY_DOWN;

    // Editing keys
    if (is_equal_str(str, "backspace") || is_equal_str(str, "back"))
        return KBDKEY_BACKSPACE;
    if (is_equal_str(str, "insert") || is_equal_str(str, "ins"))
        return KBDKEY_INSERT;
    if (is_equal_str(str, "delete") || is_equal_str(str, "del"))
        return KBDKEY_DELETE;
    if (is_equal_str(str, "clear"))     return KBDKEY_CLEAR;

    // Function keys
    if (is_equal_str(str, "f1"))        return KBDKEY_F1;
    if (is_equal_str(str, "f2"))        return KBDKEY_F2;
    if (is_equal_str(str, "f3"))        return KBDKEY_F3;
    if (is_equal_str(str, "f4"))        return KBDKEY_F4;
    if (is_equal_str(str, "f5"))        return KBDKEY_F5;
    if (is_equal_str(str, "f6"))        return KBDKEY_F6;
    if (is_equal_str(str, "f7"))        return KBDKEY_F7;
    if (is_equal_str(str, "f8"))        return KBDKEY_F8;
    if (is_equal_str(str, "f9"))        return KBDKEY_F9;
    if (is_equal_str(str, "f10"))       return KBDKEY_F10;
    if (is_equal_str(str, "f11"))       return KBDKEY_F11;
    if (is_equal_str(str, "f12"))       return KBDKEY_F12;
    if (is_equal_str(str, "f13"))       return KBDKEY_F13;
    if (is_equal_str(str, "f14"))       return KBDKEY_F14;
    if (is_equal_str(str, "f15"))       return KBDKEY_F15;
    if (is_equal_str(str, "f16"))       return KBDKEY_F16;
    if (is_equal_str(str, "f17"))       return KBDKEY_F17;
    if (is_equal_str(str, "f18"))       return KBDKEY_F18;
    if (is_equal_str(str, "f19"))       return KBDKEY_F19;
    if (is_equal_str(str, "f20"))       return KBDKEY_F20;
    if (is_equal_str(str, "f21"))       return KBDKEY_F21;
    if (is_equal_str(str, "f22"))       return KBDKEY_F22;
    if (is_equal_str(str, "f23"))       return KBDKEY_F23;
    if (is_equal_str(str, "f24"))       return KBDKEY_F24;

    // Numpad number keys
    if (is_equal_str(str, "numpad 0") || is_equal_str(str, "keypad 0"))
        return KBDKEY_NUMPAD_0;
    if (is_equal_str(str, "numpad 1") || is_equal_str(str, "keypad 1"))
        return KBDKEY_NUMPAD_1;
    if (is_equal_str(str, "numpad 2") || is_equal_str(str, "keypad 2"))
        return KBDKEY_NUMPAD_2;
    if (is_equal_str(str, "numpad 3") || is_equal_str(str, "keypad 3"))
        return KBDKEY_NUMPAD_3;
    if (is_equal_str(str, "numpad 4") || is_equal_str(str, "keypad 4"))
        return KBDKEY_NUMPAD_4;
    if (is_equal_str(str, "numpad 5") || is_equal_str(str, "keypad 5"))
        return KBDKEY_NUMPAD_5;
    if (is_equal_str(str, "numpad 6") || is_equal_str(str, "keypad 6"))
        return KBDKEY_NUMPAD_6;
    if (is_equal_str(str, "numpad 7") || is_equal_str(str, "keypad 7"))
        return KBDKEY_NUMPAD_7;
    if (is_equal_str(str, "numpad 8") || is_equal_str(str, "keypad 8"))
        return KBDKEY_NUMPAD_8;
    if (is_equal_str(str, "numpad 9") || is_equal_str(str, "keypad 9"))
        return KBDKEY_NUMPAD_9;

    // Numpad operator keys
    if (is_equal_str(str, "numpad add") || is_equal_str(str, "keypad add"))
        return KBDKEY_NUMPAD_ADD;
    if (is_equal_str(str, "numpad subtract") || is_equal_str(str, "keypad subtract"))
        return KBDKEY_NUMPAD_SUBTRACT;
    if (is_equal_str(str, "numpad multiply") || is_equal_str(str, "keypad multiply"))
        return KBDKEY_NUMPAD_MULTIPLY;
    if (is_equal_str(str, "numpad divide") || is_equal_str(str, "keypad divide"))
        return KBDKEY_NUMPAD_DIVIDE;
    if (is_equal_str(str, "numpad decimal") || is_equal_str(str, "keypad decimal"))
        return KBDKEY_NUMPAD_DECIMAL;
    if (is_equal_str(str, "numpad separator") || is_equal_str(str, "keypad separator"))
        return KBDKEY_NUMPAD_SEPARATOR;
    if (is_equal_str(str, "numpad equal") || is_equal_str(str, "keypad equal"))
        return KBDKEY_NUMPAD_EQUAL;
    if (is_equal_str(str, "numpad enter") || is_equal_str(str, "keypad enter") ||
        is_equal_str(str, "numpad return") || is_equal_str(str, "keypad return"))
        return KBDKEY_NUMPAD_EQUAL;

    // Applications keys
    if (is_equal_str(str, "esc") || is_equal_str(str, "escape"))
        return KBDKEY_ESCAPE;
    if (is_equal_str(str, "pause"))     return KBDKEY_PAUSE;
    if (is_equal_str(str, "play"))      return KBDKEY_PLAY;
    if (is_equal_str(str, "help"))      return KBDKEY_HELP;
    if (is_equal_str(str, "menu"))      return KBDKEY_MENU;

    // Media keys
    if (is_equal_str(str, "media next") || is_equal_str(str, "media track next"))
        return KBDKEY_MEDIA_NEXT;
    if (is_equal_str(str, "media previous") || is_equal_str(str, "media prev") ||
        is_equal_str(str, "media track previous") || is_equal_str(str, "media track prev"))
        return KBDKEY_MEDIA_PREVIOUS;
    if (is_equal_str(str, "media play/pause") || is_equal_str(str, "media play pause"))
        return KBDKEY_MEDIA_PLAY_PAUSE;
    if (is_equal_str(str, "media stop"))
        return KBDKEY_MEDIA_STOP;

    // Volume keys
    if (is_equal_str(str, "volume up") || is_equal_str(str, "vol up"))
        return KBDKEY_VOLUME_UP;
    if (is_equal_str(str, "volume down") || is_equal_str(str, "vol down"))
        return KBDKEY_VOLUME_DOWN;
    if (is_equal_str(str, "volume mute") || is_equal_str(str, "vol mute"))
        return KBDKEY_VOLUME_MUTE;

    // Lock keys
    if (is_equal_str(str, "caps lock") || is_equal_str(str, "caps lk"))
        return KBDKEY_CAPS_LOCK;
    if (is_equal_str(str, "num lock") || is_equal_str(str, "num lk"))
        return KBDKEY_NUM_LOCK;
    if (is_equal_str(str, "scroll lock") || is_equal_str(str, "scroll lk"))
        return KBDKEY_SCROLL_LOCK;

    // Device keys
    if (is_equal_str(str, "print screen") || is_equal_str(str, "prtsc") || is_equal_str(str, "print"))
        return KBDKEY_PRINT_SCREEN;
    if (is_equal_str(str, "sleep"))
        return KBDKEY_SLEEP;

    // OEM (Original Equipment Manufacturer) keys
    // The specific values of the following keys may vary on different devices.
    // All of the following uses the standard keyboard layout of the United States.
    if (is_equal_str(str, "`"))     return KBDKEY_LEFT_QUOTE;
    if (is_equal_str(str, "-"))     return KBDKEY_MINUS;
    if (is_equal_str(str, "="))     return KBDKEY_EQUAL;
    if (is_equal_str(str, "["))     return KBDKEY_LEFT_BRACKET;
    if (is_equal_str(str, "]"))     return KBDKEY_RIGHT_BRACKET;
    if (is_equal_str(str, ";"))     return KBDKEY_SEMICOLON;
    if (is_equal_str(str, "'"))     return KBDKEY_APOSTROPHE;
    if (is_equal_str(str, ","))     return KBDKEY_COMMA;
    if (is_equal_str(str, "."))     return KBDKEY_PERIOD;
    if (is_equal_str(str, "/"))     return KBDKEY_SLASH;
    if (is_equal_str(str, "\\"))    return KBDKEY_BACKSLASH;
    if (is_equal_str(str, "<>"))    return KBDKEY_ANGLE_BRACKET;

    // Modifier keys
    if (IS_META(str, ""))           return KBDKEY_MOD_META;
    if (IS_META(str, "left"))       return KBDKEY_MOD_META_LEFT;
    if (IS_META(str, "right"))      return KBDKEY_MOD_META_RIGHT;
    if (IS_CTRL(str, ""))           return KBDKEY_MOD_CTRL;
    if (IS_CTRL(str, "left"))       return KBDKEY_MOD_CTRL_LEFT;
    if (IS_CTRL(str, "right"))      return KBDKEY_MOD_CTRL_RIGHT;
    if (IS_ALT(str, ""))            return KBDKEY_MOD_ALT;
    if (IS_ALT(str, "left"))        return KBDKEY_MOD_ALT_LEFT;
    if (IS_ALT(str, "right"))       return KBDKEY_MOD_ALT_RIGHT;
    if (IS_SHIFT(str, ""))          return KBDKEY_MOD_SHIFT;
    if (IS_SHIFT(str, "left"))      return KBDKEY_MOD_SHIFT_LEFT;
    if (IS_SHIFT(str, "right"))     return KBDKEY_MOD_SHIFT_RIGHT;

    return (enum keyboard_key) 0;
}
