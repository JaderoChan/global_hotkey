#include "container.h"

#include <memory.h>
#include <stdlib.h>

static void alloc_container(struct container* cont, size_t size)
{
    cont->data = NULL;
    if (size != 0)
        cont->data = malloc(cont->step * size);
    cont->capacity = size;
}

static void dealloc_container(struct container* cont)
{
    if (cont->data)
        free(cont->data);
    cont->data = NULL;
    cont->capacity = 0;
}

static void realloc_container(struct container* cont, size_t size)
{
    if (size == 0)
    {
        dealloc_container(cont);
        return;
    }

    void* data = malloc(cont->step * size);
    size_t min_size = size < cont->size ? size : cont->size;
    if (min_size != 0)
        memcpy(data, cont->data, min_size * cont->step);
    dealloc_container(cont);
    cont->data = data;
    cont->capacity = size;
}

void construct_container(struct container* cont, size_t step, size_t reserved)
{
    if (step == 0)
        return;
    cont->step = step;
    cont->size = 0;
    alloc_container(cont, reserved);
}

void destroy_container(struct container* cont)
{
    dealloc_container(cont);
    cont->size = 0;
}

void add_item_to_container(struct container* cont, void* item)
{
    if (cont->size == cont->capacity)
    {
        if (cont->size == 0)
            realloc_container(cont, 2);
        else
            realloc_container(cont, 2 * cont->capacity);
    }
    memcpy((char*) cont->data + cont->size * cont->step, item, cont->step);
    cont->size++;
}

void* get_item_in_container(struct container* cont, size_t index)
{
    if (index >= cont->size)
        return NULL;
    return ((char*) cont->data + index * cont->step);
}

void remove_item_in_container(struct container* cont, size_t index)
{
    if (index >= cont->size)
        return;
    if (index != cont->size - 1)
        memcpy(
            (char*) cont->data + index * cont->step,
            (char*) cont->data + (index + 1) * cont->step,
            (cont->size - index - 1) * cont->step);
    cont->size--;
}

void clear_container(struct container* cont)
{
    cont->size = 0;
}
