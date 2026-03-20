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
    int stopWork() override;
    void work() override;
    int registerHotkeyImpl(const KeyCombination& kc, bool autoRepeat) override;
    int unregisterHotkeyImpl(const KeyCombination& kc) override;

private:
    // The following functions should only be called from within the worker thread.
    int nativeRegisterHotkey(WPARAM wParam, LPARAM lParam);
    int nativeUnregisterHotkey(WPARAM wParam, LPARAM lParam);
    void tryInvoke(WPARAM wParam, LPARAM lParam) const;

    DWORD workerThreadId_ = 0;
    int regUnregRc_ = -2;
    mutable std::mutex regUnregRcMtx_;
    std::condition_variable regUnregRcCv_;
    int maxHotkeyId_ = 0;
    std::vector<int> freeHotkeyIds_;
    std::unordered_map<int, KeyCombination> hotkeyIdToKc_;
    std::unordered_map<KeyCombination, int> kcToHotkeyId_;
};

} // namespace gbhk

#endif // !GLOBAL_HOTKEY_REGISTER_GHM_PRIVATE_WIN_HPP
