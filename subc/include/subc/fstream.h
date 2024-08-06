#pragma once

#ifndef SUBC_FSTREAM_H
#define SUBC_FSTREAM_H

#include "subc/common.h"
#include "subc/buffer.h"

// =============================================================================
// input file stream
// =============================================================================
typedef struct ifstream
{
    buffer_t buffer;
    size_t   size;
    char*    data;
} *ifstream_t;

ifstream_t ifstream_create(const char* filename);
ifstream_t ifstream_delete(ifstream_t self);

// =============================================================================
// output file stream
// =============================================================================
typedef struct ofstream
{
    buffer_t buffer;
    size_t   size;
    char*    data;
} *ofstream_t;

ofstream_t ofstream_create(void);
ofstream_t ofstream_delete(ofstream_t self);
ofstream_t ofstream_append(ofstream_t self, const char* data, size_t count);
size_t     ofstream_fwrite(ofstream_t self, const char* filename);

#endif // SUBC_FSTREAM_H
