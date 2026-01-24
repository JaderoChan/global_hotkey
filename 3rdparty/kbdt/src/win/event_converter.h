#ifndef KBDT_WIN_EVENT_CONVERTER_H
#define KBDT_WIN_EVENT_CONVERTER_H

#include <windows.h>

#include <kbdt/kbdt.h>

void keyboard_event_to_input(struct keyboard_event* event, INPUT* input);

void keyboard_event_from_param(WPARAM wParam, LPARAM lParam, struct keyboard_event* event);

#endif // !KBDT_WIN_EVENT_CONVERTER_H
