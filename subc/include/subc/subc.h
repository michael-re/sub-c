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
#include "subc/hashmap.h"

#include "subc/ast.h"
#include "subc/token.h"
#include "subc/lexer.h"
#include "subc/table.h"
#include "subc/parser.h"
#include "subc/printer.h"


// =============================================================================
// debug macros
// =============================================================================

#ifdef NDEBUG
#define DEBUG_OPEN_FILE false
#define DEBUG_TOKENIZE  false
#define DEBUG_PARSE     false
#else
#define DEBUG_OPEN_FILE true
#define DEBUG_TOKENIZE  true
#define DEBUG_PARSE     true
#endif



// =============================================================================
// function forward decl
// =============================================================================

ifstream_t open_file(const char* name);
tkstream_t tokenize (const char* name, ifstream_t source);
ast_t      parse    (const char* name, tkstream_t source);
symtable_t analyze  (const char* name, ast_t source);

#endif // SUBC_SUBC_H
