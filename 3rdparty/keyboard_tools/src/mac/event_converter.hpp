#ifndef KBDT_MAC_EVENT_CONVERTER_HPP
#define KBDT_MAC_EVENT_CONVERTER_HPP

#include <CoreGraphics/CGEvent.h>

#include <kbdt/types.hpp>

namespace kbdt
{

namespace details
{

void keyEventToCGEvent(const KeyEvent& event, CGEventRef& cgEvent);

void keyEventFromCGEvent(KeyEvent& event, CGEventType cgEventType, CGEventRef cgEvent);

} // namespace details

} // namespace kbdt

#endif // !KBDT_MAC_EVENT_CONVERTER_HPP
