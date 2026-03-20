#ifndef GLOBAL_HOTKEY_REGISTER_GHM_PRIVATE_MAC_HPP
#define GLOBAL_HOTKEY_REGISTER_GHM_PRIVATE_MAC_HPP

#include "register_ghm_private.hpp"

#include <Carbon/Carbon.h>
#include <CoreFoundation/CoreFoundation.h>

namespace gbhk
{

enum EventType : uint8_t
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
    int initialize() override;
    int stopWork() override;
    void work() override;
    int registerHotkeyImpl(const KeyCombination& kc, bool autoRepeat) override;
    int unregisterHotkeyImpl(const KeyCombination& kc) override;

private:
    static void runLoopSourceCallback(void* info);
    static OSStatus hotkeyEventHandler(EventHandlerCallRef nextHandler, EventRef event, void* userData);

    static int nativeRegisterHotkey();
    static int nativeUnregisterHotkey();
    static void tryInvoke(const KeyCombination& prevKc, const KeyCombination& currKc);

    static int regUnregRc_;
    static KeyCombination regUnregKc_;
    static EventType eventType_;
    static std::mutex regUnregRcKcTypeMtx_;
    static std::condition_variable regUnregRcCv_;
    static std::unordered_map<KeyCombination, EventHotKeyRef> kcToHotkeyRef_;

    static KeyCombination prevKc_;
    static KeyCombination currKc_;

    CFRunLoopSourceContext sourceContext_ = {0};
    CFRunLoopSourceRef source_ = nullptr;
    CFRunLoopRef runLoop_ = nullptr;
};

} // namespace gbhk

#endif // !GLOBAL_HOTKEY_REGISTER_GHM_PRIVATE_MAC_HPP
