#include "subc/hashmap.h"
#include "subc/memory.h"

// =============================================================================
// hashmap impl
// =============================================================================

hashmap_t hashmap_create(size_t capacity)
{
    hashmap_t self = NEW(self, 1);
    self->buffer   = buffer_create(sizeof(*self->entries), 1024);
    self->size     = 0;
    self->max_load = 0.75;
    self->entries  = self->buffer->data;
    return hashmap_resize(self, capacity);
}

hashmap_t hashmap_delete(hashmap_t self)
{
    if (self)
    {
        for (size_t i = 0; i < self->buffer->capacity; i++)
        {
            self->entries[i].key   = string_delete(self->entries[i].key);
            self->entries[i].value = 0;
        }

        self->buffer = buffer_delete(self->buffer);
        (*self)      = (struct hashmap) { 0 };
    }

    return DELETE(self);
}

hashmap_t hashmap_resize(hashmap_t self, size_t min_capacity)
{
    ASSERT(self         != NULL, "can't resize null hashmap");
    ASSERT(self->buffer != NULL, "corrupted hashmap");
    if (((double) self->buffer->capacity * self->max_load) > ((double) min_capacity))
        return self;

    buffer_t old_buffer   = self->buffer;
    size_t   new_capacity = 1024;

    while (((double) new_capacity * self->max_load) <= (double) min_capacity)
        new_capacity *= 2;

    self->buffer  = buffer_create(sizeof(*self->entries), new_capacity);
    self->size    = 0;
    self->entries = self->buffer->data;

    for (size_t i = 0; old_buffer && i < old_buffer->capacity; i++)
    {
        entry_t src = ((entry_t) (old_buffer->data)) + i;
        if (!src || !src->key) continue;

        entry_t dst = hashmap_retrieve(self, src->key);
        ASSERT(dst != NULL, "failed to find empty bucket");

        dst->key   = src->key;
        dst->value = src->value;
        self->size++;
    }

    buffer_delete(old_buffer);
    return self;
}

entry_t hashmap_insert(hashmap_t self, string_t key, uint64_t value)
{
    if (!self || !key || !hashmap_resize(self, self->size + 1))
        return NULL;

    entry_t entry = hashmap_retrieve(self, key);
    ASSERT(entry != NULL, "failed to find bucket");

    if (!entry->key)
    {
        entry->key = string_clone(key);
        self->size++;
    }

    entry->value = value;
    return entry;
}

entry_t hashmap_retrieve(hashmap_t self, string_t key)
{
    if (!self || !key)
        return NULL;

    ASSERT(self->buffer != NULL, "corrupted hashmap");
    const size_t capacity = self->buffer->capacity - 1;

    for (size_t i = string_hash(key)->hash & (capacity);; i = (i + 1) & capacity)
    {
        entry_t entry = &self->entries[i];
        if (!entry->key)                    return entry;
        if (string_cmp_eq(entry->key, key)) return entry;
    }
}

entry_t hashmap_contains(hashmap_t self, string_t key)
{
    entry_t entry = hashmap_retrieve(self, key);
    return (entry && entry->key) ? entry : NULL;
}
