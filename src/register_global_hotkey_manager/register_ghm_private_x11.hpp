#ifndef GLOBAL_HOTKEY_REGISTER_GHM_PRIVATE_X11_HPP
#define GLOBAL_HOTKEY_REGISTER_GHM_PRIVATE_X11_HPP

#include "register_ghm_private.hpp"

#include <list>

#include <X11/Xlib.h>

namespace gbhk
{

class ErrorHandler
{
public:
    ErrorHandler();
    ~ErrorHandler();

    static int ec;

private:
    static int handleError(Display* display, XErrorEvent* error);
    static XErrorHandler prevXErrorHandler_;
};

enum EventType
{
    ET_EXIT = 1,
    ET_REGISTER,
    ET_UNREGISTER
};

class RegisterGHMPrivateX11 final : public RegisterGHMPrivate
{
public:
    RegisterGHMPrivateX11();
    ~RegisterGHMPrivateX11();

protected:
    int initialize() override;
    int stopWork() override;
    void work() override;
    int registerHotkeyImpl(const KeyCombination& kc, bool autoRepeat) override;
    int unregisterHotkeyImpl(const KeyCombination& kc) override;

private:
    static std::unordered_map<int, int> keycodeToKeysym_;

    // The following functions should only be called from within the worker thread.
    int nativeRegisterHotkey(Display* display);
    int nativeUnregisterHotkey(Display* display);
    void tryInvoke(const KeyCombination& prevKc, const KeyCombination& currKc) const;

    int regUnregRc_ = -2;
    KeyCombination regUnregKc_;
    mutable std::mutex regUnregRcKcMtx_;
    std::condition_variable regUnregRcCv_;

    int eventFd_ = -1;
    std::list<EventType> events_;
    mutable std::mutex eventsMtx;
};

} // namespace gbhk

#endif // !GLOBAL_HOTKEY_REGISTER_GHM_PRIVATE_X11_HPP
