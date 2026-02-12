#ifndef KEYBOARD_TOOLS_TYPES_HPP
#define KEYBOARD_TOOLS_TYPES_HPP

#include <cstdint>  // uint8_t
#include <cstddef>

namespace kbdt
{

/** @brief Key event types. */
enum KeyEventType : uint8_t
{
    KET_RELEASED,   ///< Key released event
    KET_PRESSED     ///< Key pressed event
};

/** @brief Key event structure. */
struct KeyEvent
{
    KeyEventType type;  ///< Type of the key event
    uint32_t nativeKey; ///< Native key code
};

/**
 * @brief The function type of the key event handler.
 * @note If the event handler returns true, the event continues to propagate, otherwise, the event is blocked.
 * (Available only on Windows and MacOS platforms)
 */
using KeyEventHandler = bool (*)(KeyEvent);

} // namespace kbdt

#endif // !KEYBOARD_TOOLS_TYPES_HPP
