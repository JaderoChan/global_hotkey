#include <iostream>
#include <string>

#include "block_queue.hpp"
#include "event.hpp"
#include "event_work.hpp"
#include "utilitys.hpp"

static Queue<EventType> eventQueue;

int main()
{
    GHM.run();

    GHM.registerHotkey(exitKc, [&]() { eventQueue.push(ET_EXIT); });
    GHM.registerHotkey(addKc, [&]() { eventQueue.push(ET_ADD_HOTKEY); });
    GHM.registerHotkey(removeKc, [&]() { eventQueue.push(ET_REMOVE_HOTKEY); });
    GHM.registerHotkey(replaceKc, [&]() { eventQueue.push(ET_REPLACE_HOTKEY); });
    GHM.registerHotkey(simpleWorkKc, [&]() { eventQueue.push(ET_SIMPLE_WORK); });
    GHM.registerHotkey(heavyWorkKc, [&]() { eventQueue.push(ET_HEAVY_WORK); });
    GHM.registerHotkey(setHeavyWorkLevelKc, [&]() { eventQueue.push(ET_SET_HEAVY_WORK_LEVEL); });

    GHM.registerHotkey(listKc, [&]() { listAllKeyCombination(); });
    GHM.registerHotkey(clearTerminalKc, [&]() { clearTerminal(); });

    printPrompt();
    while (true)
    {
        EventType et = eventQueue.take();
        if (et == ET_EXIT)
            break;
        switch (et)
        {
            case ET_ADD_HOTKEY:
                addHotkeyWork();
                break;
            case ET_REMOVE_HOTKEY:
                removeHotkeyWork();
                break;
            case ET_REPLACE_HOTKEY:
                replaceHotkeyWork();
                break;
            case ET_SIMPLE_WORK:
                simpleWork();
                break;
            case ET_HEAVY_WORK:
                heavyWork();
                break;
            case ET_SET_HEAVY_WORK_LEVEL:
                setHeavyWorkLevelWork();
                break;
            default:
                break;
        }
    }

    GHM.stop();

    return 0;
}
