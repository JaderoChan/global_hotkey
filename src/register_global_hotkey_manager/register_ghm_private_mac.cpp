#include "register_ghm_private_mac.hpp"

#include <global_hotkey/return_code.hpp>
#include <key/key_private.hpp>

namespace gbhk
{

int RegisterGHMPrivateMac::regUnregRc_ = -2;
KeyCombination RegisterGHMPrivateMac::regUnregKc_;
std::mutex RegisterGHMPrivateMac::regUnregRcKcTypeMtx_;
std::condition_variable RegisterGHMPrivateMac::regUnregRcCv_;
EventType RegisterGHMPrivateMac::eventType_ = ET_NONE;
std::unordered_map<KeyCombination, EventHotKeyRef> RegisterGHMPrivateMac::kcToHotkeyRef_;

KeyCombination RegisterGHMPrivateMac::prevKc_;
KeyCombination RegisterGHMPrivateMac::currKc_;

RegisterGHMPrivateMac::RegisterGHMPrivateMac() = default;

RegisterGHMPrivateMac::~RegisterGHMPrivateMac() { stop(); }

int RegisterGHMPrivateMac::initialize()
{
    sourceContext_ = {
        .version = 0,
        .info = this,
        .retain = nullptr,
        .release = nullptr,
        .copyDescription = nullptr,
        .equal = nullptr,
        .hash = nullptr,
        .schedule = nullptr,
        .cancel = nullptr,
        .perform = &RegisterGHMPrivateMac::runLoopSourceCallback
    };
    return RC_SUCCESS;
}

int RegisterGHMPrivateMac::stopWork()
{
    CFRunLoopStop(runLoop_);
    return RC_SUCCESS;
}

void RegisterGHMPrivateMac::work()
{
    runLoop_ = CFRunLoopGetCurrent();
    if (runLoop_ == nullptr)
    {
        // TODO: Need error code.
        setRunFail(-1);
        return;
    }

    EventTypeSpec eventTypeSpecs[2] = {0};
    eventTypeSpecs[0].eventClass = kEventClassKeyboard;
    eventTypeSpecs[0].eventKind = kEventHotKeyPressed;
    eventTypeSpecs[1].eventClass = kEventClassKeyboard;
    eventTypeSpecs[1].eventKind = kEventHotKeyReleased;

    EventHandlerRef eventHandler;
    auto status = InstallApplicationEventHandler(
        &RegisterGHMPrivateMac::hotkeyEventHandler,
        2, eventTypeSpecs,
        this, &eventHandler
    );
    if (status != noErr)
    {
        setRunFail(status);
        return;
    }

    source_ = CFRunLoopSourceCreate(nullptr, 0, &sourceContext_);
    if (source_ == nullptr)
    {
        // TODO: Need error code.
        setRunFail(-1);
        return;
    }
    CFRunLoopAddSource(runLoop_, source_, kCFRunLoopDefaultMode);

    setRunSuccess();
    CFRunLoopRun();

    if (eventHandler)
        RemoveEventHandler(eventHandler);
    CFRelease(source_);

    sourceContext_ = {0};
    source_ = nullptr;
    runLoop_ = nullptr;
    regUnregRc_ = -2;
    eventType_ = ET_NONE;
    regUnregKc_ = KeyCombination();
    kcToHotkeyRef_.clear();
}

int RegisterGHMPrivateMac::registerHotkeyImpl(const KeyCombination& kc, bool autoRepeat)
{
    {
        std::lock_guard<std::mutex> locker(regUnregRcKcTypeMtx_);
        regUnregRc_ = -1;
        regUnregKc_ = kc;
        eventType_ = ET_REGISTER;
    }

    CFRunLoopSourceSignal(source_);
    CFRunLoopWakeUp(runLoop_);

    std::unique_lock<std::mutex> locker(regUnregRcKcTypeMtx_);
    regUnregRcCv_.wait(locker, [this]() { return regUnregRc_ != -1; });
    return regUnregRc_;
}

int RegisterGHMPrivateMac::unregisterHotkeyImpl(const KeyCombination& kc)
{
    {
        std::lock_guard<std::mutex> locker(regUnregRcKcTypeMtx_);
        regUnregRc_ = -1;
        regUnregKc_ = kc;
        eventType_ = ET_UNREGISTER;
    }

    CFRunLoopSourceSignal(source_);
    CFRunLoopWakeUp(runLoop_);

    std::unique_lock<std::mutex> locker(regUnregRcKcTypeMtx_);
    regUnregRcCv_.wait(locker, [this]() { return regUnregRc_ != -1; });
    return regUnregRc_;
}

void RegisterGHMPrivateMac::runLoopSourceCallback(void* info)
{
    {
        std::lock_guard<std::mutex> locker(regUnregRcKcTypeMtx_);
        switch (eventType_)
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
    }
    regUnregRcCv_.notify_one();
}

OSStatus RegisterGHMPrivateMac::hotkeyEventHandler(EventHandlerCallRef nextHandler, EventRef event, void* userData)
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
            nullptr,
            sizeof(hotkeyId),
            nullptr,
            &hotkeyId
        );
        if (status != noErr)
            return status;

        if (eventKind == kEventHotKeyPressed)
        {
            auto nativeModifiers = hotkeyId.signature;
            auto nativeKey = hotkeyId.id;
            auto mod = modifiersFromNativeModifiers(nativeModifiers);
            auto key = keyFromNativeKey(nativeKey);
            currKc_ = {mod, key};
        }
        else
        {
            currKc_ = {};
        }

        tryInvoke(prevKc_, currKc_);
        prevKc_ = currKc_;
    }

    return noErr;
}

int RegisterGHMPrivateMac::nativeRegisterHotkey()
{
    KeyCombination kc = regUnregKc_;
    UInt32 mod = static_cast<UInt32>(modifiersToNativeModifiers(kc.modifiers()));
    UInt32 key = static_cast<UInt32>(keyToNativeKey(kc.key()));

    EventHotKeyID hotkeyId = {static_cast<OSType>(mod), key};
    EventHotKeyRef eventHotkey = 0;
    auto status = RegisterEventHotKey(
        key,
        mod,
        hotkeyId,
        GetApplicationEventTarget(),
        0,
        &eventHotkey
    );
    if (status != noErr)
        return static_cast<int>(status);
    kcToHotkeyRef_[regUnregKc_] = eventHotkey;
    return RC_SUCCESS;
}

int RegisterGHMPrivateMac::nativeUnregisterHotkey()
{
    EventHotKeyRef eventHotkey = kcToHotkeyRef_[regUnregKc_];
    auto status = UnregisterEventHotKey(eventHotkey);
    if (status != noErr)
        return static_cast<int>(status);
    kcToHotkeyRef_.erase(regUnregKc_);
    return RC_SUCCESS;
}

void RegisterGHMPrivateMac::tryInvoke(const KeyCombination& prevKc, const KeyCombination& currKc)
{
    auto pair = static_cast<RegisterGHMPrivateMac&>(getInstance()).getPairValue(currKc);
    const auto& autoRepeat = pair.first;
    const auto& fn = pair.second;
    bool shouldInvoke = fn && (currKc != prevKc || autoRepeat);
    if (shouldInvoke)
        fn();
}

} // namespace gbhk
