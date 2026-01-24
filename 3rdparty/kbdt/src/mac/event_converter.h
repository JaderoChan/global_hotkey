#ifndef KBDT_MAC_EVENT_CONVERTER_H
#define KBDT_MAC_EVENT_CONVERTER_H

#include <CoreGraphics/CGEvent.h>

#include <kbdt/kbdt.h>

void keyboard_event_to_cg_event_ref(struct keyboard_event* event, CGEventRef* cg_event_ref);

void keyboard_event_from_cg_event_type_ref(
    CGEventType cg_event_type,
    CGEventRef* cg_event_ref,
    struct keyboard_event* event);

#endif // !KBDT_MAC_EVENT_CONVERTER_H
