#ifndef GLOBAL_HOTKEY_HOOK_GLOBAL_HOTKEY_MANAGER_HPP
#define GLOBAL_HOTKEY_HOOK_GLOBAL_HOTKEY_MANAGER_HPP

#include "global_hotkey_manager.hpp"

namespace gbhk
{

class GLOBAL_HOTKEY_API HookGlobalHotkeyManager final : public GlobalHotkeyManager
{
public:
    static HookGlobalHotkeyManager& getInstance();

private:
    HookGlobalHotkeyManager();
    ~HookGlobalHotkeyManager();
};

} // namespace gbhk

#endif // !GLOBAL_HOTKEY_HOOK_GLOBAL_HOTKEY_MANAGER_HPP
