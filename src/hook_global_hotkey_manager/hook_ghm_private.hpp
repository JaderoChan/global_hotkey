#ifndef GLOBAL_HOTKEY_HOOK_GHM_PRIVATE_HPP
#define GLOBAL_HOTKEY_HOOK_GHM_PRIVATE_HPP

#ifndef GLOBAL_HOTKEY_DISABLE_HOOK

#include "../global_hotkey_manager/ghm_private.hpp"

#include <kbdt/kbdt.h>

namespace gbhk
{

class HookGHMPrivate final : public GHMPrivate
{
public:
    HookGHMPrivate();
    ~HookGHMPrivate();

protected:
    int doBeforeThreadRun() override;
    int doBeforeThreadEnd() override;
    void work() override;
    int registerHotkey(const KeyCombination& kc, bool autoRepeat) override;
    int unregisterHotkey(const KeyCombination& kc) override;

private:
    void invoke_(const KeyCombination& prevKc, const KeyCombination& currKc) const;
};

} // namespace gbhk

#endif // !GLOBAL_HOTKEY_DISABLE_HOOK

#endif // !GLOBAL_HOTKEY_HOOK_GHM_PRIVATE_HPP
