#ifndef EVENT_HPP
#define EVENT_HPP

#include <cstdint>

enum EventType
{
    ET_EXIT,
    ET_ADD_HOTKEY,
    ET_REMOVE_HOTKEY,
    ET_REPLACE_HOTKEY,
    ET_SIMPLE_WORK,
    ET_HEAVY_WORK,
    ET_SET_HEAVY_WORK_LEVEL
};

#endif // !EVENT_HPP
