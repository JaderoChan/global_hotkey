#ifndef GLOBAL_HOTKEY_REGISTER_GHM_PRIVATE_X11_HPP
#define GLOBAL_HOTKEY_REGISTER_GHM_PRIVATE_X11_HPP

#ifndef GLOBAL_HOTKEY_DISABLE_REGISTER

#include "register_ghm_private.hpp"

#ifdef GLOBAL_HOTKEY_LINUX

#include <X11/Xlib.h>

namespace gbhk
{

class RegisterGHMPrivateX11 final : public RegisterGHMPrivate
{
public:
    RegisterGHMPrivateX11();
    ~RegisterGHMPrivateX11();

protected:
    int doBeforeThreadRun() override;
    int doBeforeThreadEnd() override;
    void work() override;
    int registerHotkey(const KeyCombination& kc, bool autoRepeat) override;
    int unregisterHotkey(const KeyCombination& kc) override;

private:
    // The following functions should only be called from within the worker thread.
    int nativeRegisterHotkey(Display* display);
    int nativeUnregisterHotkey(Display* display);
    void invoke_(const KeyCombination& prevKc, const KeyCombination& currKc) const;

    std::condition_variable cvRegUnregRc_;
    std::atomic<int> regUnregRc_;
    std::atomic<KeyCombination> regUnregKc_;
    int eventFd_ = 0;
};

} // namespace gbhk

#endif // GLOBAL_HOTKEY_LINUX

#endif // !GLOBAL_HOTKEY_DISABLE_REGISTER

#endif // !GLOBAL_HOTKEY_REGISTER_GHM_PRIVATE_X11_HPP
