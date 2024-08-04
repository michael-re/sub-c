#pragma once

#ifndef SUBC_TABLE_H
#define SUBC_TABLE_H

#include "subc/common.h"
#include "subc/buffer.h"
#include "subc/string.h"
#include "subc/hashmap.h"
#include "subc/token.h"
#include "subc/ast.h"

typedef struct layout   *layout_t;
typedef struct symtable *symtable_t;

// =============================================================================
// symbol type: types for variables, parameters, and functions
// =============================================================================
typedef enum symbol_type
{
    TYP_UNK,    // unknown type - error
    TYP_INT,    // int
    TYP_STR,    // string
} sym_type;
const char* sym_type_string(sym_type type);

// =============================================================================
// symbol role: the role of each identifier in the layout table
// =============================================================================
typedef enum symbol_role
{
    ROL_UNK,    // unknown type - error
    ROL_PAR,    // parameter
    ROL_VAR,    // variable
} sym_role;
const char* sym_role_string(sym_role role);

// =============================================================================
// symbol in layout table
// =============================================================================
typedef struct symbol
{
    sym_type type;
    sym_role role;
    token_t  name;
    int64_t  offset;
    size_t   index;
} *symbol_t;

// =============================================================================
// function stack frame layout
// =============================================================================
typedef struct layout
{
    buffer_t  buffer;
    hashmap_t indicies;
    ast_func  function;
    symbol_t  symbols;
    size_t    offset;
} *layout_t;

layout_t layout_create(symtable_t ctx, ast_func function);
layout_t layout_delete(layout_t self);

// =============================================================================
// symbol table (collection of layout tables)
// =============================================================================
typedef struct symtable
{
    buffer_t  buffer;
    hashmap_t indicies;
    layout_t* layouts;
    ast_list  functions;
    ast_list  intrinsics;
} *symtable_t;

symtable_t symtable_create    (ast_t program);
symtable_t symtable_delete    (symtable_t self);
layout_t   symtable_get_layout(symtable_t self, string_t function);
symbol_t   symtable_get_symbol(symtable_t self, string_t function, string_t name);

#endif // SUBC_TABLE_H
