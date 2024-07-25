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

#endif // SUBC_FSTREAM_H
