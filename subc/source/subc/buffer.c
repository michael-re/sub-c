#include "subc/buffer.h"
#include "subc/memory.h"

// =============================================================================
// buffer utility functions
// =============================================================================

static inline size_t capacity_grow(const size_t capacity, const size_t min_capacity)
{
    if (capacity >= min_capacity)
        return capacity;

    size_t new_capacity = 32;
    while (new_capacity <= min_capacity)
        new_capacity *= 2;

    return new_capacity;
}

static inline void* buffer_resize(buffer_t self, size_t new_capacity)
{
    ASSERT(self != NULL, "can't resize null buffer");
    if (self->capacity != new_capacity)
    {
        const size_t old_count = self->capacity * self->element_size;
        const size_t new_count = new_capacity   * self->element_size;

        self->bytes_allocated = new_count;
        self->capacity        = new_capacity;
        self->data            = NEW_RESIZE(((char*) self->data), old_count, new_count);
    }

    return self->data;
}

// =============================================================================
// buffer impl
// =============================================================================

buffer_t buffer_create(size_t element_size, size_t initial_capacity)
{
    ASSERT(element_size != 0, "buffer should never hold a zero-sized element");
    buffer_t self         = NEW(self, 1);
    self->bytes_allocated = 0;
    self->element_size    = element_size;
    self->capacity        = 0;

    const size_t capacity = MAX_VALUE(initial_capacity, 1);
    buffer_expand(self, capacity_grow(self->capacity, capacity));
    ASSERT(self->capacity >= initial_capacity, "invalid new buffer capacity");
    ASSERT(self->data     != NULL,             "buffer data/capacity mismatch");

    return self;
}

buffer_t buffer_delete(buffer_t self)
{
    if (self)
    {
        self->data = DELETE(self->data);
        (*self)    = (struct buffer) { 0 };
    }

    return DELETE(self);
}

void* buffer_expand(buffer_t self, size_t min_capacity)
{
    ASSERT(self != NULL, "can't expand null buffer");
    const size_t old_capacity = self ? self->capacity : 0;
    const size_t new_capacity = capacity_grow(old_capacity, min_capacity);
    return buffer_resize(self, new_capacity);
}

void* buffer_shrink(buffer_t self, size_t max_capacity)
{
    ASSERT(self != NULL, "can't shrink null buffer");
    const size_t old_capacity = self ? self->capacity : 0;
    const size_t new_capacity = MIN_VALUE(old_capacity, max_capacity);
    return buffer_resize(self, new_capacity);
}
