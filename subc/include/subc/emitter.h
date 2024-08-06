#pragma once

#ifndef SUBC_EMITTER_H
#define SUBC_EMITTER_H

#include "subc/common.h"
#include "subc/string.h"

// =============================================================================
// arm v7 emitter
// =============================================================================
typedef struct emitter
{
    string_t code_buffer;
    string_t data_buffer;
    uint64_t curr_label;
} *emitter_t;

emitter_t emitter_create         (void);
emitter_t emitter_delete         (emitter_t self);
emitter_t emitter_emit_opcode    (emitter_t self, const char* opcode, const char* format, ...);
emitter_t emitter_emit_string    (emitter_t self, const char* text);
emitter_t emitter_emit_code      (emitter_t self, const char* format, ...);
emitter_t emitter_emit_data      (emitter_t self, const char* format, ...);
emitter_t emitter_emit_code_label(emitter_t self, string_t label);
emitter_t emitter_emit_data_label(emitter_t self, string_t label);
string_t  emitter_make_label     (emitter_t self);
bool      emitter_save           (emitter_t self, const char* output);

#endif // SUBC_EMITTER_H
