#pragma once

#ifndef SUBC_SUBC_H
#define SUBC_SUBC_H

// =============================================================================
// c headers
// =============================================================================

#include <stdio.h>
#include <stdlib.h>



// =============================================================================
// sub-c headers
// =============================================================================

#include "subc/common.h"
#include "subc/memory.h"
#include "subc/buffer.h"
#include "subc/fstream.h"



// =============================================================================
// debug macros
// =============================================================================

#ifdef NDEBUG
#define DEBUG_OPEN_FILE false
#else
#define DEBUG_OPEN_FILE true
#endif

ifstream_t open_file(const char* name);

#endif // SUBC_SUBC_H
