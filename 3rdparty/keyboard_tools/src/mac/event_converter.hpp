#ifndef KEYBOARD_TOOLS_MAC_EVENT_CONVERTER_HPP
#define KEYBOARD_TOOLS_MAC_EVENT_CONVERTER_HPP

#include <CoreGraphics/CGEvent.h>

#include <keyboard_tools/types.hpp>

namespace kbdt
{

namespace details
{

void keyEventToCGEvent(const KeyEvent& event, CGEventRef& cgEvent);

void keyEventFromCGEvent(KeyEvent& event, CGEventType cgEventType, CGEventRef cgEvent);

} // namespace details

} // namespace kbdt

#endif // !KEYBOARD_TOOLS_MAC_EVENT_CONVERTER_HPP
