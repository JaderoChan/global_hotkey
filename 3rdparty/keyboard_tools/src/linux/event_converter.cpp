#include "event_converter.hpp"

namespace kbdt
{

namespace details
{

void keyEventToInputEvent(const KeyEvent& event, struct input_event& ie)
{
    ie.type = EV_KEY;
    ie.code = event.nativeKey;
    ie.value = (event.type == KET_PRESSED ? 1 : 0);
    ie.time.tv_sec = 0;
    ie.time.tv_usec = 0;
}

void keyEventFromInputEvent(KeyEvent& event, const struct input_event& ie)
{
    event.type = (ie.value == 0 ? KET_RELEASED : KET_PRESSED);
    event.nativeKey = (uint32_t) ie.code;
}

} // namespace details

} // namespace kbdt
