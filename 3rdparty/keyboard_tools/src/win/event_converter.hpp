#ifndef KEYBOARD_TOOLS_WIN_EVENT_CONVERTER_HPP
#define KEYBOARD_TOOLS_WIN_EVENT_CONVERTER_HPP

#include <windows.h>

#include <keyboard_tools/types.hpp>

namespace kbdt
{

namespace details
{

void keyEventToInput(const KeyEvent& event, INPUT& input);

void keyEventFromParam(KeyEvent& event, WPARAM wParam, LPARAM lParam);

} // namespace details

} // namespace kbdt

#endif // !KEYBOARD_TOOLS_WIN_EVENT_CONVERTER_HPP
