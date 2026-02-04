#include <keyboard_tools_details.hpp>

#include <windows.h>

#include "event_converter.hpp"

namespace kbdt
{

namespace details
{

#define WM_SET_EVENT_HANDLER (WM_USER + 1)

static KeyEventHandler eventHandler = nullptr;
static DWORD workerThreadId = 0;

static LRESULT WINAPI LowLevelKeyboardProc(int nCode, WPARAM wParam, LPARAM lParam)
{
    if (nCode == HC_ACTION)
    {
        KBDLLHOOKSTRUCT* kbdllhs = (KBDLLHOOKSTRUCT*) lParam;
        if (!eventHandler)
            return CallNextHookEx(NULL, nCode, wParam, lParam);
        KeyEvent event;
        keyEventFromParam(event, wParam, lParam);
        if (!eventHandler(event))
            return true;
    }
    return CallNextHookEx(NULL, nCode, wParam, lParam);
}

int initialize()
{
    return KBDT_RC_SUCCESS;
}

int stopWork()
{
    if (PostThreadMessageA(workerThreadId, WM_DESTROY, 0, 0) != 0)
        return KBDT_RC_SUCCESS;
    return GetLastError();
}

int setEventHandler(KeyEventHandler handler)
{
    if (PostThreadMessageA(workerThreadId, WM_SET_EVENT_HANDLER, (WPARAM) handler, 0) != 0)
        return KBDT_RC_SUCCESS;
    return GetLastError();
}

void work()
{
    HHOOK kbdHook = SetWindowsHookExA(WH_KEYBOARD_LL, &LowLevelKeyboardProc, NULL, 0);
    if (!kbdHook)
    {
        setRunFail(GetLastError());
        return;
    }

    workerThreadId = GetCurrentThreadId();
    MSG msg;
    // Force the system to create the message queue.
    PeekMessageA(&msg, NULL, WM_USER, WM_USER, PM_NOREMOVE);
    // Indicate the worker thread is created successfully after create the message queue.
    // This can ensure that the PostThreadMessage() be performed only when the message queue exists.
    setRunSuccess();
    // Retrieves only messages on the current thread's message queue whose hwnd value is NULL.
    // In this case the thread message as posted by PostThreadMessage().
    while (GetMessageA(&msg, (HWND) -1, 0, 0) != 0)
    {
        if (msg.message == WM_DESTROY)
            PostQuitMessage(0);
        else if (msg.message == WM_SET_EVENT_HANDLER)
            eventHandler = (KeyEventHandler) msg.wParam;
    }

    UnhookWindowsHookEx(kbdHook);

    eventHandler = nullptr;
    workerThreadId = 0;
}

} // namespace details

} // namespace kbdt
