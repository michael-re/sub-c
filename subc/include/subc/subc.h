#pragma once

#ifndef SUBC_SUBC_H
#define SUBC_SUBC_H

// =============================================================================
// c headers
// =============================================================================

#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>



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
#include "subc/emitter.h"
#include "subc/codegen.h"



// =============================================================================
// debug macros
// =============================================================================

#ifdef SUBC_DEBUG_MODE
#define DEBUG_OPEN_FILE true
#define DEBUG_TOKENIZE  true
#define DEBUG_PARSE     true
#define DEBUG_ANALYZE   true
#else
#define DEBUG_OPEN_FILE false
#define DEBUG_TOKENIZE  false
#define DEBUG_PARSE     false
#define DEBUG_ANALYZE   false
#endif



// =============================================================================
// function forward decl
// =============================================================================

ifstream_t open_file(const char* name);
tkstream_t tokenize (const char* name, ifstream_t source);
ast_t      parse    (const char* name, tkstream_t source);
symtable_t analyze  (const char* name, ast_t source);

#endif // SUBC_SUBC_H
