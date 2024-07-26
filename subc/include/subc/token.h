#pragma once

#ifndef SUBC_TOKEN_H
#define SUBC_TOKEN_H

#include "subc/common.h"

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

#endif // SUBC_TOKEN_H
