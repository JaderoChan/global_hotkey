#include <keyboard_tools_details.hpp>

#include <CoreFoundation/CoreFoundation.h>
#include <CoreGraphics/CGEvent.h>

#include "event_converter.hpp"

namespace kbdt
{

namespace details
{

size_t sendEvents(const std::vector<KeyEvent>& events)
{
    size_t sent = 0;
    CGEventRef cgEvent = NULL;
    for (size_t i = 0; i < events.size(); ++i)
    {
        keyEventToCGEvent(events[i], cgEvent);
        if (cgEvent)
        {
            CGEventPost(kCGHIDEventTap, cgEvent);
            CFRelease(cgEvent);
            cgEvent = NULL;
            sent++;
        }
    }
    return sent;
}

} // namespace details

} // namespace kbdt
