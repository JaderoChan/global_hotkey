#ifndef KEYBOARD_TOOLS_TYPES_HPP
#define KEYBOARD_TOOLS_TYPES_HPP

#include <cstddef>  // size_t

namespace kbdt
{

/**
 * @brief Key event types.
 */
enum KeyEventType
{
    KET_RELEASED,   ///< Key released event
    KET_PRESSED     ///< Key pressed event
};

/**
 * @brief Key event structure.
 */
struct KeyEvent
{
    KeyEventType type;  ///< Type of the key event
    int nativeKey;      ///< Native key code
};

/**
 * @brief Key event handler callback type.
 * @note - If the event handler returns true, the event continues to propagate, otherwise, the event is blocked.
 * (Available only on Windows and MacOS platforms)
 */
using KeyEventHandler = bool (*)(KeyEvent);

} // namespace kbdt

#endif // !KEYBOARD_TOOLS_TYPES_HPP
