#pragma once

#ifndef SUBC_AST_H
#define SUBC_AST_H

#include "subc/common.h"
#include "subc/buffer.h"
#include "subc/string.h"
#include "subc/token.h"

// =============================================================================
// ast node type
// =============================================================================
typedef enum ast_type
{
    AST_ASSIGN,
    AST_BINARY,
    AST_CALL,
    AST_FUNC,
    AST_IF,
    AST_LIST,
    AST_NAME,
    AST_NUMBER,
    AST_RETURN,
    AST_STRING,
    AST_WHILE,
} ast_type;
const char* ast_type_string(ast_type type);

// =============================================================================
// base ast node
// =============================================================================
typedef struct ast
{
    ast_type type;
} base_t, *ast_t;
ast_t ast_delete(ast_t self);

// =============================================================================
// ast list: collection of ast nodes
// =============================================================================
typedef struct ast_list
{
    base_t   base;
    buffer_t buff;
    size_t   size;
    ast_t*   nodes;
} *ast_list;
ast_t ast_list_create(void);
ast_t ast_list_append(ast_t self, ast_t ast);

// =============================================================================
// name => alpha alpha_num* (some identifier, eg: variable, function, etc...)
// =============================================================================
typedef struct ast_name
{
    base_t  base;
    token_t token;
} *ast_name;
ast_t ast_name_create(token_t token);

// =============================================================================
// assign => name "=" ( expression | call ) ";"
// =============================================================================
typedef struct ast_assign
{
    base_t  base;
    token_t name;
    ast_t   value;
} *ast_assign;
ast_t ast_assign_create(token_t name, ast_t value);

// =============================================================================
// binary => primary operator primary
// =============================================================================
typedef struct ast_binary
{
    base_t  base;
    token_t bop;
    ast_t   lhs;
    ast_t   rhs;
} *ast_binary;
ast_t ast_binary_create(token_t bop, ast_t lhs, ast_t rhs);

// =============================================================================
// call => name "(" args ")"
// =============================================================================
typedef struct ast_call
{
    base_t   base;
    ast_name name;
    ast_list args;
} *ast_call;
ast_t ast_call_create(ast_t name, ast_t args);

// =============================================================================
// function => "int" name "(" parameters ")" body
// =============================================================================
typedef struct ast_func
{
    base_t   base;
    ast_name name;
    ast_list pars;
    ast_list vars;
    ast_list stmts;
} *ast_func;
ast_t ast_func_create(ast_t name, ast_t pars, ast_t vars, ast_t stmts);

// =============================================================================
// if => "if" "(" expression ")" block
// =============================================================================
typedef struct ast_if
{
    base_t base;
    ast_t  expr;
    ast_t  block;
} *ast_if;
ast_t ast_if_create(ast_t expr, ast_t block);

// =============================================================================
// number => [0-9]+ (number liter, eg: 999)
// =============================================================================
typedef struct ast_number
{
    base_t   base;
    token_t  token;
    uint64_t value;
} *ast_number;
ast_t ast_number_create(token_t token);

// =============================================================================
// return => "return" expression ";"
// =============================================================================
typedef struct ast_return
{
    base_t base;
    ast_t  expr;
} *ast_return;
ast_t ast_return_create(ast_t expr);

// =============================================================================
// string => "\"" non_quotes "\"" (some string literal, eg: "hello world")
// =============================================================================
typedef struct ast_string
{
    base_t   base;
    token_t  token;
    string_t value;
} *ast_string;
ast_t ast_string_create(token_t token);

// =============================================================================
// while => "while" "(" expression ")" block
// =============================================================================
typedef struct ast_while
{
    base_t base;
    ast_t  expr;
    ast_t  block;
} *ast_while;
ast_t ast_while_create(ast_t expr, ast_t block);

#endif // SUBC_AST_H
