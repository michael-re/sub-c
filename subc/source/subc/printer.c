#include <stdio.h>

#include "subc/printer.h"
#include "subc/memory.h"
#include "subc/string.h"

// =============================================================================
// printer
// =============================================================================
typedef string_t printer_t;

static printer_t printer_create      (void);
static printer_t printer_delete      (printer_t self);
static printer_t printer_indent      (printer_t self);
static printer_t printer_dedent      (printer_t self);
static printer_t printer_print_header(printer_t self, const char* header, ...);
static printer_t printer_print_footer(printer_t self);
static printer_t printer_print       (printer_t self, const char* format, ...);
static printer_t printer_vprint      (printer_t self, const char* format, va_list args);
static printer_t printer_print_block (printer_t self, const char* header, ast_t ast);
static printer_t printer_print_ast   (printer_t self, ast_t ast);
static printer_t printer_print_assign(printer_t self, ast_t ast);
static printer_t printer_print_binary(printer_t self, ast_t ast);
static printer_t printer_print_call  (printer_t self, ast_t ast);
static printer_t printer_print_func  (printer_t self, ast_t ast);
static printer_t printer_print_if    (printer_t self, ast_t ast);
static printer_t printer_print_list  (printer_t self, ast_t ast);
static printer_t printer_print_name  (printer_t self, ast_t ast);
static printer_t printer_print_number(printer_t self, ast_t ast);
static printer_t printer_print_return(printer_t self, ast_t ast);
static printer_t printer_print_string(printer_t self, ast_t ast);
static printer_t printer_print_while (printer_t self, ast_t ast);

// =============================================================================
// printer impl
// =============================================================================

static printer_t printer_create(void)
{
    return string_create(NULL);
}

static printer_t printer_delete(printer_t self)
{
    return string_delete(self);
}

static printer_t printer_indent(printer_t self)
{
    for (size_t i = 0; i < 4; i++)
        string_append(self, " ");
    return self;
}

static printer_t printer_dedent(printer_t self)
{
    for (size_t i = 0; i < 4; i++)
        string_remove(self, self->size - 1, 1);
    return self;
}

static printer_t printer_print_header(printer_t self, const char* header, ...)
{
    ASSERT(self && header, "invalid args");

    va_list args;
    va_start(args, header);
    printer_vprint(self, header, args);
    va_end(args);

    printer_print(self, "{");
    printer_indent(self);
    return self;
}

static printer_t printer_print_footer(printer_t self)
{
    printer_dedent(self);
    printer_print(self, "}");
    return self;
}

static printer_t printer_print(printer_t self, const char* format, ...)
{
    ASSERT(self && format, "invalid args");
    va_list args;
    va_start(args, format);
    printer_vprint(self, format, args);
    va_end(args);
    return self;
}

static printer_t printer_vprint(printer_t self, const char* format, va_list args)
{
    ASSERT(self && format, "invalid args");
    printf("%s", self->chars);

    va_list copy;
    va_copy(copy, args);
    vprintf(format, copy);
    va_end(copy);

    printf("\n");
    fflush(stdout);
    return self;
}

static printer_t printer_print_block(printer_t self, const char* header, ast_t ast)
{
    printer_print_header(self, "%s", header);
    printer_print_ast   (self, ast);
    printer_print_footer(self);
    return self;
}

static printer_t printer_print_ast(printer_t self, ast_t ast)
{
    ASSERT(self != NULL, "printer should never be null");
    ASSERT(ast  != NULL, "can't print null ast");

    switch (ast->type)
    {
        case AST_ASSIGN: return printer_print_assign(self, ast);
        case AST_BINARY: return printer_print_binary(self, ast);
        case AST_CALL:   return printer_print_call  (self, ast);
        case AST_FUNC:   return printer_print_func  (self, ast);
        case AST_IF:     return printer_print_if    (self, ast);
        case AST_LIST:   return printer_print_list  (self, ast);
        case AST_NAME:   return printer_print_name  (self, ast);
        case AST_NUMBER: return printer_print_number(self, ast);
        case AST_RETURN: return printer_print_return(self, ast);
        case AST_STRING: return printer_print_string(self, ast);
        case AST_WHILE:  return printer_print_while (self, ast);
        default:         break;
    }

    return FATAL("unknown ast type");
}

static printer_t printer_print_assign(printer_t self, ast_t ast)
{
    ASSERT(self && ast && ast->type == AST_ASSIGN, "invalid args");
    ast_assign assign = ANY_CAST(ast);
    printer_print_header(self, "Assign");

    printer_print_header(self, "Variable");
    printer_print       (self, "nam = %s", assign->name->lexeme->chars);
    printer_print       (self, "typ = int");
    printer_print_footer(self);

    printer_print_header(self, "Value");
    printer_print_ast   (self, assign->value);
    printer_print_footer(self);

    printer_print_footer(self);
    return self;
}

static printer_t printer_print_binary(printer_t self, ast_t ast)
{
    ASSERT(self && ast && ast->type == AST_BINARY, "invalid args");
    ast_binary binary = ANY_CAST(ast);
    printer_print_header(self, "Binary");

    printer_print_header(self, "Operator");
    printer_print       (self, "bop = %s", binary->bop->lexeme->chars);
    printer_print       (self, "typ = %s", tok_type_string(binary->bop->type));
    printer_print_footer(self);

    printer_print_header(self, "Left");
    printer_print_ast   (self, binary->lhs);
    printer_print_footer(self);

    printer_print_header(self, "Right");
    printer_print_ast   (self, binary->rhs);
    printer_print_footer(self);

    printer_print_footer(self);
    return self;
}

static printer_t printer_print_call(printer_t self, ast_t ast)
{
    ASSERT(self && ast && ast->type == AST_CALL, "invalid args");
    ast_call call = ANY_CAST(ast);
    printer_print_header(self, "Call");
    printer_print_ast   (self, ANY_CAST(call->name));
    printer_print_block (self, "Arguments", ANY_CAST(call->args));
    printer_print_footer(self);
    return self;
}

static printer_t printer_print_func(printer_t self, ast_t ast)
{
    ASSERT(self && ast && ast->type == AST_FUNC, "invalid args");
    ast_func func = ANY_CAST(ast);
    printer_print_header(self, "Function");

    printer_print_ast   (self, ANY_CAST(func->name));
    printer_print_block (self, "Parameters", ANY_CAST(func->pars));

    printer_print_header(self, "Body");
    printer_print_block (self, "Variables",  ANY_CAST(func->vars));
    printer_print_block (self, "Statements", ANY_CAST(func->stmts));
    printer_print_footer(self);

    printer_print_footer(self);
    return self;
}

static printer_t printer_print_if(printer_t self, ast_t ast)
{
    ASSERT(self && ast && ast->type == AST_IF, "invalid args");
    ast_if if_stmt = ANY_CAST(ast);
    printer_print_header(self, "If");
    printer_print_block (self, "Condition", if_stmt->expr);
    printer_print_block (self, "Block", ANY_CAST(if_stmt->block));
    printer_print_footer(self);
    return self;
}

static printer_t printer_print_list(printer_t self, ast_t ast)
{
    ASSERT(self && ast && ast->type == AST_LIST, "invalid args");
    ast_list list = ANY_CAST(ast);
    for (size_t i = 0; i < list->size; i++)
        printer_print_ast(self, list->nodes[i]);
    return self;
}

static printer_t printer_print_name(printer_t self, ast_t ast)
{
    ASSERT(self && ast && ast->type, "invalid args");
    ast_name name = ANY_CAST(ast);
    printer_print_header(self, "Identifier");
    printer_print       (self, "nam = %s", name->token->lexeme->chars);
    printer_print       (self, "typ = int");
    printer_print_footer(self);
    return self;
}

static printer_t printer_print_number(printer_t self, ast_t ast)
{
    ASSERT(self && ast && ast->type == AST_NUMBER, "invalid args");
    ast_number number = ANY_CAST(ast);
    printer_print_header(self, "Number");
    printer_print       (self, "val = %s", number->token->lexeme->chars);
    printer_print       (self, "typ = int");
    printer_print_footer(self);
    return self;
}

static printer_t printer_print_return(printer_t self, ast_t ast)
{
    ASSERT(self && ast && ast->type == AST_RETURN, "invalid args");
    ast_return ret = ANY_CAST(ast);
    printer_print_header(self, "Return");
    printer_print_ast   (self, ret->expr);
    printer_print_footer(self);
    return self;
}

static printer_t printer_print_string(printer_t self, ast_t ast)
{
    ASSERT(self && ast && ast->type == AST_STRING, "invalid args");
    ast_string string = ANY_CAST(ast);
    printer_print_header(self, "String");
    printer_print       (self, "val = %s", string->token->lexeme->chars);
    printer_print       (self, "typ = char*");
    printer_print_footer(self);
    return self;
}

static printer_t printer_print_while(printer_t self, ast_t ast)
{
    ASSERT(self && ast && ast->type == AST_WHILE, "invalid args");
    ast_while while_stmt = ANY_CAST(ast);
    printer_print_header(self, "While");
    printer_print_block (self, "Condition", while_stmt->expr);
    printer_print_block (self, "Block", ANY_CAST(while_stmt->block));
    printer_print_footer(self);
    return self;
}

// =============================================================================
// ast pretty printer impl
// =============================================================================

void pretty_print_ast(ast_t ast)
{
    ASSERT(ast != NULL, "can't print null ast");
    printer_t printer = printer_create();
    printer_print_header(printer, "Program");
    printer_print_ast   (printer, ast);
    printer_print_footer(printer);
    printer_delete      (printer);
}
