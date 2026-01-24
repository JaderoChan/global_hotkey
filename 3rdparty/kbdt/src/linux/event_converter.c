#include "event_converter.h"

void keyboard_event_to_input_event(struct keyboard_event* event, struct input_event* ie)
{
    ie->type = EV_KEY;
    ie->code = event->native_key;
    ie->value = (event->type == KBDET_PRESSED ? 1 : 0);
    ie->time.tv_sec = 0;
    ie->time.tv_usec = 0;
}

void keyboard_event_from_input_event(struct input_event* ie, struct keyboard_event* event)
{
    event->type = (ie->value == 0 ? KBDET_RELEASED : KBDET_PRESSED);
    event->native_key = ie->code;
}
