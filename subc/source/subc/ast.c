#include <stdlib.h>

#include "subc/ast.h"
#include "subc/memory.h"

// =============================================================================
// ast impl
// =============================================================================

static ast_t ast_assign_delete(ast_t self);
static ast_t ast_binary_delete(ast_t self);
static ast_t ast_call_delete  (ast_t self);
static ast_t ast_func_delete  (ast_t self);
static ast_t ast_if_delete    (ast_t self);
static ast_t ast_list_delete  (ast_t self);
static ast_t ast_name_delete  (ast_t self);
static ast_t ast_number_delete(ast_t self);
static ast_t ast_return_delete(ast_t self);
static ast_t ast_string_delete(ast_t self);
static ast_t ast_while_delete (ast_t self);

#define AST_DELETE(pointer) (ANY_CAST(ast_delete((ast_t) pointer)))

ast_t ast_delete(ast_t self)
{
    if (self == NULL) return self;
    switch (self->type)
    {
        case AST_ASSIGN: return ast_assign_delete(self);
        case AST_BINARY: return ast_binary_delete(self);
        case AST_CALL:   return ast_call_delete  (self);
        case AST_FUNC:   return ast_func_delete  (self);
        case AST_IF:     return ast_if_delete    (self);
        case AST_LIST:   return ast_list_delete  (self);
        case AST_NAME:   return ast_name_delete  (self);
        case AST_NUMBER: return ast_number_delete(self);
        case AST_RETURN: return ast_return_delete(self);
        case AST_STRING: return ast_string_delete(self);
        case AST_WHILE:  return ast_while_delete (self);
        default:         break;
    }

    FATAL("calling delete on ast with an unknown type");
}

// =============================================================================
// ast node type impl
// =============================================================================

const char* ast_type_string(ast_type type)
{
    switch (type)
    {
        case AST_ASSIGN: return "AST_ASSIGN";
        case AST_BINARY: return "AST_BINARY";
        case AST_CALL:   return "AST_CALL";
        case AST_FUNC:   return "AST_FUNC";
        case AST_IF:     return "AST_IF";
        case AST_LIST:   return "AST_LIST";
        case AST_NAME:   return "AST_NAME";
        case AST_NUMBER: return "AST_NUMBER";
        case AST_RETURN: return "AST_RETURN";
        case AST_STRING: return "AST_STRING";
        case AST_WHILE:  return "AST_WHILE";
        default:         break;
    }

    FATAL("unknown ast type");
}

// =============================================================================
// list impl
// =============================================================================

ast_t ast_list_create(void)
{
    ast_list self   = NEW(self, 1);
    self->base.type = AST_LIST;
    self->buff      = buffer_create(sizeof(*self->nodes), 0);
    self->size      = 0;
    self->nodes     = self->buff->data;
    return ANY_CAST(self);
}

static ast_t ast_list_delete(ast_t self)
{
    ASSERT(self && self->type == AST_LIST, "invalid ast");

    ast_list list = ANY_CAST(self);
    for (size_t i = 0; i < list->size; i++)
        list->nodes[i] = AST_DELETE(list->nodes[i]);

    list->buff = buffer_delete(list->buff);
    (*list)    = (struct ast_list) { 0 };
    return DELETE(self);
}

ast_t ast_list_append(ast_t self, ast_t ast)
{
    if (!ast) return self;
    ASSERT(self && self->type == AST_LIST, "can't append to non-list ast");
    ASSERT(self != ast,                    "can't append ast_list to itself");

    ast_list list = ANY_CAST(self);
    list->nodes   = buffer_expand(list->buff, list->size + 1);
    ASSERT(list->buff  != NULL, "list buffer corrupted");
    ASSERT(list->nodes != NULL, "list buffer corrupted");

    const size_t index = list->size++;
    list->nodes[index] = ast;
    return self;
}

// =============================================================================
// name impl
// =============================================================================

ast_t ast_name_create(token_t token)
{
    if (!token) return NULL;
    ASSERT(token->type == TOK_NAME, "invalid name");

    ast_name self   = NEW(self, 1);
    self->base.type = AST_NAME;
    self->token     = token_clone(token);
    return ANY_CAST(self);
}

static ast_t ast_name_delete(ast_t self)
{
    ASSERT(self && self->type == AST_NAME, "invalid ast");
    ast_name name = ANY_CAST(self);
    name->token   = token_delete(name->token);
    (*name)       = (struct ast_name) { 0 };
    return DELETE(self);
}

// =============================================================================
// assign impl
// =============================================================================

ast_t ast_assign_create(token_t name, ast_t value)
{
    if (!name || !value)
    {
        AST_DELETE(value);
        return NULL;
    }

    ASSERT(name->type == TOK_NAME, "invalid assign name");

    ast_assign self = NEW(self, 1);
    self->base.type = AST_ASSIGN;
    self->name      = token_clone(name);
    self->value     = value;
    return ANY_CAST(self);
}

static ast_t ast_assign_delete(ast_t self)
{
    ASSERT(self && self->type == AST_ASSIGN, "invalid ast");
    ast_assign assign = ANY_CAST(self);
    assign->name      = token_delete(assign->name);
    assign->value     = AST_DELETE(assign->value);
    (*assign)         = (struct ast_assign) { 0 };
    return DELETE(self);
}

// =============================================================================
// binary impl
// =============================================================================

ast_t ast_binary_create(token_t bop, ast_t lhs, ast_t rhs)
{
    if (!bop || !lhs || !rhs)
    {
        AST_DELETE(lhs);
        AST_DELETE(rhs);
        return NULL;
    }

    ASSERT(tok_type_is_bop(bop->type), "invalid binary operator");

    ast_binary self = NEW(self, 1);
    self->base.type = AST_BINARY;
    self->bop       = token_clone(bop);
    self->lhs       = lhs;
    self->rhs       = rhs;
    return ANY_CAST(self);
}

static ast_t ast_binary_delete(ast_t self)
{
    ASSERT(self && self->type == AST_BINARY, "invalid ast");
    ast_binary binary = ANY_CAST(self);
    binary->bop       = token_delete(binary->bop);
    binary->lhs       = AST_DELETE(binary->lhs);
    binary->rhs       = AST_DELETE(binary->rhs);
    (*binary)         = (struct ast_binary) { 0 };
    return DELETE(self);
}

// =============================================================================
// call impl
// =============================================================================

ast_t ast_call_create(ast_t name, ast_t args)
{
    if (!name || !args)
    {
        AST_DELETE(name);
        AST_DELETE(args);
        return NULL;
    }

    ASSERT(name->type == AST_NAME, "invalid callee name");
    ASSERT(args->type == AST_LIST, "invalid call args");

    ast_call self   = NEW(self, 1);
    self->base.type = AST_CALL;
    self->name      = ANY_CAST(name);
    self->args      = ANY_CAST(args);
    return ANY_CAST(self);
}

static ast_t ast_call_delete(ast_t self)
{
    ASSERT(self && self->type == AST_CALL, "invalid ast");
    ast_call call = ANY_CAST(self);
    call->name    = AST_DELETE(call->name);
    call->args    = AST_DELETE(call->args);
    (*call)       = (struct ast_call) { 0 };
    return DELETE(self);
}

// =============================================================================
// func impl
// =============================================================================

ast_t ast_func_create(ast_t name, ast_t pars, ast_t vars, ast_t stmts)
{
    if (!name || !pars || !vars || !stmts)
    {
        AST_DELETE(name);
        AST_DELETE(pars);
        AST_DELETE(vars);
        AST_DELETE(stmts);
        return NULL;
    }

    ASSERT(name->type  == AST_NAME, "invalid func name");
    ASSERT(pars->type  == AST_LIST, "invalid func pars");
    ASSERT(vars->type  == AST_LIST, "invalid func vars");
    ASSERT(stmts->type == AST_LIST, "invalid func body");

    ast_func self   = NEW(self, 1);
    self->base.type = AST_FUNC;
    self->name      = ANY_CAST(name);
    self->pars      = ANY_CAST(pars);
    self->vars      = ANY_CAST(vars);
    self->stmts     = ANY_CAST(stmts);
    return ANY_CAST(self);
}

static ast_t ast_func_delete(ast_t self)
{
    ASSERT(self && self->type == AST_FUNC, "invalid ast");
    ast_func func = ANY_CAST(self);
    func->name    = AST_DELETE(func->name);
    func->pars    = AST_DELETE(func->pars);
    func->vars    = AST_DELETE(func->vars);
    func->stmts   = AST_DELETE(func->stmts);
    (*func)       = (struct ast_func) { 0 };
    return DELETE(self);
}

// =============================================================================
// if impl
// =============================================================================

ast_t ast_if_create(ast_t expr, ast_t block)
{
    if (!expr || !block)
    {
        AST_DELETE(expr);
        AST_DELETE(block);
        return NULL;
    }

    ast_if self     = NEW(self, 1);
    self->base.type = AST_IF;
    self->expr      = expr;
    self->block     = block;
    return ANY_CAST(self);
}

static ast_t ast_if_delete(ast_t self)
{
    ASSERT(self && self->type == AST_IF, "invalid ast");
    ast_if if_stmt = ANY_CAST(self);
    if_stmt->expr  = AST_DELETE(if_stmt->expr);
    if_stmt->block = AST_DELETE(if_stmt->block);
    (*if_stmt)     = (struct ast_if) { 0 };
    return DELETE(self);
}

// =============================================================================
// number impl
// =============================================================================

ast_t ast_number_create(token_t token)
{
    if (!token) return NULL;
    ASSERT(token->type == TOK_NUMBER, "invalid number literal");

    ast_number self = NEW(self, 1);
    self->base.type = AST_NUMBER;
    self->token     = token_clone(token);
    self->value     = strtoull(self->token->lexeme->chars, NULL, 10);
    return ANY_CAST(self);
}

static ast_t ast_number_delete(ast_t self)
{
    ASSERT(self && self->type == AST_NUMBER, "invalid ast");
    ast_number number = ANY_CAST(self);
    number->token     = token_delete(number->token);
    (*number)         = (struct ast_number) { 0 };
    return DELETE(self);
}

// =============================================================================
// return impl
// =============================================================================

ast_t ast_return_create(ast_t expr)
{
    if (!expr) return NULL;
    ast_return self = NEW(self, 1);
    self->base.type = AST_RETURN;
    self->expr      = expr;
    return ANY_CAST(self);
}

static ast_t ast_return_delete(ast_t self)
{
    ASSERT(self && self->type == AST_RETURN, "invalid ast");
    ast_return ret = ANY_CAST(self);
    ret->expr      = AST_DELETE(ret->expr);
    (*ret)         = (struct ast_return) { 0 };
    return DELETE(self);
}

// =============================================================================
// string impl
// =============================================================================

ast_t ast_string_create(token_t token)
{
    if (!token) return NULL;
    ASSERT(token->type == TOK_STRING, "invalid string literal");

    ast_string self = NEW(self, 1);
    self->base.type = AST_STRING;
    self->token     = token_clone(token);
    self->value     = self->token->lexeme;
    return ANY_CAST(self);
}

static ast_t ast_string_delete(ast_t self)
{
    ASSERT(self && self->type == AST_STRING, "invalid ast");
    ast_string string = ANY_CAST(self);
    string->token     = token_delete(string->token);
    (*string)         = (struct ast_string) { 0 };
    return DELETE(self);
}

// =============================================================================
// while impl
// =============================================================================

ast_t ast_while_create(ast_t expr, ast_t block)
{
    if (!expr || !block)
    {
        AST_DELETE(expr);
        AST_DELETE(block);
        return NULL;
    }

    ast_while self  = NEW(self, 1);
    self->base.type = AST_WHILE;
    self->expr      = expr;
    self->block     = block;
    return ANY_CAST(self);
}

static ast_t ast_while_delete(ast_t self)
{
    ASSERT(self && self->type == AST_WHILE, "invalid ast");
    ast_while while_stmt = ANY_CAST(self);
    while_stmt->expr     = AST_DELETE(while_stmt->expr);
    while_stmt->block    = AST_DELETE(while_stmt->block);
    (*while_stmt)        = (struct ast_while) { 0 };
    return DELETE(self);
}
