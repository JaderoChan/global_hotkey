#include "message_id.h"

#include <stdbool.h>
#include <stdlib.h> // rand, srand
#include <time.h>   // time_t, time

uint64_t message_id()
{
    static uint64_t id;
    static bool is_inited = false;
    if (is_inited)
        return id;

    time_t current_time = time(NULL);
    srand((unsigned int) current_time);

    uint32_t random_part = 0;
    for (int i = 0; i < 4; i++)
        random_part = (random_part << 8) | (rand() & 0xFF);

    id = ((uint64_t) current_time << 32) | random_part;
    is_inited = true;
    return id;
}
