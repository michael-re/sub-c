#include <stdlib.h>
#include <string.h>

#include "subc/memory.h"

void* reallocate(void* block, const size_t old_size, const size_t new_size)
{
    if (new_size == 0)
        return deallocate(block);

    void* resized = realloc(block, new_size);
    if (resized == NULL)
        exit(EX_OS_ERROR);

    if (new_size > old_size)
        memset(((char*) resized + old_size), 0x0, (new_size - old_size)); //! NOLINT

    return resized;
}

void* deallocate(void* block)
{
    free(block);
    return NULL;
}
