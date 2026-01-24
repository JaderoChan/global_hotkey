#include "event_converter.h"

#include "message_id.h"

void keyboard_event_to_input(struct keyboard_event* event, INPUT* input)
{
    input->type = INPUT_KEYBOARD;
    input->ki.wVk = (WORD) event->native_key;
    input->ki.dwExtraInfo = (ULONG_PTR) message_id();
    switch (event->type)
    {
        case KBDET_RELEASED:
            input->ki.dwFlags = KEYEVENTF_KEYUP;
            break;
        case KBDET_PRESSED:
            input->ki.dwFlags = 0;
            break;
        default:
            break;
    }
}

void keyboard_event_from_param(WPARAM wParam, LPARAM lParam, struct keyboard_event* event)
{
    KBDLLHOOKSTRUCT* kbdllhs = (KBDLLHOOKSTRUCT*) lParam;
    switch (wParam)
    {
        case WM_KEYDOWN:    // Fallthrough
        case WM_SYSKEYDOWN:
            event->type = KBDET_PRESSED;
            break;
        case WM_KEYUP:      // Fallthrough
        case WM_SYSKEYUP:
            event->type = KBDET_RELEASED;
            break;
        default:
            break;
    }
    event->native_key = kbdllhs->vkCode;
}
