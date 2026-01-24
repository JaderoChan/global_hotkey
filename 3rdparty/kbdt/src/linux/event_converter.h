#ifndef KBDT_LINUX_EVENT_CONVERTER_H
#define KBDT_LINUX_EVENT_CONVERTER_H

#include <linux/input.h>

#include <kbdt/kbdt.h>

void keyboard_event_to_input_event(struct keyboard_event* event, struct input_event* ie);

void keyboard_event_from_input_event(struct input_event* ie, struct keyboard_event* event);

#endif // !KBDT_LINUX_EVENT_CONVERTER_H
