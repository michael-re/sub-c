#include <stdio.h>
#include <string.h>

#include "subc/fstream.h"
#include "subc/memory.h"

// =============================================================================
// input file stream impl
// =============================================================================

ifstream_t ifstream_create(const char* filename)
{
    if (filename == NULL || filename[0] == '\0')
        return NULL;

    FILE* file = fopen(filename, "rb");
    if (!file)
        return NULL;

    if (fseek(file, 0l, SEEK_END) != 0)
    {
        fclose(file);
        return NULL;
    }

    const long   ssize = ftell(file);
    const size_t usize = (size_t) ssize;

    if (ssize == -1l)
    {
        fclose(file);
        return NULL;
    }

    ifstream_t self = NEW(self, 1);
    self->buffer    = buffer_create(sizeof(*self->data), usize + 1);
    self->size      = usize;
    self->data      = self->buffer->data;

    rewind(file);
    const size_t read = fread(self->data, sizeof(*self->data), self->size, file);
    fclose(file);

    return read == self->size ? self : ifstream_delete(self);
}

ifstream_t ifstream_delete(ifstream_t self)
{
    if (self)
    {
        self->buffer = buffer_delete(self->buffer);
        (*self)      = (struct ifstream) { 0 };
    }

    return DELETE(self);
}

// =============================================================================
// output file stream
// =============================================================================

ofstream_t ofstream_create(void)
{
    ofstream_t self = NEW(self, 1);
    self->buffer    = buffer_create(sizeof(*self->data), 4096);
    self->size      = 0;
    self->data      = self->buffer->data;
    return self;
}

ofstream_t ofstream_delete(ofstream_t self)
{
    if (self)
    {
        self->buffer = buffer_delete(self->buffer);
        (*self)      = (struct ofstream) { 0 };
    }

    return DELETE(self);
}

ofstream_t ofstream_append(ofstream_t self, const char* data, size_t count)
{
    if (self && data && count)
    {
        self->data = buffer_expand(self->buffer, (self->size) + (count + 512));
        memmove(self->data + self->size, data, count); //! NOLINT
        self->size += count;
    }

    return self;
}

size_t ofstream_fwrite(ofstream_t self, const char* filename)
{
    if (!self || !self->size || !filename || filename[0] == '\0')
        return 0;

    FILE* file = fopen(filename, "w");
    if (!file)
        return 0;

    const size_t written = fwrite(self->buffer->data, sizeof(*self->data), self->size, file);
    fclose(file);

    return written;
}
