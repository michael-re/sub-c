#include <stdio.h>
#include <string.h>

#include "subc/string.h"
#include "subc/memory.h"

// =============================================================================
// string impl
// =============================================================================

string_t string_create(const char* format, ...)
{
    string_t self = NEW(self, 1);
    self->buffer  = buffer_create(sizeof(*self->chars), 0);
    self->size    = 0;
    self->hash    = 0;
    self->chars   = self->buffer->data;

    if (format)
    {
        va_list args;
        va_start(args, format);
        string_format(self, format, args);
        va_end(args);
    }

    return self;
}

string_t string_delete(string_t self)
{
    if (self)
    {
        self->buffer = buffer_delete(self->buffer);
        (*self)      = (struct string) { 0 };
    }

    return DELETE(self);
}

string_t string_hash(string_t self)
{
    ASSERT(self         != NULL, "can't hash null string");
    ASSERT(self->buffer != NULL, "corrupted string buffer");
    ASSERT(self->chars  != NULL, "corrupted string buffer");

    if (self->hashed)
        return self;

    self->hash = 0xcbf29ce484222325;
    for (size_t i = 0; i < self->size; i++)
    {
        const uint8_t character = (uint8_t) self->chars[i];
        ASSERT(character != 0x0, "corrupted string buffer");
        self->hash ^= character;
        self->hash *= 0x100000001b3;
    }

    return self;
}

string_t string_format(string_t self, const char* format, va_list args)
{
    ASSERT(self   != NULL, "can't format append to null string");
    ASSERT(format != NULL, "null format string");

    va_list copy;
    va_copy(copy, args);
    const int format_ssize = vsnprintf(NULL, 0, format, copy); //! NOLINT
    va_end(copy);

    if (format_ssize <= 0) return 0; //! ERROR
    const size_t format_usize = (size_t) format_ssize;
    self->chars = buffer_expand(self->buffer, self->size + format_usize + 2);

    va_copy(copy, args);
    char* const  buffer  = self->chars + self->size;
    const size_t count   = self->buffer->capacity - self->size;
    const int    written = vsnprintf(buffer, count, format, copy); //! NOLINT
    va_end(copy);

    ASSERT(written == format_ssize, "vsnprintf failed when it shouldn't have");
    self->size     += (size_t) written;
    return self;
}

string_t string_append(string_t self, const char* format, ...)
{
    ASSERT(self   != NULL, "can't append to null string");
    ASSERT(format != NULL, "null format string");

    va_list args;
    va_start(args, format);
    string_format(self, format, args);
    va_end(args);

    return self;
}

string_t string_remove(string_t self, size_t start, size_t count)
{
    ASSERT(self  != NULL,       "can't remove characters from null string");
    ASSERT(start <  self->size, "start index out of range");

    size_t cnt = MIN_VALUE(self->size - start, count);
    char*  dst = self->chars + start;
    char*  src = dst + cnt;

    if (cnt && dst && src)
    {
        memmove(dst, src, self->size - cnt - start); //! NOLINT
        self->size      -= cnt;
        self->hashed     = false;
        void*  removed   = self->chars + self->size;
        size_t remaining = self->buffer->capacity - self->size;
        memset(removed, 0x0, remaining); //! NOLINT
    }

    return self;
}

bool string_cmp_eq(string_t self, string_t other)
{
    if (!self && !other) return true;
    if (!self || !other) return false;
    return (self->size = other->size)
        && (string_hash(self)->hash == string_hash(other)->hash)
        && (memcmp(self->chars, other->chars, self->size) == 0);
}

bool string_str_eq(string_t self, const char* str)
{
    if (!self && !str) return true;
    if (!self || !str) return false;

    ASSERT(self->buffer != NULL, "comparing against string with corrupted buffer");
    ASSERT(self->chars  != NULL, "comparing against string with corrupted buffer");
    ASSERT(str          != NULL, "raw string buffer should not be null at this point");
    return self->chars == str || strcmp(self->chars, str) == 0;
}
