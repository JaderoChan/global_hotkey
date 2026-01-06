#ifndef UTILITY_HPP
#define UTILITY_HPP

#include "defines.hpp"

bool isUnchangedKeyCombination(const KeyCombination& kc);

void printPrompt();

void clearTerminal();

void listAllKeyCombination();

/// @brief Check whether two strings is equal, ignoring case, spaces and underscores.
bool isEqualStr(const std::string& str1, const std::string& str2);

#endif // !UTILITY_HPP
