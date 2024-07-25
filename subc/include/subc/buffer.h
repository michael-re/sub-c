#pragma once

#ifndef SUBC_BUFFER_H
#define SUBC_BUFFER_H

#include "subc/common.h"

// =============================================================================
// generic resizable buffer
// =============================================================================
typedef struct buffer
{
    size_t bytes_allocated;
    size_t element_size;
    size_t capacity;
    void*  data;
} *buffer_t;

buffer_t buffer_create(size_t element_size, size_t initial_capacity);
buffer_t buffer_delete(buffer_t self);
void*    buffer_expand(buffer_t self, size_t min_capacity);
void*    buffer_shrink(buffer_t self, size_t max_capacity);

#endif // SUBC_BUFFER_H
