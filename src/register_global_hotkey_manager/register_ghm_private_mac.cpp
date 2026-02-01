#include "register_ghm_private_mac.hpp"

#include <global_hotkey/return_code.hpp>
#include <key/key_private.hpp>

namespace gbhk
{

std::condition_variable RegisterGHMPrivateMac::cvRegUnregRc_;
std::atomic<int> RegisterGHMPrivateMac::regUnregRc_{0};
std::atomic<EventType> RegisterGHMPrivateMac::eventType_{ET_NONE};
std::atomic<KeyCombination> RegisterGHMPrivateMac::regUnregKc_;
std::unordered_map<KeyCombination, EventHotKeyRef> RegisterGHMPrivateMac::kcToHotkeyRef_;

RegisterGHMPrivateMac::RegisterGHMPrivateMac() = default;

RegisterGHMPrivateMac::~RegisterGHMPrivateMac() { uninitialize(); }

int RegisterGHMPrivateMac::doBeforeThreadRun()
{
    sourceContext_ = {
        .version = 0,
        .info = &source_,
        .retain = NULL,
        .release = NULL,
        .copyDescription = NULL,
        .equal = NULL,
        .hash = NULL,
        .schedule = NULL,
        .cancel = NULL,
        .perform = &RegisterGHMPrivateMac::runLoopSourceCallback
    };
    return RC_SUCCESS;
}

int RegisterGHMPrivateMac::doBeforeThreadEnd()
{
    CFRunLoopStop(runLoop_);
    return RC_SUCCESS;
}

void RegisterGHMPrivateMac::work()
{
    runLoop_ = CFRunLoopGetCurrent();
    if (runLoop_ == NULL)
    {
        // TODO: Need error code.
        setRunFail(-1);
        return;
    }

    EventTypeSpec eventType_Specs[2] = {0};
    eventType_Specs[0].eventClass = kEventClassKeyboard;
    eventType_Specs[0].eventKind = kEventHotKeyPressed;
    eventType_Specs[1].eventClass = kEventClassKeyboard;
    eventType_Specs[1].eventKind = kEventHotKeyReleased;
    auto status = InstallApplicationEventHandler(
        &RegisterGHMPrivateMac::hotkeyEventHandler,
        2, eventType_Specs,
        NULL, NULL
    );
    if (status != noErr)
    {
        setRunFail(status);
        return;
    }

    source_ = CFRunLoopSourceCreate(NULL, 0, &sourceContext_);
    if (source_ == NULL)
    {
        // TODO: Need error code.
        setRunFail(-1);
        return;
    }
    CFRunLoopAddSource(runLoop_, source_, kCFRunLoopDefaultMode);

    setRunSuccess();
    CFRunLoopRun();

    CFRelease(source_);
}

int RegisterGHMPrivateMac::registerHotkey(const KeyCombination& kc, bool autoRepeat)
{
    regUnregRc_ = -1;
    eventType_ = ET_REGISTER;
    regUnregKc_ = kc;
    CFRunLoopSourceSignal(source_);
    CFRunLoopWakeUp(runLoop_);

    std::mutex dummyMtx;
    std::unique_lock<std::mutex> lock(dummyMtx);
    cvRegUnregRc_.wait(lock, [this]() { return regUnregRc_ != -1; });
    return regUnregRc_;
}

int RegisterGHMPrivateMac::unregisterHotkey(const KeyCombination& kc)
{
    regUnregRc_ = -1;
    eventType_ = ET_UNREGISTER;
    regUnregKc_ = kc;
    CFRunLoopSourceSignal(source_);
    CFRunLoopWakeUp(runLoop_);

    std::mutex dummyMtx;
    std::unique_lock<std::mutex> lock(dummyMtx);
    cvRegUnregRc_.wait(lock, [this]() { return regUnregRc_ != -1; });
    return regUnregRc_;
}

void RegisterGHMPrivateMac::runLoopSourceCallback(void* info)
{
    CFRunLoopSourceRef* sref = (CFRunLoopSourceRef*) info;
    switch (eventType_.load())
    {
        case ET_REGISTER:
            regUnregRc_ = nativeRegisterHotkey();
            break;
        case ET_UNREGISTER:
            regUnregRc_ = nativeUnregisterHotkey();
            break;
        default:
            regUnregRc_ = RC_SUCCESS;
            break;
    }
    cvRegUnregRc_.notify_one();
}

OSStatus RegisterGHMPrivateMac::hotkeyEventHandler(EventHandlerCallRef handler, EventRef event, void* userData)
{
    if (GetEventClass(event) == kEventClassKeyboard)
    {
        auto eventKind = GetEventKind(event);
        if (eventKind != kEventHotKeyPressed && eventKind != kEventHotKeyReleased)
            return noErr;

        EventHotKeyID hotkeyId;
        auto status = GetEventParameter(
            event,
            kEventParamDirectObject,
            typeEventHotKeyID,
            NULL,
            sizeof(hotkeyId),
            NULL,
            &hotkeyId
        );
        if (status != noErr)
            return status;

        auto nativeModifiers = hotkeyId.signature;
        auto nativeKey = hotkeyId.id;

        if (eventKind == kEventHotKeyPressed)
        {
            printf("KeyPressed: %d\n", nativeKey);
        }
        // eventKind == kEventHotKeyReleased
        else
        {
            printf("KeyReleased: %d\n", nativeKey);
        }
    }

    return noErr;
}

int RegisterGHMPrivateMac::nativeRegisterHotkey()
{
    KeyCombination kc = regUnregKc_;
    UInt32 mod = (UInt32) modifiersToNativeModifiers(kc.modifiers());
    UInt32 key = (UInt32) keyToNativeKey(kc.key());

    EventHotKeyID hotkeyId = {(OSType) mod, key};
    EventHotKeyRef ref = 0;
    auto status = RegisterEventHotKey(
        key,
        mod,
        hotkeyId,
        GetApplicationEventTarget(),
        0,
        &ref
    );
    if (status != noErr)
        return (int) status;
    kcToHotkeyRef_[regUnregKc_] = ref;
    return RC_SUCCESS;
}

int RegisterGHMPrivateMac::nativeUnregisterHotkey()
{
    EventHotKeyRef ref = kcToHotkeyRef_[regUnregKc_];
    auto status = UnregisterEventHotKey(ref);
    if (status != noErr)
        return (int) status;
    kcToHotkeyRef_.erase(regUnregKc_);
    return RC_SUCCESS;
}

void RegisterGHMPrivateMac::tryInvoke() const
{

}

} // namespace gbhk
