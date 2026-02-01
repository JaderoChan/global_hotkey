#ifndef GLOBAL_HOTKEY_REGISTER_GHM_PRIVATE_WIN_HPP
#define GLOBAL_HOTKEY_REGISTER_GHM_PRIVATE_WIN_HPP

#include "register_ghm_private.hpp"

#include <vector>

#include <windows.h>

namespace gbhk
{

class RegisterGHMPrivateWin final : public RegisterGHMPrivate
{
public:
    RegisterGHMPrivateWin();
    ~RegisterGHMPrivateWin();

protected:
    int doBeforeThreadEnd() override;
    void work() override;
    int registerHotkey(const KeyCombination& kc, bool autoRepeat) override;
    int unregisterHotkey(const KeyCombination& kc) override;

private:
    // The following functions should only be called from within the worker thread.
    int nativeRegisterHotkey(WPARAM wParam, LPARAM lParam);
    int nativeUnregisterHotkey(WPARAM wParam, LPARAM lParam);
    void tryInvoke(WPARAM wParam, LPARAM lParam) const;

    DWORD workerThreadId_ = 0;
    std::condition_variable cvRegUnregRc_;
    std::atomic<int> regUnregRc_;
    std::atomic<int> maxHotkeyId_;
    std::vector<int> freeHotkeyIds_;
    std::unordered_map<int, KeyCombination> hotkeyIdToKc_;
    std::unordered_map<KeyCombination, int> kcToHotkeyId_;
};

} // namespace gbhk

#endif // !GLOBAL_HOTKEY_REGISTER_GHM_PRIVATE_WIN_HPP
