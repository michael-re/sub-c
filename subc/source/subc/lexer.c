#include "subc/lexer.h"
#include "subc/memory.h"

// =============================================================================
// source
// =============================================================================
typedef struct source
{
    char prev;
    char curr;
    char next;
    bool atend;
    bool error;
} source_t;

// =============================================================================
// cursor
// =============================================================================
typedef struct cursor
{
    size_t cursor;
    size_t line;
    size_t column;
} cursor_t;

// =============================================================================
// lexer
// =============================================================================
typedef struct lexer
{
    ifstream_t stream;
    source_t   source;
    cursor_t   start;
    cursor_t   current;
} *lexer_t;

static lexer_t lexer_create          (ifstream_t source);
static lexer_t lexer_delete          (lexer_t self);
static lexer_t lexer_advance         (lexer_t self);
static lexer_t lexer_consume         (lexer_t self, char c);
static token_t lexer_make_error      (lexer_t self, const char* message);
static token_t lexer_make_token      (lexer_t self, tok_type type);
static lexer_t lexer_skip_whitespace (lexer_t self);
static token_t lexer_scan_identifier (lexer_t self);
static token_t lexer_scan_number     (lexer_t self);
static token_t lexer_scan_punctuation(lexer_t self);
static token_t lexer_scan_string     (lexer_t self);
static token_t lexer_scan_token      (lexer_t self);

// =============================================================================
// lexer utility function
// =============================================================================

static inline bool is_alpha(const char c)
{
    return (c == '_')
        || (c >= 'a' && c <= 'z')
        || (c >= 'A' && c <= 'Z');
}

static inline bool is_digit(const char c)
{
    return (c >= '0' && c <= '9');
}

static inline bool is_quote(const char c)
{
    return (c == '"');
}

// =============================================================================
// lexer impl
// =============================================================================

static lexer_t lexer_create(ifstream_t source)
{
    ASSERT(source != NULL, "can't lex null source");
    lexer_t self = NEW(self, 1);
    self->stream = source;

    const size_t size = source->size;
    const char*  data = source->data;

    self->source.prev  = 0x0;
    self->source.curr  = size >= 1 ? data[0] : 0x0;
    self->source.next  = size >= 2 ? data[1] : 0x0;
    self->source.atend = size <= 0;
    self->source.error = false;
    self->start        = (cursor_t) { .cursor = 0, .line = 1, .column = 1 };
    self->current      = (cursor_t) { .cursor = 0, .line = 1, .column = 1 };

    return self;
}

static lexer_t lexer_delete(lexer_t self)
{
    if (self)
    {
        self->stream = ifstream_delete(self->stream);
        (*self)      = (struct lexer) { 0 };
    }

    return DELETE(self);
}

static lexer_t lexer_advance(lexer_t self)
{
    if (self->source.atend)
        return self;

    if (self->source.curr == '\n')
    {
        self->current.line++;\
        self->current.column = 0;
    }

    self->current.cursor++;
    self->current.column++;

    const size_t index = self->current.cursor;
    const size_t size  = self->stream->size;
    const char*  data  = self->stream->data;

    self->source.atend = (index >= size);
    self->source.prev  = self->source.curr;
    self->source.curr  = (index + 0) < size ? data[index + 0] : 0x0;
    self->source.next  = (index + 1) < size ? data[index + 1] : 0x0;

    return self;
}

static lexer_t lexer_consume(lexer_t self, char c)
{
    return (!self->source.atend && self->source.curr == c)
         ? lexer_advance(self)
         : NULL;
}

static token_t lexer_make_error(lexer_t self, const char *message)
{
    ASSERT(message != NULL, "null error message");
    const tok_type type   = TOK_BAD;
    const size_t   line   = self->start.line;
    const size_t   column = self->start.column;

    self->start = self->current;
    self->source.error = true;
    return token_create(type, string_create("%s", message), line, column);
}

static token_t lexer_make_token(lexer_t self, tok_type type)
{
    const size_t line   = self->start.line;
    const size_t column = self->start.column;
    const char*  text   = self->stream->data + self->start.cursor;

    const size_t length = self->current.cursor - self->start.cursor;
    ASSERT(length < INT_MAX, "token text too large");

    self->start = self->current;
    return token_create(type, string_create("%.*s", (int) length, text), line, column);
}

static lexer_t lexer_skip_whitespace(lexer_t self)
{
    for (bool skip = true; skip && !self->source.atend;)
    {
        switch (self->source.curr)
        {
            case ' ':  // space
            case '\f': // form-feed
            case '\n': // new-line
            case '\t': // horizontal tab
            case '\v': // vertical tab
                lexer_advance(self);
                break;
            case '/': // comment
                if (self->source.next == '/')
                {
                    while (!self->source.atend && self->source.curr != '\n')
                        lexer_advance(self);
                    continue;
                }
                skip = false;
                break;
            default:
                skip = false;
                break;
        }
    }

    self->start = self->current;
    return self;
}

static token_t lexer_scan_identifier(lexer_t self)
{
    ASSERT(is_alpha(self->source.curr), "identifier name must start with an alpha character");
    while (is_alpha(self->source.curr) || is_digit(self->source.curr))
        lexer_advance(self);

    token_t token = lexer_make_token(self, TOK_NAME);
         if (string_str_eq(token->lexeme, "if"))     token->type = TOK_IF;
    else if (string_str_eq(token->lexeme, "int"))    token->type = TOK_INT;
    else if (string_str_eq(token->lexeme, "return")) token->type = TOK_RETURN;
    else if (string_str_eq(token->lexeme, "while"))  token->type = TOK_WHILE;

    return token;
}

static token_t lexer_scan_number(lexer_t self)
{
    ASSERT(is_digit(self->source.curr), "number literal must start with a digit");
    while (is_digit(self->source.curr)) lexer_advance(self);
    return lexer_make_token(self, TOK_NUMBER);
}

static token_t lexer_scan_punctuation(lexer_t self)
{
    switch (lexer_advance(self)->source.prev)
    {
        case '-': return lexer_make_token(self, TOK_OP_SUB);
        case ',': return lexer_make_token(self, TOK_COMMA);
        case ';': return lexer_make_token(self, TOK_SEMICOLON);
        case '(': return lexer_make_token(self, TOK_LEFT_PAREN);
        case ')': return lexer_make_token(self, TOK_RIGHT_PAREN);
        case '{': return lexer_make_token(self, TOK_LEFT_BRACE);
        case '}': return lexer_make_token(self, TOK_RIGHT_BRACE);
        case '*': return lexer_make_token(self, TOK_OP_MUL);
        case '+': return lexer_make_token(self, TOK_OP_ADD);
        case '!': return lexer_make_token(self, lexer_consume(self, '=') ? TOK_OP_NEQ : TOK_EQUAL);
        case '<': return lexer_make_token(self, lexer_consume(self, '=') ? TOK_OP_LE  : TOK_OP_LT);
        case '=': return lexer_make_token(self, lexer_consume(self, '=') ? TOK_OP_EEQ : TOK_EQUAL);
        case '>': return lexer_make_token(self, lexer_consume(self, '=') ? TOK_OP_GE  : TOK_OP_GT);
        default:  break;
    }

    token_t error = lexer_make_error(self, "unrecognized punctuation ");
    if (self->source.prev <= 0x20)
        string_append(error->lexeme, "with int value '%d'", (int) self->source.prev);
    else
        string_append(error->lexeme, "'%c'", self->source.prev);

    return error;
}

static token_t lexer_scan_string(lexer_t self)
{
    ASSERT(lexer_consume(self, '"'), "string string literal must start with '\"'");
    while (!self->source.atend && !is_quote(self->source.curr))
        lexer_advance(self);

    if (!lexer_consume(self, '"'))
        return lexer_make_error(self, "unterminated string literal");

    return lexer_make_token(self, TOK_STRING);
}

static token_t lexer_scan_token(lexer_t self)
{
    lexer_skip_whitespace(self);
    if (self->source.atend)          return lexer_make_token     (self, TOK_EOF);
    if (is_alpha(self->source.curr)) return lexer_scan_identifier(self);
    if (is_digit(self->source.curr)) return lexer_scan_number    (self);
    if (is_quote(self->source.curr)) return lexer_scan_string    (self);
    return lexer_scan_punctuation(self);
}

// =============================================================================
// fstream lexer impl
// =============================================================================

tkstream_t lex_tokens(ifstream_t source)
{
    lexer_t    lexer  = lexer_create(source);
    tkstream_t tokens = tkstream_create();

    for (;;)
    {
        token_t token = lexer_scan_token(lexer);
        ASSERT(token != NULL, "something went wrong in lexer_scan_token");

        if (token->type == TOK_BAD)
            token_report(token, "lex error", token->lexeme->chars);

        tkstream_append(tokens, token);
        if (token->type == TOK_EOF) break;
    }

    if (lexer->source.error)
        tokens = tkstream_delete(tokens);

    lexer_delete(lexer);
    return tokens;
}
