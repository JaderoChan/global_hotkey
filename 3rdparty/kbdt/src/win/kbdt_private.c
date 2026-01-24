#include <kbdt_private.h>

#include <stdlib.h>     // malloc, free

#include <windows.h>

#include "event_converter.h"
#include "message_id.h"

#define WM_SET_HANDLER (WM_USER + 1)

static keyboard_event_handler g_handler = NULL;
static DWORD g_worker_thread_id;

static LRESULT WINAPI LowLevelKeyboardProc(int nCode, WPARAM wParam, LPARAM lParam)
{
    if (nCode == HC_ACTION)
    {
        KBDLLHOOKSTRUCT* kbdllhs = (KBDLLHOOKSTRUCT*) lParam;
        if (!g_handler || kbdllhs->dwExtraInfo == message_id())
            return CallNextHookEx(NULL, nCode, wParam, lParam);
        struct keyboard_event event;
        keyboard_event_from_param(wParam, lParam, &event);
        g_handler(&event);
    }
    return CallNextHookEx(NULL, nCode, wParam, lParam);
}

int do_before_start_work()
{
    return KBDT_RC_SUCCESS;
}

int do_before_end_work()
{
    if (PostThreadMessageA(g_worker_thread_id, WM_DESTROY, 0, 0) != 0)
        return KBDT_RC_SUCCESS;
    return GetLastError();
}

void work()
{
    HHOOK kbd_hook = SetWindowsHookExA(WH_KEYBOARD_LL, &LowLevelKeyboardProc, NULL, 0);
    if (!kbd_hook)
    {
        set_run_fail(GetLastError());
        return;
    }

    g_worker_thread_id = GetCurrentThreadId();
    MSG msg;
    // Force the system to create the message queue.
    PeekMessageA(&msg, NULL, WM_USER, WM_USER, PM_NOREMOVE);
    // Indicate the worker thread is created successfully after create the message queue.
    // This can ensure that the PostThreadMessage() be performed only when the message queue exists.
    set_run_success();
    // Retrieves only messages on the current thread's message queue whose hwnd value is NULL.
    // In this case the thread message as posted by PostThreadMessage().
    while (GetMessageA(&msg, (HWND) -1, 0, 0) != 0)
    {
        if (msg.message == WM_DESTROY)
            PostQuitMessage(0);
        else if (msg.message == WM_SET_HANDLER)
            g_handler = (keyboard_event_handler) msg.wParam;
    }

    UnhookWindowsHookEx(kbd_hook);
}

int set_event_handler_p(keyboard_event_handler handler)
{
    if (PostThreadMessageA(g_worker_thread_id, WM_SET_HANDLER, (WPARAM) handler, 0) != 0)
        return KBDT_RC_SUCCESS;
    return GetLastError();
}

size_t send_events_p(struct keyboard_event* events, size_t event_count)
{
    INPUT* inputs = malloc(event_count * sizeof(INPUT));
    if (!inputs)
        return 0;
    for (size_t i = 0; i < event_count; ++i)
        keyboard_event_to_input(&events[i], &inputs[i]);
    UINT sent = SendInput(event_count, inputs, sizeof(INPUT));
    free(inputs);
    return sent;
}
