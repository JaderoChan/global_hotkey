#ifndef GLOBAL_HOTKEY_DISABLE_HOOK

#include "hook_ghm_private.hpp"

#include <queue>    // queue

#include <global_hotkey/return_code.hpp>

#include "../key/key_private.hpp"

namespace gbhk
{

std::mutex HookGHMPrivate::mtx_;
std::condition_variable HookGHMPrivate::cvHasEvent_;
std::queue<Event> HookGHMPrivate::eventQueue_;

HookGHMPrivate::HookGHMPrivate() = default;

HookGHMPrivate::~HookGHMPrivate() { uninitialize(); }

int HookGHMPrivate::doBeforeThreadRun()
{
    clearEventQueue();

    int rc = kbdt_start();
    if (rc != KBDT_RC_SUCCESS)
        return rc;
    kbdt_set_event_handler(&kbdtEventHandler);
    return RC_SUCCESS;
}

int HookGHMPrivate::doBeforeThreadEnd()
{
    pushEvent({ET_EXIT});
    int rc = kbdt_stop();
    if (rc != KBDT_RC_SUCCESS)
        return rc;
    return RC_SUCCESS;
}

void HookGHMPrivate::work()
{
    setRunSuccess();
    KeyCombination prevKc;
    Modifiers pressedMod = 0;
    Key pressedKey = 0;
    while (true)
    {
        Event ev = takeEvent();

        if (ev.type == ET_EXIT)
            break;

        // ev.type == ET_KEY_PRESSED || ev.type == ET_KEY_RELEASED
        Key key = ev.data;
        if (ev.type == ET_KEY_PRESSED)
        {
            if (key == Key_Mod_Meta || key == Key_Mod_Meta_Left || key == Key_Mod_Meta_Right)
                pressedMod = pressedMod.add(META);
            else if (key == Key_Mod_Ctrl || key == Key_Mod_Ctrl_Left || key == Key_Mod_Ctrl_Right)
                pressedMod = pressedMod.add(CTRL);
            else if (key == Key_Mod_Alt || key == Key_Mod_Alt_Left || key == Key_Mod_Alt_Right)
                pressedMod = pressedMod.add(ALT);
            else if (key == Key_Mod_Shift || key == Key_Mod_Shift_Left || key == Key_Mod_Shift_Right)
                pressedMod = pressedMod.add(SHIFT);
            else
                pressedKey = key;
        }
        // ev.type == ET_KEY_RELEASED
        else
        {
            if (key == Key_Mod_Meta || key == Key_Mod_Meta_Left || key == Key_Mod_Meta_Right)
                pressedMod = pressedMod.remove(META);
            else if (key == Key_Mod_Ctrl || key == Key_Mod_Ctrl_Left || key == Key_Mod_Ctrl_Right)
                pressedMod = pressedMod.remove(CTRL);
            else if (key == Key_Mod_Alt || key == Key_Mod_Alt_Left || key == Key_Mod_Alt_Right)
                pressedMod = pressedMod.remove(ALT);
            else if (key == Key_Mod_Shift || key == Key_Mod_Shift_Left || key == Key_Mod_Shift_Right)
                pressedMod = pressedMod.remove(SHIFT);
            else if (pressedKey == key)
                pressedKey = 0;
        }

        KeyCombination currKc(pressedMod, pressedKey);
        invoke(prevKc, currKc);
        prevKc = currKc;
    }
}

int HookGHMPrivate::registerHotkey(const KeyCombination& kc, bool autoRepeat)
{ return RC_SUCCESS; }

int HookGHMPrivate::unregisterHotkey(const KeyCombination& kc)
{ return RC_SUCCESS; }

void HookGHMPrivate::invoke(const KeyCombination& prevKc, const KeyCombination& currKc) const
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

void HookGHMPrivate::kbdtEventHandler(keyboard_event* event)
{
    auto key = keyFromNativeKey(event->native_key);
    auto et = (event->type == KBDET_PRESSED ? ET_KEY_PRESSED : ET_KEY_RELEASED);
    pushEvent({et, key});
}

} // namespace gbhk

#endif // !GLOBAL_HOTKEY_DISABLE_HOOK
