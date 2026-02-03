#ifndef KBDT_WIN_EVENT_CONVERTER_HPP
#define KBDT_WIN_EVENT_CONVERTER_HPP

#include <windows.h>

#include <kbdt/types.hpp>

namespace kbdt
{

namespace details
{

void keyEventToInput(const KeyEvent& event, INPUT& input);

void keyEventFromParam(KeyEvent& event, WPARAM wParam, LPARAM lParam);

} // namespace details

} // namespace kbdt

#endif // !KBDT_WIN_EVENT_CONVERTER_HPP
