#include "subc/token.h"
#include "subc/memory.h"

// =============================================================================
// token type impl
// =============================================================================

const char* tok_type_string(tok_type type)
{
    switch (type)
    {
        case TOK_LEFT_BRACE:  return "TOK_LEFT_BRACE";
        case TOK_RIGHT_BRACE: return "TOK_RIGHT_BRACE";
        case TOK_LEFT_PAREN:  return "TOK_LEFT_PAREN";
        case TOK_RIGHT_PAREN: return "TOK_RIGHT_PAREN";
        case TOK_SEMICOLON:   return "TOK_SEMICOLON";
        case TOK_COMMA:       return "TOK_COMMA";
        case TOK_EQUAL:       return "TOK_EQUAL";
        case TOK_OP_ADD:      return "TOK_OP_ADD";
        case TOK_OP_MUL:      return "TOK_OP_MUL";
        case TOK_OP_SUB:      return "TOK_OP_SUB";
        case TOK_OP_EEQ:      return "TOK_OP_EEQ";
        case TOK_OP_NEQ:      return "TOK_OP_NEQ";
        case TOK_OP_GT:       return "TOK_OP_GT";
        case TOK_OP_GE:       return "TOK_OP_GE";
        case TOK_OP_LT:       return "TOK_OP_LT";
        case TOK_OP_LE:       return "TOK_OP_LE";
        case TOK_STRING:      return "TOK_STRING";
        case TOK_NUMBER:      return "TOK_NUMBER";
        case TOK_NAME:        return "TOK_NAME";
        case TOK_IF:          return "TOK_IF";
        case TOK_INT:         return "TOK_INT";
        case TOK_RETURN:      return "TOK_RETURN";
        case TOK_WHILE:       return "TOK_WHILE";
        case TOK_BAD:         return "TOK_BAD";
        case TOK_EOF:         return "TOK_EOF";
        default:              break;
    }

    return FATAL("unknown token type");
}

// =============================================================================
// token impl
// =============================================================================

token_t token_create(tok_type type, string_t lexeme, size_t line, size_t column)
{
    ASSERT(lexeme != NULL, "can't create token with empty lexeme");
    token_t self = NEW(self, 1);
    self->type   = type;
    self->lexeme = lexeme;
    self->line   = line;
    self->column = column;
    return self;
}

token_t token_delete(token_t self)
{
    if (self)
    {
        self->lexeme = string_delete(self->lexeme);
        (*self)      = (struct token) { 0 };
    }

    return DELETE(self);
}
