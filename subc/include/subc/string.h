#pragma once

#ifndef SUBC_STRING_H
#define SUBC_STRING_H

#include "subc/common.h"
#include "subc/buffer.h"

// =============================================================================
// string
// =============================================================================
typedef struct string
{
    buffer_t buffer;
    size_t   size;
    char*    chars;
    size_t   hash;
} *string_t;

string_t string_create(const char* format, ...);
string_t string_delete(string_t self);
string_t string_clone (string_t self);
string_t string_hash  (string_t self);
string_t string_format(string_t self, const char* format, va_list args);
string_t string_append(string_t self, const char* format, ...);
string_t string_remove(string_t self, size_t start, size_t count);
bool     string_cmp_eq(string_t self, string_t other);
bool     string_str_eq(string_t self, const char* str);

#endif // SUBC_STRING_H
