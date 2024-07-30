#pragma once

#ifndef SUBC_TOKEN_H
#define SUBC_TOKEN_H

#include "subc/common.h"
#include "subc/buffer.h"
#include "subc/string.h"

// =============================================================================
// token type
// =============================================================================
typedef enum token_type
{
    // punctuations
    TOK_LEFT_BRACE,   // '{'
    TOK_RIGHT_BRACE,  // '}'
    TOK_LEFT_PAREN,   // '('
    TOK_RIGHT_PAREN,  // ')'
    TOK_SEMICOLON,    // ';'
    TOK_COMMA,        // ','

    // punctuations - assignment
    TOK_EQUAL,        // '='

    // punctuations - arithmetic
    TOK_OP_ADD,       // '+'
    TOK_OP_MUL,       // '*'
    TOK_OP_SUB,       // '-'

    // punctuations - comparison
    TOK_OP_EEQ,       // '=='
    TOK_OP_NEQ,       // '!='
    TOK_OP_GT,        // '>'
    TOK_OP_GE,        // '>='
    TOK_OP_LT,        // '<'
    TOK_OP_LE,        // '<'

    // literals/names/identifiers
    TOK_STRING,       // '"' non_quotes '"'
    TOK_NUMBER,       // [0-9]+
    TOK_NAME,         // ([a-zA-Z] | '_') ([a-zA-Z0-9] | '_')*

    // keywords
    TOK_IF,           // 'if'
    TOK_INT,          // 'int'
    TOK_RETURN,       // 'return'
    TOK_WHILE,        // 'while'

    // other
    TOK_BAD,          // error/bad -- for internal use only
    TOK_EOF,          // end of file
} tok_type;
const char* tok_type_string(tok_type type);
bool        tok_type_is_bop(tok_type type);

// =============================================================================
// token
// =============================================================================
typedef struct token
{
    tok_type type;
    string_t lexeme;
    size_t   line;
    size_t   column;
} *token_t;

token_t token_create(tok_type type, string_t lexeme, size_t line, size_t column);
token_t token_delete(token_t self);
token_t token_clone (token_t self);
token_t token_report(token_t self, const char* header, const char* message);

// =============================================================================
// token stream
// =============================================================================
typedef struct tkstream
{
    buffer_t buffer;
    size_t   size;
    token_t* tokens;
} *tkstream_t;

tkstream_t tkstream_create(void);
tkstream_t tkstream_delete(tkstream_t self);
tkstream_t tkstream_append(tkstream_t self, token_t token);

#endif // SUBC_TOKEN_H
