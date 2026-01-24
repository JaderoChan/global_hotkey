#ifndef KBDT_LINUX_CONTAINER_H
#define KBDT_LINUX_CONTAINER_H

#include <stddef.h>

struct container
{
    void* data;
    size_t step;
    size_t size;
    size_t capacity;
};

void construct_container(struct container* cont, size_t step, size_t reserved);

void destroy_container(struct container* cont);

void add_item_to_container(struct container* cont, void* item);

void* get_item_in_container(struct container* cont, size_t index);

void remove_item_in_container(struct container* cont, size_t index);

void clear_container(struct container* cont);

#endif // !KBDT_LINUX_CONTAINER_H
