#include "subc/parser.h"
#include "subc/memory.h"

// =============================================================================
// parser
// =============================================================================
typedef struct parser
{
    tkstream_t stream;
    size_t     cursor;
    bool       error;
} *parser_t;

static parser_t parser_create          (tkstream_t stream);
static parser_t parser_delete          (parser_t self);
static token_t  parser_current         (parser_t self);
static token_t  parser_advance         (parser_t self);
static token_t  parser_consume         (parser_t self, tok_type type, const char* message);
static token_t  parser_check           (parser_t self, tok_type type);
static token_t  parser_match           (parser_t self, tok_type type);
static ast_t    parser_error           (parser_t self, const char* message);
static ast_t    parser_result          (parser_t self, ast_t ast);
static ast_t    parser_parse_program   (parser_t self);
static ast_t    parser_parse_function  (parser_t self);
static ast_t    parser_parse_parameters(parser_t self);
static ast_t    parser_parse_variable  (parser_t self);
static ast_t    parser_parse_statement (parser_t self);
static ast_t    parser_parse_assign    (parser_t self);
static ast_t    parser_parse_block     (parser_t self);
static ast_t    parser_parse_if        (parser_t self);
static ast_t    parser_parse_return    (parser_t self);
static ast_t    parser_parse_while     (parser_t self);
static ast_t    parser_parse_expression(parser_t self);
static ast_t    parser_parse_primary   (parser_t self);
static ast_t    parser_parse_call      (parser_t self);
static ast_t    parser_parse_arg       (parser_t self);
static ast_t    parser_parse_number    (parser_t self);
static ast_t    parser_parse_string    (parser_t self);
static ast_t    parser_parse_name      (parser_t self);

// =============================================================================
// parser impl
// =============================================================================

static parser_t parser_create(tkstream_t stream)
{
    ASSERT(stream       != NULL, "can't parse null token stream");
    ASSERT(stream->size != 0,    "can't parse empty token stream");

    const size_t last = stream->size - 1;
    ASSERT(stream->tokens[last]       != NULL,    "corrupted token stream");
    ASSERT(stream->tokens[last]->type == TOK_EOF, "invalid token stream");

    parser_t self = NEW(self, 1);
    self->stream  = stream;
    self->cursor  = 0;
    self->error   = false;
    return self;
}

static parser_t parser_delete(parser_t self)
{
    if (self)
    {
        self->stream = tkstream_delete(self->stream);
        (*self)      = (struct parser) { 0 };
    }

    return DELETE(self);
}

static token_t parser_current(parser_t self)
{
    ASSERT(self         != NULL, "can't peek into null parser");
    ASSERT(self->stream != NULL, "corrupted parser");

    const size_t last  = self->stream->size - 1;
    const size_t index = self->cursor;
    ASSERT(index <= last, "invalid peek");

    token_t token = self->stream->tokens[index];
    ASSERT(token != NULL, "corrupted token stream");

    return token;
}

static token_t parser_advance(parser_t self)
{
    if (!parser_check(self, TOK_EOF)) self->cursor++;
    return parser_current(self);
}

static token_t parser_consume(parser_t self, tok_type type, const char* message)
{
    token_t consumed = parser_match(self, type);
    if (!consumed) parser_error(self, message);
    return consumed;
}

static token_t parser_check(parser_t self, tok_type type)
{
    token_t current = parser_current(self);
    return current->type == type ? current : NULL;
}

static token_t parser_match(parser_t self, tok_type type)
{
    token_t matched = parser_check(self, type);
    if (matched) parser_advance(self);
    return matched;
}

static ast_t parser_error(parser_t self, const char* message)
{
    ASSERT(self    != NULL, "error in null parser???");
    ASSERT(message != NULL, "null error message");

    self->error = true;
    token_report(parser_current(self), "parse error", message);
    parser_advance(self);
    return NULL;
}

static ast_t parser_result(parser_t self, ast_t ast)
{
    return self && !self->error ? ast : ast_delete(ast);
}

static ast_t parser_parse_program(parser_t self)
{
    ast_t functions = ast_list_create();
    while (!parser_check(self, TOK_EOF))
        ast_list_append(functions, parser_parse_function(self));

    parser_consume(self, TOK_EOF, "expect end of file");
    return parser_result(self, functions);
}

static ast_t parser_parse_function(parser_t self)
{
    parser_consume(self, TOK_INT, "expect function return type");
    ast_t name = parser_parse_name(self);
    ast_t pars = parser_parse_parameters(self);

    parser_consume(self, TOK_LEFT_BRACE, "expect '{' before function body");
    ast_t vars = ast_list_create();
    while (parser_check(self, TOK_INT))
        ast_list_append(vars, parser_parse_variable(self));

    ast_t stmts = ast_list_create();
    while (!parser_check(self, TOK_EOF) && !parser_check(self, TOK_RIGHT_BRACE))
        ast_list_append(stmts, parser_parse_statement(self));

    parser_consume(self, TOK_RIGHT_BRACE, "expect '}' after function body");
    return parser_result(self, ast_func_create(name, pars, vars, stmts));
}

static ast_t parser_parse_parameters(parser_t self)
{
    ast_t pars = ast_list_create();
    parser_consume(self, TOK_LEFT_PAREN, "expect '(' before parameters");

    while (!parser_check(self, TOK_EOF) && !parser_check(self, TOK_RIGHT_PAREN))
    {
        parser_consume(self, TOK_INT, "expect parameter type");
        ast_list_append(pars, parser_parse_name(self));
        if (!parser_match(self, TOK_COMMA)) break;
    }

    parser_consume(self, TOK_RIGHT_PAREN, "expect ')' after parameters");
    return parser_result(self, pars);
}

static ast_t parser_parse_variable(parser_t self)
{
    parser_consume(self, TOK_INT, "expect variable type");
    ast_t name = parser_parse_name(self);
    parser_consume(self, TOK_SEMICOLON, "expect ';' after variable declaration");
    return parser_result(self, name);
}

static ast_t parser_parse_statement(parser_t self)
{
    switch (parser_current(self)->type)
    {
        case TOK_IF:         return parser_parse_if    (self);
        case TOK_LEFT_BRACE: return parser_parse_block  (self);
        case TOK_NAME:       return parser_parse_assign (self);
        case TOK_RETURN:     return parser_parse_return(self);
        case TOK_WHILE:      return parser_parse_while (self);
        default:             return parser_error(self, "expect statement");
    }
}

static ast_t parser_parse_assign(parser_t self)
{
    token_t name = parser_consume(self, TOK_NAME, "expect variable name");
    parser_consume(self, TOK_EQUAL, "expect '=' after variable name");

    const size_t cursor = self->cursor;
    parser_advance(self); // name
    const bool   call   = parser_check(self, TOK_LEFT_PAREN);

    self->cursor = cursor;
    ast_t value  = call ? parser_parse_call(self) : parser_parse_expression(self);

    parser_consume(self, TOK_SEMICOLON, "expect ';' after assign value");
    return parser_result(self, ast_assign_create(name, value));
}

static ast_t parser_parse_block(parser_t self)
{
    ast_t stmts = ast_list_create();
    parser_consume(self, TOK_LEFT_BRACE, "expect '{' before block");

    while (!parser_check(self, TOK_EOF) && !parser_check(self, TOK_RIGHT_BRACE))
        ast_list_append(stmts, parser_parse_statement(self));

    parser_consume(self, TOK_RIGHT_BRACE, "expect ';' after block");
    return parser_result(self, stmts);
}

static ast_t parser_parse_if(parser_t self)
{
    parser_consume(self, TOK_IF, "expect 'if' keyword");
    parser_consume(self, TOK_LEFT_PAREN,  "expect '(' before 'if' condition");
    ast_t expr  = parser_parse_expression(self);
    parser_consume(self, TOK_RIGHT_PAREN, "expect ')' after 'if' condition");
    ast_t block = parser_parse_block(self);
    return parser_result(self, ast_if_create(expr, block));
}

static ast_t parser_parse_return(parser_t self)
{
    parser_consume(self, TOK_RETURN, "expect 'return' keyword");
    ast_t expr = parser_parse_expression(self);
    parser_consume(self, TOK_SEMICOLON, "expect ';' after 'return' expression");
    return parser_result(self, ast_return_create(expr));
}

static ast_t parser_parse_while(parser_t self)
{
    parser_consume(self, TOK_WHILE, "expect 'while' keyword");
    parser_consume(self, TOK_LEFT_PAREN,  "expect '(' before 'while' condition");
    ast_t expr  = parser_parse_expression(self);
    parser_consume(self, TOK_RIGHT_PAREN, "expect ')' after 'while' condition");
    ast_t block = parser_parse_block(self);
    return parser_result(self, ast_while_create(expr, block));
}

static ast_t parser_parse_expression(parser_t self)
{
    ast_t   lhs = parser_parse_primary(self);
    token_t bop = parser_current(self);

    if (tok_type_is_bop(bop->type))
    {
        parser_advance(self);
        ast_t rhs = parser_parse_primary(self);
        return parser_result(self, ast_binary_create(bop, lhs, rhs));
    }

    return parser_result(self, lhs);
}

static ast_t parser_parse_primary(parser_t self)
{
    switch (parser_current(self)->type)
    {
        case TOK_NAME:   return parser_parse_name  (self);
        case TOK_NUMBER: return parser_parse_number(self);
        default:         return parser_error       (self, "expect primary expression");
    }
}

static ast_t parser_parse_call(parser_t self)
{
    ast_t name = parser_parse_name(self);
    ast_t args = ast_list_create();

    parser_consume(self, TOK_LEFT_PAREN, "expect '(' before args");
    while (!parser_check(self, TOK_EOF) && !parser_check(self, TOK_RIGHT_PAREN))
    {
        ast_list_append(args, parser_parse_arg(self));
        if (!parser_match(self, TOK_COMMA)) break;
    }
    parser_consume(self, TOK_RIGHT_PAREN, "expect ')' after args");

    return parser_result(self, ast_call_create(name, args));
}

static ast_t parser_parse_arg(parser_t self)
{
    switch (parser_current(self)->type)
    {
        case TOK_NAME:   return parser_parse_name  (self);
        case TOK_NUMBER: return parser_parse_number(self);
        case TOK_STRING: return parser_parse_string(self);
        default:         return parser_error(self, "expect name, number, or string");
    }
}

static ast_t parser_parse_number(parser_t self)
{
    token_t number = parser_consume(self, TOK_NUMBER, "expect number literal");
    return parser_result(self, ast_number_create(number));
}

static ast_t parser_parse_string(parser_t self)
{
    token_t string = parser_consume(self, TOK_STRING, "expect string literal");
    return parser_result(self, ast_string_create(string));
}

static ast_t parser_parse_name(parser_t self)
{
    token_t name = parser_consume(self, TOK_NAME, "expect identifier name");
    return parser_result(self, ast_name_create(name));
}

// =============================================================================
// program parser impl
// =============================================================================

ast_t parse_program(tkstream_t stream)
{
    parser_t parser = parser_create(stream);
    ast_t    ast    = parser_parse_program(parser);

    ast = parser_result(parser, ast);
    parser_delete(parser);

    return ast;
}
