#include <kbdt_details.hpp>

#include <windows.h>

#include "event_converter.hpp"

namespace kbdt
{

namespace details
{

size_t sendEvents(const std::vector<KeyEvent>& events)
{
    auto eventCount = events.size();
    std::vector<INPUT> inputs(eventCount);
    for (size_t i = 0; i < eventCount; ++i)
        keyEventToInput(events[i], inputs[i]);
    UINT sent = SendInput((UINT) eventCount, inputs.data(), sizeof(INPUT));
    return sent;
}

} // namespace details

} // namespace kbdt
