#include "event_converter.h"

#include <Carbon/Carbon.h>

#define MOD_FIRST   kVK_RightCommand
#define MOD_LAST    kVK_Function
#define MOD_COUNT   (MOD_LAST - MOD_FIRST + 1)

static bool g_mod_states[MOD_COUNT] = {0};
#define GMSINDEX(keycode)   ((keycode) - MOD_FIRST)
#define GMS(keycode)        (&g_mod_states[GMSINDEX(keycode)])

void keyboard_event_to_cg_event_ref(struct keyboard_event* event, CGEventRef* cg_event_ref)
{
    CGKeyCode keycode = (CGKeyCode) event->native_key;
    bool state = false;
    switch (event->type)
    {
        case KBDET_PRESSED:
            state = true;
            break;
        case KBDET_RELEASED:
            state = false;
            break;
        default:
            break;
    }
    *cg_event_ref = CGEventCreateKeyboardEvent(NULL, keycode, state);
}

void keyboard_event_from_cg_event_type_ref(
    CGEventType cg_event_type,
    CGEventRef* cg_event_ref,
    struct keyboard_event* event)
{
    event->native_key = (int) CGEventGetIntegerValueField(*cg_event_ref, kCGKeyboardEventKeycode);
    switch (cg_event_type)
    {
        case kCGEventKeyDown:
            event->type = KBDET_PRESSED;
            break;
        case kCGEventKeyUp:
            event->type = KBDET_RELEASED;
            break;
        case kCGEventFlagsChanged:
        {
            bool* mod_state = GMS(event->native_key);
            event->type = *mod_state ? KBDET_RELEASED : KBDET_PRESSED;
            *mod_state = !(*mod_state);
            break;
        }
        default:
            break;
    }
}
