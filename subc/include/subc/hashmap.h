#pragma once

#ifndef SUBC_HASHMAP_H
#define SUBC_HASHMAP_H

#include "subc/common.h"
#include "subc/buffer.h"
#include "subc/string.h"

// =============================================================================
// hashmap entry
// =============================================================================
typedef struct entry
{
    string_t key;
    uint64_t value;
} *entry_t;

// =============================================================================
// hashmap entry
// =============================================================================
typedef struct hashmap
{
    buffer_t buffer;
    size_t   size;
    double   max_load;
    entry_t  entries;
} *hashmap_t;

hashmap_t hashmap_create  (size_t capacity);
hashmap_t hashmap_delete  (hashmap_t self);
hashmap_t hashmap_resize  (hashmap_t self, size_t min_capacity);
entry_t   hashmap_insert  (hashmap_t self, string_t key, uint64_t value);
entry_t   hashmap_retrieve(hashmap_t self, string_t key);
entry_t   hashmap_contains(hashmap_t self, string_t key);

#endif // SUBC_HASHMAP_H
