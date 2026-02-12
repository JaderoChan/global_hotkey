#include "event_converter.hpp"

#include <Carbon/Carbon.h>

namespace kbdt
{

namespace details
{

#define MOD_FIRST   kVK_RightCommand
#define MOD_LAST    kVK_Function
#define MOD_COUNT   (MOD_LAST - MOD_FIRST + 1)

static bool modStates[MOD_COUNT] = {0};
#define GMSINDEX(keyCode)   ((keyCode) - MOD_FIRST)
#define GMS(keyCode)        (modStates[GMSINDEX(keyCode)])

void keyEventToCGEvent(const KeyEvent& event, CGEventRef& cgEvent)
{
    CGKeyCode keyCode = (CGKeyCode) event.nativeKey;
    bool state = false;
    switch (event.type)
    {
        case KET_PRESSED:
            state = true;
            break;
        case KET_RELEASED:
            state = false;
            break;
        default:
            break;
    }
    cgEvent = CGEventCreateKeyboardEvent(NULL, keyCode, state);
}

void keyEventFromCGEvent(KeyEvent& event, CGEventType cgEventType, CGEventRef cgEvent)
{
    event.nativeKey = (uint32_t) CGEventGetIntegerValueField(cgEvent, kCGKeyboardEventKeycode);
    switch (cgEventType)
    {
        case kCGEventKeyDown:
            event.type = KET_PRESSED;
            break;
        case kCGEventKeyUp:
            event.type = KET_RELEASED;
            break;
        case kCGEventFlagsChanged:
        {
            bool& modState = GMS(event.nativeKey);
            event.type = modState ? KET_RELEASED : KET_PRESSED;
            modState = !modState;
            break;
        }
        default:
            break;
    }
}

} // namespace details

} // namespace kbdt
