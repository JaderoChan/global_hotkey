#ifndef GLOBAL_HOTKEY_HOOK_GHM_PRIVATE_HPP
#define GLOBAL_HOTKEY_HOOK_GHM_PRIVATE_HPP

#include <global_hotkey_manager/ghm_private.hpp>

#include <queue>    // queue

#include <keyboard_tools/keyboard_tools.hpp>

namespace gbhk
{

enum EventType
{
    ET_EXIT         = 1,
    ET_KEY_PRESSED  = 2,
    ET_KEY_RELEASED = 3
};

struct Event
{
    EventType type;
    int32_t data;
};

class HookGHMPrivate final : public GHMPrivate
{
public:
    HookGHMPrivate();
    ~HookGHMPrivate();

protected:
    int initialize() override;
    int stopWork() override;
    void work() override;
    int registerHotkeyImpl(const KeyCombination& kc, bool autoRepeat) override;
    int unregisterHotkeyImpl(const KeyCombination& kc) override;

private:
    void tryInvoke(const KeyCombination& prevKc, const KeyCombination& currKc) const;

    static std::mutex mtx_;
    static std::condition_variable cvHasEvent_;
    static std::queue<Event> eventQueue_;

    // Block until has event pushed.
    static Event takeEvent();
    static void pushEvent(const Event& event);
    static void clearEventQueue();
    static bool kbdtEventHandler(kbdt::KeyEvent event);

    kbdt::KeyboardToolsManager& kbdtMgr_;
};

} // namespace gbhk

#endif // !GLOBAL_HOTKEY_HOOK_GHM_PRIVATE_HPP
