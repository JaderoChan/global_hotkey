#ifndef GLOBAL_HOTKEY_REGISTER_GHM_PRIVATE_HPP
#define GLOBAL_HOTKEY_REGISTER_GHM_PRIVATE_HPP

#include <global_hotkey_manager/ghm_private.hpp>

namespace gbhk
{

class RegisterGHMPrivate : public GHMPrivate
{
public:
    static RegisterGHMPrivate& getInstance();

protected:
    RegisterGHMPrivate();
    ~RegisterGHMPrivate();
};

} // namespace gbhk

#endif // !GLOBAL_HOTKEY_REGISTER_GHM_PRIVATE_HPP
