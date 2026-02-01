#ifndef GLOBAL_HOTKEY_REGISTER_GHM_PRIVATE_MAC_HPP
#define GLOBAL_HOTKEY_REGISTER_GHM_PRIVATE_MAC_HPP

#include "register_ghm_private.hpp"

#include <Carbon/Carbon.h>
#include <CoreFoundation/CoreFoundation.h>

namespace gbhk
{

enum EventType
{
    ET_NONE,
    ET_REGISTER,
    ET_UNREGISTER
};

class RegisterGHMPrivateMac final : public RegisterGHMPrivate
{
public:
    RegisterGHMPrivateMac();
    ~RegisterGHMPrivateMac();

protected:
    int doBeforeThreadRun() override;
    int doBeforeThreadEnd() override;
    void work() override;
    int registerHotkey(const KeyCombination& kc, bool autoRepeat) override;
    int unregisterHotkey(const KeyCombination& kc) override;

private:
    static void runLoopSourceCallback(void* info);
    static OSStatus hotkeyEventHandler(EventHandlerCallRef handler, EventRef event, void* userData);

    static int nativeRegisterHotkey();
    static int nativeUnregisterHotkey();
    /// @todo
    void tryInvoke() const;

    static std::condition_variable cvRegUnregRc_;
    static std::atomic<int> regUnregRc_;
    static std::atomic<EventType> eventType_;
    static std::atomic<KeyCombination> regUnregKc_;
    static std::unordered_map<KeyCombination, EventHotKeyRef> kcToHotkeyRef_;

    CFRunLoopSourceContext sourceContext_ = {0};
    CFRunLoopSourceRef source_ = NULL;
    CFRunLoopRef runLoop_ = NULL;
};

} // namespace gbhk

#endif // !GLOBAL_HOTKEY_REGISTER_GHM_PRIVATE_MAC_HPP
