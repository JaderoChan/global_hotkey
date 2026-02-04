#include <keyboard_tools_details.hpp>

#include <atomic>

#include <Carbon/Carbon.h>
#include <CoreFoundation/CoreFoundation.h>
#include <CoreGraphics/CGEvent.h>

#include "event_converter.hpp"

namespace kbdt
{

namespace details
{

// Return code for failed operation.
#define KBDT_RC_FAIL       -1

std::atomic<KeyEventHandler> eventHandler{nullptr};
static CFRunLoopRef runLoop = NULL;

static CGEventRef keyboardTapCallback(CGEventTapProxy proxy, CGEventType type, CGEventRef event, void* data)
{
    KeyEvent keyEvent;
    keyEventFromCGEvent(keyEvent, type, event);
    if (eventHandler.load())
    {
        if (!eventHandler.load()(keyEvent))
            return NULL;
    }
    return event;
}

int initialize()
{
    return KBDT_RC_SUCCESS;
}

int stopWork()
{
    CFRunLoopStop(runLoop);
    return KBDT_RC_SUCCESS;
}

int setEventHandler(KeyEventHandler handler)
{
    eventHandler = handler;
    return KBDT_RC_SUCCESS;
}

void work()
{
    runLoop = CFRunLoopGetCurrent();
    if (!runLoop)
    {
        setRunFail(KBDT_RC_FAIL);
        return;
    }

    CGEventMask eventMask =
        CGEventMaskBit(kCGEventKeyDown) |
        CGEventMaskBit(kCGEventKeyUp) |
        CGEventMaskBit(kCGEventFlagsChanged);
    CFMachPortRef eventTap = CGEventTapCreate(
        kCGHIDEventTap,
        kCGHeadInsertEventTap,
        kCGEventTapOptionDefault,
        eventMask,
        &keyboardTapCallback,
        NULL);
    if (!eventTap)
    {
        setRunFail(KBDT_RC_FAIL);
        return;
    }

    CFRunLoopSourceRef runLoopSource =
        CFMachPortCreateRunLoopSource(kCFAllocatorDefault, eventTap, 0);
    if (!runLoopSource)
    {
        CFRelease(eventTap);
        setRunFail(KBDT_RC_FAIL);
        return;
    }
    CFRunLoopAddSource(runLoop, runLoopSource, kCFRunLoopDefaultMode);
    CGEventTapEnable(eventTap, true);

    setRunSuccess();
    CFRunLoopRun();

    CFRelease(eventTap);
    CFRelease(runLoopSource);

    eventHandler = nullptr;
    runLoop = NULL;
}

} // namespace details

} // namespace kbdt
