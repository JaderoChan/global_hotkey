#include "hook_ghm_private.hpp"

#include <global_hotkey/return_code.hpp>
#include <key/key_private.hpp>

namespace gbhk
{

std::mutex HookGHMPrivate::mtx_;
std::condition_variable HookGHMPrivate::cvHasEvent_;
std::queue<Event> HookGHMPrivate::eventQueue_;

HookGHMPrivate::HookGHMPrivate() : kbdtMgr_(kbdt::KeyboardToolsManager::getInstance()) {}

HookGHMPrivate::~HookGHMPrivate() { stop(); }

int HookGHMPrivate::initialize()
{
    clearEventQueue();

    int rc = kbdtMgr_.run();
    if (rc != KBDT_RC_SUCCESS)
        return rc;
    kbdtMgr_.setEventHandler(&kbdtEventHandler);
    return RC_SUCCESS;
}

int HookGHMPrivate::stopWork()
{
    pushEvent({ET_EXIT});
    int rc = kbdtMgr_.stop();
    if (rc != KBDT_RC_SUCCESS)
        return rc;
    return RC_SUCCESS;
}

static bool isMetaKey(Key key)
{
    return key == Key_Mod_Meta || key == Key_Mod_Meta_Left || key == Key_Mod_Meta_Right;
}

static bool isCtrlKey(Key key)
{
    return key == Key_Mod_Ctrl || key == Key_Mod_Ctrl_Left || key == Key_Mod_Ctrl_Right;
}

static bool isAltKey(Key key)
{
    return key == Key_Mod_Alt || key == Key_Mod_Alt_Left || key == Key_Mod_Alt_Right;
}

static bool isShiftKey(Key key)
{
    return key == Key_Mod_Shift || key == Key_Mod_Shift_Left || key == Key_Mod_Shift_Right;
}

void HookGHMPrivate::work()
{
    setRunSuccess();
    bool shouldExit = false;
    KeyCombination prevKc;
    Modifiers pressedMod = 0;
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
                        pressedMod = 0;
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

int HookGHMPrivate::registerHotkeyImpl(const KeyCombination& kc, bool autoRepeat)
{ return RC_SUCCESS; }

int HookGHMPrivate::unregisterHotkeyImpl(const KeyCombination& kc)
{ return RC_SUCCESS; }

void HookGHMPrivate::tryInvoke(const KeyCombination& prevKc, const KeyCombination& currKc) const
{
    auto pair = getPairValue(currKc);
    auto& autoRepeat = pair.first;
    auto& fn = pair.second;
    bool shouldInvoke = fn && (currKc != prevKc || autoRepeat);
    if (shouldInvoke)
        fn();
}

Event HookGHMPrivate::takeEvent()
{
    std::unique_lock<std::mutex> lock(mtx_);
    cvHasEvent_.wait(lock, []() { return !eventQueue_.empty(); });
    Event ev = eventQueue_.front();
    eventQueue_.pop();
    return ev;
}

void HookGHMPrivate::pushEvent(const Event& event)
{
    {
        std::lock_guard<std::mutex> lock(mtx_);
        eventQueue_.push(event);
    }
    cvHasEvent_.notify_one();
}

void HookGHMPrivate::clearEventQueue()
{
    std::lock_guard<std::mutex> lock(mtx_);
    while (!eventQueue_.empty())
        eventQueue_.pop();
}

bool HookGHMPrivate::kbdtEventHandler(kbdt::KeyEvent event)
{
    auto key = keyFromNativeKey(event.nativeKey);
    auto et = (event.type == kbdt::KET_PRESSED ? ET_KEY_PRESSED : ET_KEY_RELEASED);
    pushEvent({et, key});
    return true;
}

} // namespace gbhk
