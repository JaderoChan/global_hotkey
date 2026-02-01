#ifndef GLOBAL_HOTKEY_REGISTER_GLOBAL_HOTKEY_MANAGER_HPP
#define GLOBAL_HOTKEY_REGISTER_GLOBAL_HOTKEY_MANAGER_HPP

#include "global_hotkey_manager.hpp"

namespace gbhk
{

class GLOBAL_HOTKEY_API RegisterGlobalHotkeyManager final : public GlobalHotkeyManager
{
public:
    static RegisterGlobalHotkeyManager& getInstance();

private:
    RegisterGlobalHotkeyManager();
    ~RegisterGlobalHotkeyManager();
};

} // namespace gbhk

#endif // !GLOBAL_HOTKEY_REGISTER_GLOBAL_HOTKEY_MANAGER_HPP
