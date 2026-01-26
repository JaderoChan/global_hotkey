#ifndef UTILITYS_HPP
#define UTILITYS_HPP

#include "defines.hpp"

bool isImmutableKeyCombination(const KeyCombination& kc);

void printPrompt();

void clearTerminal();

void listAllKeyCombination();

/// @brief Check whether two strings is equal, ignoring case, spaces and underscores.
bool isEqualStr(const std::string& str1, const std::string& str2);

#endif // !UTILITYS_HPP
