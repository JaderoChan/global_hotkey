#include "hook_ghm_private.hpp"

#include <global_hotkey/return_code.hpp>
#include <key/key_private.hpp>

namespace gbhk
{

#define MAKE_VERSION(major, minor, patch) ((major << 16) | (minor << 8) | patch)
#define KBDT_VERSION \
    MAKE_VERSION(KEYBOARD_TOOLS_VERSION_MAJOR, KEYBOARD_TOOLS_VERSION_MINOR, KEYBOARD_TOOLS_VERSION_PATCH)
#define GET_KEY_STATE_AVAILABLE_VERSION MAKE_VERSION(3, 0, 3)

#if KBDT_VERSION >= GET_KEY_STATE_AVAILABLE_VERSION
    #define GET_KEY_STATE_AVAILABLE
#endif

std::queue<Event> HookGHMPrivate::eventQueue_;
std::mutex HookGHMPrivate::eventQueueMtx_;
std::condition_variable HookGHMPrivate::eventQueueCv_;

HookGHMPrivate::HookGHMPrivate() : eventHookService_(kbt::EventHookService::getInstance()) {}

HookGHMPrivate::~HookGHMPrivate() { stop(); }

HookGHMPrivate& HookGHMPrivate::getInstance()
{
    static HookGHMPrivate instance;
    return instance;
}

int HookGHMPrivate::initialize()
{
    clearEventQueue();

    int rc = eventHookService_.run();
    if (rc != KBT_RC_SUCCESS)
        return rc;
    eventHookService_.setEventHandler(&kbtEventHandler);
    return RC_SUCCESS;
}

int HookGHMPrivate::stopWork()
{
    pushEvent({ET_EXIT});
    int rc = eventHookService_.stop();
    if (rc != KBT_RC_SUCCESS)
        return rc;
    return RC_SUCCESS;
}

static inline bool isMetaKey(Key key)
{
    return key == Key_Mod_Meta || key == Key_Mod_Meta_Left || key == Key_Mod_Meta_Right;
}

static inline bool isCtrlKey(Key key)
{
    return key == Key_Mod_Ctrl || key == Key_Mod_Ctrl_Left || key == Key_Mod_Ctrl_Right;
}

static inline bool isAltKey(Key key)
{
    return key == Key_Mod_Alt || key == Key_Mod_Alt_Left || key == Key_Mod_Alt_Right;
}

static inline bool isShiftKey(Key key)
{
    return key == Key_Mod_Shift || key == Key_Mod_Shift_Left || key == Key_Mod_Shift_Right;
}

static inline bool isModifierKey(Key key)
{
    return isMetaKey(key) || isCtrlKey(key) || isAltKey(key) || isShiftKey(key);
}

static inline bool isPressedKey(int32_t nativeKey)
{
    return nativeKey < 0 ? false : kbt::getKeyState(nativeKey) == kbt::KS_PRESSED;
}

static inline Modifiers getCurrentModifiers()
{
    static const auto metaKey       = keyToNativeKey(Key_Mod_Meta);
    static const auto metaLKey      = keyToNativeKey(Key_Mod_Meta_Left);
    static const auto metaRKey      = keyToNativeKey(Key_Mod_Meta_Right);
    static const auto ctrlKey       = keyToNativeKey(Key_Mod_Ctrl);
    static const auto ctrlLKey      = keyToNativeKey(Key_Mod_Ctrl_Left);
    static const auto ctrlRKey      = keyToNativeKey(Key_Mod_Ctrl_Right);
    static const auto altKey        = keyToNativeKey(Key_Mod_Alt);
    static const auto altLKey       = keyToNativeKey(Key_Mod_Alt_Left);
    static const auto altRKey       = keyToNativeKey(Key_Mod_Alt_Right);
    static const auto shiftKey      = keyToNativeKey(Key_Mod_Shift);
    static const auto shiftLKey     = keyToNativeKey(Key_Mod_Shift_Left);
    static const auto shiftRKey     = keyToNativeKey(Key_Mod_Shift_Right);

    bool hasMeta  = isPressedKey(metaKey)  || isPressedKey(metaLKey)  || isPressedKey(metaRKey);
    bool hasCtrl  = isPressedKey(ctrlKey)  || isPressedKey(ctrlLKey)  || isPressedKey(ctrlRKey);
    bool hasAlt   = isPressedKey(altKey)   || isPressedKey(altLKey)   || isPressedKey(altRKey);
    bool hasShift = isPressedKey(shiftKey) || isPressedKey(shiftLKey) || isPressedKey(shiftRKey);

    return Modifiers(
        (hasMeta  ? META  : 0) |
        (hasCtrl  ? CTRL  : 0) |
        (hasAlt   ? ALT   : 0) |
        (hasShift ? SHIFT : 0));
}

// Get key state function performace is poor on Linux, so use old work().
#if defined(GET_KEY_STATE_AVAILABLE) && !defined(GLOBAL_HOTKEY_LINUX)
void HookGHMPrivate::work()
{
    setRunSuccess();
    bool shouldExit = false;
    KeyCombination prevKc;
    while (!shouldExit)
    {
        Event ev = takeEvent();
        switch (ev.type)
        {
            case ET_EXIT:
                shouldExit = true;
                break;
            case ET_KEY_PRESSED:
                if (!isModifierKey(static_cast<Key>(ev.data)))
                {
                    auto mod = getCurrentModifiers();
                    KeyCombination currKc(mod, static_cast<Key>(ev.data));
                    tryInvoke(prevKc, currKc);
                    prevKc = currKc;
                }
                break;
            case ET_KEY_RELEASED:
                prevKc = {};
                break;
            default:
                break;
        }
    }
}
#else
void HookGHMPrivate::work()
{
    setRunSuccess();
    bool shouldExit = false;
    KeyCombination prevKc;
    Modifiers pressedMod;
    while (!shouldExit)
    {
        Event ev = takeEvent();
        switch (ev.type)
        {
            case ET_EXIT:
            {
                shouldExit = true;
                break;
            }
            case ET_KEY_PRESSED:
            {
                Key key = ev.data;
                if (isMetaKey(key))
                {
                    pressedMod.add(META);
                }
                else if (isCtrlKey(key))
                {
                    pressedMod.add(CTRL);
                }
                else if (isAltKey(key))
                {
                    pressedMod.add(ALT);
                }
                else if (isShiftKey(key))
                {
                    pressedMod.add(SHIFT);
                }
                else
                {
                #if defined(GLOBAL_HOTKEY_WIN) && defined(GLOBAL_HOTKEY_OPTIMIZE_SYSTEM_RESERVE_HOTKEY)
                    bool isCtrlShiftEsc = (pressedMod == (CTRL | SHIFT) && key == Key_Esc);
                    bool isCtrlAltDel = (pressedMod == (CTRL | ALT) && key == Key_Delete);
                    if (isCtrlShiftEsc || isCtrlAltDel)
                    {
                        pressedMod = 0U;
                        break;
                    }
                #endif

                    KeyCombination currKc(pressedMod, key);
                    tryInvoke(prevKc, currKc);
                    prevKc = currKc;
                }
                break;
            }
            case ET_KEY_RELEASED:
            {
                Key key = ev.data;
                if (isMetaKey(key))
                    pressedMod.remove(META);
                else if (isCtrlKey(key))
                    pressedMod.remove(CTRL);
                else if (isAltKey(key))
                    pressedMod.remove(ALT);
                else if (isShiftKey(key))
                    pressedMod.remove(SHIFT);
                prevKc = {};
                break;
            }
            default:
                break;
        }
    }
}
#endif // GET_KEY_STATE_AVAILABLE

int HookGHMPrivate::registerHotkeyImpl(const KeyCombination& kc, bool autoRepeat)
{ return RC_SUCCESS; }

int HookGHMPrivate::unregisterHotkeyImpl(const KeyCombination& kc)
{ return RC_SUCCESS; }

void HookGHMPrivate::tryInvoke(const KeyCombination& prevKc, const KeyCombination& currKc) const
{
    const auto pair = getPairValue(currKc);
    const auto& autoRepeat = pair.first;
    const auto& fn = pair.second;
    bool shouldInvoke = fn && (currKc != prevKc || autoRepeat);
    if (shouldInvoke)
        fn();
}

Event HookGHMPrivate::takeEvent()
{
    std::unique_lock<std::mutex> locker(eventQueueMtx_);
    eventQueueCv_.wait(locker, []() { return !eventQueue_.empty(); });
    Event ev = eventQueue_.front();
    eventQueue_.pop();
    return ev;
}

void HookGHMPrivate::pushEvent(const Event& event)
{
    {
        std::lock_guard<std::mutex> locker(eventQueueMtx_);
        eventQueue_.push(event);
    }
    eventQueueCv_.notify_one();
}

void HookGHMPrivate::clearEventQueue()
{
    std::lock_guard<std::mutex> locker(eventQueueMtx_);
    while (!eventQueue_.empty())
        eventQueue_.pop();
}

bool HookGHMPrivate::kbtEventHandler(kbt::KeyEvent event)
{
    auto key = keyFromNativeKey(event.nativeKey);
    auto et = (event.type == kbt::KET_PRESSED ? ET_KEY_PRESSED : ET_KEY_RELEASED);
    pushEvent({et, key});
    return true;
}

} // namespace gbhk
