#pragma once

#ifndef SUBC_MEMORY_H
#define SUBC_MEMORY_H

#include "subc/common.h"

#define NEW(pointer, count) \
reallocate(NULL, 0, sizeof(*(pointer)) * (count))

#define NEW_RESIZE(pointer, old_count, new_count) \
reallocate(pointer, sizeof(*(pointer)) * (old_count), sizeof(*(pointer)) * (new_count))

#define DELETE(pointer) \
deallocate(pointer)

void* reallocate(void* block, const size_t old_size, const size_t new_size);
void* deallocate(void* block);

#endif // SUBC_MEMORY_H
