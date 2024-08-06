#include <stdio.h>
#include <string.h>

#include "subc/emitter.h"
#include "subc/fstream.h"
#include "subc/memory.h"

// =============================================================================
// arm v7 emitter impl
// =============================================================================

emitter_t emitter_create(void)
{
    emitter_t self    = NEW(self, 1);
    self->code_buffer = string_create(NULL);
    self->data_buffer = string_create(NULL);
    self->curr_label  = 0;
    emitter_emit_code(self, "        %s", ".text");
    emitter_emit_data(self, "        %s", ".data");
    return self;
}

emitter_t emitter_delete(emitter_t self)
{
    if (self)
    {
        self->code_buffer = string_delete(self->code_buffer);
        self->data_buffer = string_delete(self->data_buffer);
        (*self)           = (struct emitter) { 0 };
    }

    return DELETE(self);
}

emitter_t emitter_emit_opcode(emitter_t self, const char* opcode, const char* format, ...)
{
    ASSERT(self && opcode && format, "invalid args");
    string_append(self->code_buffer, "%8s%-8s", "", opcode);
    va_list args;
    va_start(args, format);
    string_format(self->code_buffer, format, args);
    va_end(args);
    string_append(self->code_buffer, "\n");
    return self;
}

emitter_t emitter_emit_string(emitter_t self, const char* text)
{
    ASSERT(self && text, "invalid args");
    string_append(self->data_buffer, "%8s%-8s", "", ".asciz");
    string_append(self->data_buffer, "%s\n", text);
    return self;
}

emitter_t emitter_emit_code(emitter_t self, const char* format, ...)
{
    ASSERT(self && format, "invalid args");
    va_list args;
    va_start(args, format);
    string_format(self->code_buffer, format, args);
    string_append(self->code_buffer, "\n");
    va_end(args);
    return self;
}

emitter_t emitter_emit_data(emitter_t self, const char* format, ...)
{
    ASSERT(self && format, "invalid args");
    va_list args;
    va_start(args, format);
    string_format(self->data_buffer, format, args);
    string_append(self->data_buffer, "\n");
    va_end(args);
    return self;
}

emitter_t emitter_emit_code_label(emitter_t self, string_t label)
{
    ASSERT(self && label, "invalid args");
    emitter_emit_code(self, "%s:", label->chars);
    return self;
}

emitter_t emitter_emit_data_label(emitter_t self, string_t label)
{
    ASSERT(self && label, "invalid args");
    emitter_emit_data(self, "%s:", label->chars);
    return self;
}

string_t emitter_make_label(emitter_t self)
{
    ASSERT(self, "invalid args");
    return string_create("__l%zd", ++self->curr_label);
}

bool emitter_save(emitter_t self, const char* output)
{
    if (self == NULL)
        return false;

    const char* out  = output ? output : "a";
    const char* wack = strrchr(out, '\\');
    if (!wack) wack  = strrchr(out, '/');
    if (!wack) wack  = out;

    while (wack[0] == '\\' || wack[0] == '/') wack++;
    string_t name = string_create("%s", wack);

    const char* dot = strrchr(name->chars, '.');
    if (dot)
    {
        const char*  start  = name->chars;
        const size_t offset = (size_t) (((uintptr_t) dot) - (uintptr_t) start);
        string_remove(name, offset, name->size);
    }

    if (!name->size)
        string_append(name, "a");
    string_append(name, ".s");

    ofstream_t file = ofstream_create();
    ofstream_append(file, self->data_buffer->chars, self->data_buffer->size);
    ofstream_append(file, self->code_buffer->chars, self->code_buffer->size);

    const size_t written = ofstream_fwrite(file, name->chars);
    const bool   success = written == file->size;

    string_delete  (name);
    emitter_delete (self);
    ofstream_delete(file);

    return success;
}
