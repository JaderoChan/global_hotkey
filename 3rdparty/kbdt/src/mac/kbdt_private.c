#include <kbdt_private.h>

#include <stdint.h>     // uint32, uint64_t
#include <stdlib.h>     // malloc, free

#include <pthread.h>

#include <Carbon/Carbon.h>
#include <CoreFoundation/CoreFoundation.h>
#include <CoreGraphics/CGEvent.h>

#include "event_converter.h"

// Return code for failed operation.
#define KBDT_RC_FAIL       -1

static pthread_mutex_t g_mtx = PTHREAD_MUTEX_INITIALIZER;
#define LOCK(mtx)   pthread_mutex_lock(mtx)
#define UNLOCK(mtx) pthread_mutex_unlock(mtx)

static keyboard_event_handler g_handler = NULL;

static CFRunLoopRef g_run_loop;

static CGEventRef
keyboard_tap_callback(CGEventTapProxy proxy, CGEventType type, CGEventRef event, void* data);

int do_before_start_work()
{
    return KBDT_RC_SUCCESS;
}

int do_before_end_work()
{
    CFRunLoopStop(g_run_loop);
    return KBDT_RC_SUCCESS;
}

void work()
{
    g_run_loop = CFRunLoopGetCurrent();
    if (!g_run_loop)
    {
        set_run_fail(KBDT_RC_FAIL);
        return;
    }

    CGEventMask event_mask =
        CGEventMaskBit(kCGEventKeyDown) |
        CGEventMaskBit(kCGEventKeyUp) |
        CGEventMaskBit(kCGEventFlagsChanged);
    CFMachPortRef event_tap = CGEventTapCreate(
        kCGHIDEventTap,
        kCGHeadInsertEventTap,
        kCGEventTapOptionDefault,
        event_mask,
        &keyboard_tap_callback,
        NULL);
    if (!event_tap)
    {
        set_run_fail(KBDT_RC_FAIL);
        return;
    }

    CFRunLoopSourceRef run_loop_source =
        CFMachPortCreateRunLoopSource(kCFAllocatorDefault, event_tap, 0);
    if (!run_loop_source)
    {
        CFRelease(event_tap);
        set_run_fail(KBDT_RC_FAIL);
        return;
    }
    CFRunLoopAddSource(g_run_loop, run_loop_source, kCFRunLoopDefaultMode);
    CGEventTapEnable(event_tap, true);

    set_run_success();
    CFRunLoopRun();

    CFRelease(event_tap);
    CFRelease(run_loop_source);
}

int set_event_handler_p(keyboard_event_handler handler)
{
    LOCK(&g_mtx);
    g_handler = handler;
    UNLOCK(&g_mtx);
    return KBDT_RC_SUCCESS;
}

size_t send_events_p(struct keyboard_event* events, size_t event_count)
{
    size_t sent = 0;
    CGEventRef cger = NULL;
    for (size_t i = 0; i < event_count; ++i)
    {
        keyboard_event_to_cg_event_ref(events + i, &cger);
        if (cger)
        {
            CGEventPost(kCGHIDEventTap, cger);
            CFRelease(cger);
            cger = NULL;
            sent++;
        }
    }
    return sent;
}

CGEventRef keyboard_tap_callback(CGEventTapProxy proxy, CGEventType type, CGEventRef event, void* data)
{
    struct keyboard_event kbd_event;
    keyboard_event_from_cg_event_type_ref(type, &event, &kbd_event);
    LOCK(&g_mtx);
    if (g_handler)
        g_handler(&kbd_event);
    UNLOCK(&g_mtx);
    return event;
}
