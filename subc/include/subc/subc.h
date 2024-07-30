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

#include "subc/ast.h"
#include "subc/token.h"
#include "subc/lexer.h"



// =============================================================================
// debug macros
// =============================================================================

#ifdef NDEBUG
#define DEBUG_OPEN_FILE false
#define DEBUG_TOKENIZE  false
#else
#define DEBUG_OPEN_FILE true
#define DEBUG_TOKENIZE  true
#endif



// =============================================================================
// function forward decl
// =============================================================================

ifstream_t open_file(const char* name);
tkstream_t tokenize (const char* name, ifstream_t source);

#endif // SUBC_SUBC_H
