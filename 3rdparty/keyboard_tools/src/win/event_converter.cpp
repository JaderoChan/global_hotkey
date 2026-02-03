#include "event_converter.hpp"

namespace kbdt
{

namespace details
{

void keyEventToInput(const KeyEvent& event, INPUT& input)
{
    input.type = INPUT_KEYBOARD;
    input.ki.wVk = (WORD) event.nativeKey;
    switch (event.type)
    {
        case KET_RELEASED:
            input.ki.dwFlags = KEYEVENTF_KEYUP;
            break;
        case KET_PRESSED:
            input.ki.dwFlags = 0;
            break;
        default:
            break;
    }
}

void keyEventFromParam(KeyEvent& event, WPARAM wParam, LPARAM lParam)
{
    switch (wParam)
    {
        case WM_KEYDOWN:    // Fallthrough
        case WM_SYSKEYDOWN:
            event.type = KET_PRESSED;
            break;
        case WM_KEYUP:      // Fallthrough
        case WM_SYSKEYUP:
            event.type = KET_RELEASED;
            break;
        default:
            break;
    }
    KBDLLHOOKSTRUCT* kbdllhs = (KBDLLHOOKSTRUCT*) lParam;
    event.nativeKey = kbdllhs->vkCode;
}

} // namespace details

} // namespace kbdt
