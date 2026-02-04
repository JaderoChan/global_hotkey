#include "utilitys.hpp"

#include <cctype>   // isspace, isalnum, toupper
#include <cstddef>  // size_t
#include <cstdlib>

bool isImmutableKeyCombination(const KeyCombination &kc) {
  return kc == exitKc || kc == addKc || kc == removeKc || kc == replaceKc ||
         kc == listKc || kc == clearTerminalKc ||
         kc == simpleWorkKc || kc == heavyWorkKc || kc == setHeavyWorkLevelKc;
}

void printPrompt()
{
    printf
    (
"\
====================\n\
Press [%s] to exit\n\
Press [%s] to register a hotkey\n\
Press [%s] to unregister a hotkey\n\
Press [%s] to replace a hotkey\n\
Press [%s] to list all hotkey\n\
Press [%s] to perform a simple work\n\
Press [%s] to perform a heavy work\n\
Press [%s] to set the heavy work time\n\
Press [%s] to clear the terminal\n\
====================\n\n\
",
        KCSTR(exitKc),
        KCSTR(addKc),
        KCSTR(removeKc),
        KCSTR(replaceKc),
        KCSTR(listKc),
        KCSTR(simpleWorkKc),
        KCSTR(heavyWorkKc),
        KCSTR(setHeavyWorkLevelKc),
        KCSTR(clearTerminalKc)
    );
}

void clearTerminal()
{
#ifdef GLOBAL_HOTKEY_WIN
    system("cls");
#else
    int ret = system("clear");
#endif // GLOBAL_HOTKEY_WIN
    printPrompt();
}

void listAllKeyCombination()
{
    auto kcs = GHM.getRegisteredHotkeys();
    printf("====================\n");
    for (const auto& kc : kcs)
    {
        if (isImmutableKeyCombination(kc))
            printf("<Immutable> ");
        printf("[%s]\n", KCSTR(kc));
    }
    printf("====================\n");
}

#define IS_SPACE(c) std::isspace(static_cast<unsigned char>(c))
#define IS_ALNUM(c) std::isalnum(static_cast<unsigned char>(c))
#define TO_UPPER(c) std::toupper(static_cast<unsigned char>(c))

bool isEqualStr(const std::string& str1, const std::string& str2)
{
    size_t i = 0, j = 0;
    while (i < str1.size() || j < str2.size())
    {
        while (i < str1.size() && (IS_SPACE(str1[i]) || str1[i] == '_')) i++;
        while (j < str2.size() && (IS_SPACE(str2[j]) || str2[j] == '_')) j++;

        if (i >= str1.size() || j >= str2.size())
            return (i >= str1.size() && j >= str2.size());

        char c1 = TO_UPPER(str1[i]);
        char c2 = TO_UPPER(str2[j]);
        if (c1 != c2)
            return false;

        i++;
        j++;
    }

    return true;
}

