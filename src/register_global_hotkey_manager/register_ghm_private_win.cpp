#include "register_ghm_private_win.hpp"

#include <global_hotkey/return_code.hpp>
#include <key/key_private.hpp>

namespace gbhk
{

#define WM_REGISTER_HOTKEY      (WM_USER + 1)
#define WM_UNREGISTER_HOTKEY    (WM_USER + 2)

RegisterGHMPrivateWin::RegisterGHMPrivateWin() :
    regUnregRc_(0),
    maxHotkeyId_(0)
{}

RegisterGHMPrivateWin::~RegisterGHMPrivateWin() { uninitialize(); }

int RegisterGHMPrivateWin::doBeforeThreadEnd()
{
    if (PostThreadMessageA(workerThreadId_, WM_DESTROY, 0, 0) != 0)
        return RC_SUCCESS;
    return (int) GetLastError();
}

void RegisterGHMPrivateWin::work()
{
    workerThreadId_ = GetCurrentThreadId();
    MSG msg = {0};
    // Force the system to create the message queue.
    PeekMessageA(&msg, NULL, WM_USER, WM_USER, PM_NOREMOVE);
    // Indicate the worker thread is created successfully after create the message queue.
    // This can ensure that the `PostThreadMessage` be performed only when the message queue exists.
    setRunSuccess();
    // Retrieves only messages on the current thread's message queue whose hwnd value is NULL.
    // In this case the thread message as posted by `PostThreadMessage`.
    while (GetMessageA(&msg, (HWND) -1, 0, 0) != 0)
    {
        if (msg.message == WM_HOTKEY)
        {
            tryInvoke(msg.wParam, msg.lParam);
        }
        else if (msg.message == WM_REGISTER_HOTKEY)
        {
            regUnregRc_ = nativeRegisterHotkey(msg.wParam, msg.lParam);
            cvRegUnregRc_.notify_one();
        }
        else if (msg.message == WM_UNREGISTER_HOTKEY)
        {
            regUnregRc_ = nativeUnregisterHotkey(msg.wParam, msg.lParam);
            cvRegUnregRc_.notify_one();
        }
        else if (msg.message == WM_DESTROY)
        {
            PostQuitMessage(0);
        }
    }

    workerThreadId_ = 0;
    regUnregRc_ = 0;
    maxHotkeyId_ = 0;
    freeHotkeyIds_.clear();
    hotkeyIdToKc_.clear();
    kcToHotkeyId_.clear();
}

int RegisterGHMPrivateWin::registerHotkey(const KeyCombination& kc, bool autoRepeat)
{
    // wParam store the value of native modifiers.
    WPARAM wParam = modifiersToNativeModifiers(kc.modifiers());
    wParam |= (!autoRepeat ? MOD_NOREPEAT : 0);
    // lParam store the value of native key code.
    LPARAM lParam = keyToNativeKey(kc.key());

    regUnregRc_ = -1;
    if (PostThreadMessageA(workerThreadId_, WM_REGISTER_HOTKEY, wParam, lParam) != 0)
    {
        std::mutex dummyMtx;
        std::unique_lock<std::mutex> lock(dummyMtx);
        cvRegUnregRc_.wait(lock, [this]() { return regUnregRc_ != -1; });
        return regUnregRc_;
    }
    return (int) GetLastError();
}

int RegisterGHMPrivateWin::unregisterHotkey(const KeyCombination& kc)
{
    // wParam store the value of native modifiers.
    WPARAM wParam = modifiersToNativeModifiers(kc.modifiers());
    // lParam store the value of native key code.
    LPARAM lParam = keyToNativeKey(kc.key());

    regUnregRc_ = -1;
    if (PostThreadMessageA(workerThreadId_, WM_UNREGISTER_HOTKEY, wParam, lParam) != 0)
    {
        std::mutex dummyMtx;
        std::unique_lock<std::mutex> lock(dummyMtx);
        cvRegUnregRc_.wait(lock, [this]() { return regUnregRc_ != -1; });
        return regUnregRc_;
    }
    return (int) GetLastError();
}

int RegisterGHMPrivateWin::nativeRegisterHotkey(WPARAM wParam, LPARAM lParam)
{
    Modifiers mod = modifiersFromNativeModifiers(wParam);
    Key key = keyFromNativeKey(lParam);
    KeyCombination kc(mod, key);

    int hotkeyId = (freeHotkeyIds_.empty() ? maxHotkeyId_.load() : freeHotkeyIds_.back());
    if (RegisterHotKey(NULL, hotkeyId, wParam, lParam) != 0)
    {
        hotkeyIdToKc_[hotkeyId] = kc;
        kcToHotkeyId_[kc] = hotkeyId;
        if (freeHotkeyIds_.empty())
            maxHotkeyId_++;
        else
            freeHotkeyIds_.pop_back();
        return RC_SUCCESS;
    }
    return (int) GetLastError();
}

int RegisterGHMPrivateWin::nativeUnregisterHotkey(WPARAM wParam, LPARAM lParam)
{
    Modifiers mod = modifiersFromNativeModifiers(wParam);
    Key key = keyFromNativeKey(lParam);
    KeyCombination kc(mod, key);

    int hotkeyId = kcToHotkeyId_[kc];
    if (UnregisterHotKey(NULL, hotkeyId) != 0)
    {
        hotkeyIdToKc_.erase(hotkeyId);
        kcToHotkeyId_.erase(kc);
        freeHotkeyIds_.push_back(hotkeyId);
        return RC_SUCCESS;
    }
    return (int) GetLastError();
}

void RegisterGHMPrivateWin::tryInvoke(WPARAM wParam, LPARAM lParam) const
{
    int hotkeyId = (int) wParam;
    if (hotkeyIdToKc_.find(hotkeyId) != hotkeyIdToKc_.end())
    {
        auto& kc = hotkeyIdToKc_.at(hotkeyId);
        std::function<void()> fn = getPairValue(kc).second;
        if (fn)
            fn();
    }
}

} // namespace gbhk
