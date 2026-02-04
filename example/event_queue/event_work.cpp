#include "event_work.hpp"

#include <cstddef>  // size_t
#include <chrono>
#include <thread>

#include "utilitys.hpp"

constexpr size_t BUF_SIZE = 512;
static char buf[BUF_SIZE];

void exitWork()
{
    int rc = GHM.stop();
    if (rc == RC_SUCCESS)
        printf("Exit successfully!\n");
    else
        printf("Failed to exit! Error: %s\n", RCMSG(rc));
}

void addHotkeyWork()
{
    printf("Please input string of the hotkey you want to register (e.g. Ctrl+C)\n");
    int ret = scanf("%s", buf);
    KeyCombination kc(buf);

    if (!kc.isValid())
    {
        printf("The hotkey [%s] given is invalid!\n", KCSTR(kc));
        return;
    }

    if (GHM.isHotkeyRegistered(kc))
    {
        printf("The hotkey [%s] given is exists!\n", KCSTR(kc));
        return;
    }

    printf("Please input text it be print when the hotkey be triggered\n");
    ret = scanf("%s", buf);

    std::string str(buf);
    int rc = GHM.registerHotkey(kc, [=]() { printf("%s\n", str.c_str()); });
    if (rc == RC_SUCCESS)
        printf("Register the hotkey [%s]! successfully\n", KCSTR(kc));
    else
        printf("Failed to register the hotkey [%s]! Error: %s\n", KCSTR(kc), RCMSG(rc));
}

void removeHotkeyWork()
{
    printf("Please input string of the hotkey you want to unregister (e.g. Ctrl+C)\n");
    int ret = scanf("%s", buf);
    KeyCombination kc(buf);

    if (!GHM.isHotkeyRegistered(kc))
    {
        printf("The hotkey [%s] given not exists!\n", KCSTR(kc));
        return;
    }

    if (isImmutableKeyCombination(kc))
    {
        printf("The hotkey [%s] is can't be unregister!\n", KCSTR(kc));
        return;
    }

    int rc = GHM.unregisterHotkey(kc);
    if (rc == RC_SUCCESS)
        printf("Unregister the hotkey [%s]! successfully\n", KCSTR(kc));
    else
        printf("Failed to unregister the hotkey [%s]! Error: %s\n", KCSTR(kc), RCMSG(rc));
}

void replaceHotkeyWork()
{
    printf("Please input string of the old hotkey you want to replace (e.g. Ctrl+C)\n");
    int ret = scanf("%s", buf);
    KeyCombination oldKc(buf);

    if (!GHM.isHotkeyRegistered(oldKc))
    {
        printf("The old hotkey [%s] given not exists!\n", KCSTR(oldKc));
        return;
    }

    if (isImmutableKeyCombination(oldKc))
    {
        printf("The hotkey [%s] is can't be replace!\n", KCSTR(oldKc));
        return;
    }

    printf("Please input string of the new hotkey you want to register (e.g. Ctrl+Shift+C)\n");
    ret = scanf("%s", buf);
    KeyCombination newKc(buf);

    if (!newKc.isValid())
    {
        printf("The new hotkey [%s] given is invalid!\n", KCSTR(newKc));
        return;
    }

    if (GHM.isHotkeyRegistered(newKc))
    {
        printf("The newKc hotkey [%s] given is exists!\n", KCSTR(newKc));
        return;
    }

    int rc = GHM.replaceHotkey(oldKc, newKc);
    if (rc == RC_SUCCESS)
        printf("Replace the hotkey [%s] to hotkey [%s]! successfully\n", KCSTR(oldKc), KCSTR(newKc));
    else
        printf("Failed to replace the hotkey [%s] to hotkey [%s]! Error: %s\n",
            KCSTR(oldKc), KCSTR(newKc), RCMSG(rc));
}

void setHotkeyAutoRepeatWork()
{
    printf("Please input string of the hotkey you want to set is auto repeat (e.g. Ctrl+C)\n");
    int ret = scanf("%s", buf);
    KeyCombination kc(buf);

    if (GHM.isHotkeyRegistered(kc))
    {
        printf("The hotkey [%s] given is exists!\n", KCSTR(kc));
        return;
    }

    bool autoRepeat = false;
    printf("Please input \"Y/N (Yes/No)\" to set whether the hotkey is auto repeat\n");
    ret = scanf("%s", buf);
    while (true)
    {
        if (isEqualStr(buf, "yes") || isEqualStr(buf, "y"))
        {
            autoRepeat = true;
            break;
        }
        else if (isEqualStr(buf, "no") || isEqualStr(buf, "n"))
        {
            autoRepeat = false;
            break;
        }
        else
        {
            printf("Invalid input, please retry\n");
            ret = scanf("%s", buf);
        }
    }

    int rc = GHM.setHotkeyAutoRepeat(kc, autoRepeat);
    if (rc == RC_SUCCESS)
        printf("Set the hotkey [%s] to %s! successfully\n",
            KCSTR(kc), autoRepeat ? "auto repeat" : "no auto repeat");
    else
        printf("Failed to set the hotkey [%s] to %s! Error: %s\n",
            KCSTR(kc), autoRepeat ? "auto repeat" : "no auto repeat", RCMSG(rc));
}

void simpleWork()
{
    printf("This is a simple work. Just print a message\n");
}

// In seconds.
static int heavyWorkLevel = 10;

void heavyWork()
{
    printf(
"This is a heavy work. It will take a long time [current level: %d] to finish\n\
(print a message every 1 second, the level indicates how long it will take)\n", heavyWorkLevel);
    int level = heavyWorkLevel;
    std::thread th = std::thread([=]() {
        int i = 0;
        while (i < level)
        {
            printf("Heavy work: %d\n", i);
            std::this_thread::sleep_for(std::chrono::milliseconds(1000));
            i++;
        }
        printf("Heavy work: %d\n", i);
        printf("Heavy work finished!\n");
    });
    th.detach();
}

void setHeavyWorkLevelWork()
{
    printf("Please input level of the heavy work\n");
    int level = 0;
    int ret = scanf("%d", &level);
    if (level < 1)
    {
        printf("Invalid input, please retry\n");
        ret = scanf("%d", &level);
    }
    heavyWorkLevel = level;
}
