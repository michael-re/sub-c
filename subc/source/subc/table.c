#include "subc/table.h"
#include "subc/memory.h"

// =============================================================================
// context
// =============================================================================
typedef struct context
{
    symtable_t table;
    size_t     depth;
    bool       error;
    bool       exit;
} *context_t;

#define SUBC_TABLE_ERROR "@ INTERNAL TABLE LAYOUT ERROR @"
static inline void context_error(context_t self, token_t token, const char* message)
{
    ASSERT(self    != NULL, "null context");
    ASSERT(message != NULL, "null error message");
    ASSERT(token   != NULL, "null error token");

    token_report(token, "error", message);
    self->error = true;

    string_t error = string_create("%s", SUBC_TABLE_ERROR);
    hashmap_insert(self->table->indicies, error, SIZE_MAX);
    string_delete(error);
}

// =============================================================================
// symbol type impl
// =============================================================================

const char* sym_type_string(sym_type type)
{
    switch (type)
    {
        case TYP_INT: return "TYP_INT";
        case TYP_STR: return "TYP_STR";
        case TYP_UNK: return "TYP_UNK";
        default:      break;
    }

    FATAL("unknown symbol type");
}

// =============================================================================
// symbol role impl
// =============================================================================

const char* sym_role_string(sym_role role)
{
    switch (role)
    {
        case TYP_INT: return "ROL_PAR";
        case TYP_STR: return "ROL_VAR";
        case TYP_UNK: return "ROL_UNK";
        default:       break;
    }

    FATAL("unknown symbol role");
}

// =============================================================================
// layout impl
// =============================================================================

static layout_t layout_visit       (layout_t self, context_t ctx, ast_t ast);
static layout_t layout_visit_assign(layout_t self, context_t ctx, ast_t ast);
static layout_t layout_visit_binary(layout_t self, context_t ctx, ast_t ast);
static layout_t layout_visit_call  (layout_t self, context_t ctx, ast_t ast);
static layout_t layout_visit_func  (layout_t self, context_t ctx, ast_t ast);
static layout_t layout_visit_if    (layout_t self, context_t ctx, ast_t ast);
static layout_t layout_visit_list  (layout_t self, context_t ctx, ast_t ast);
static layout_t layout_visit_name  (layout_t self, context_t ctx, ast_t ast);
static layout_t layout_visit_number(layout_t self, context_t ctx, ast_t ast);
static layout_t layout_visit_return(layout_t self, context_t ctx, ast_t ast);
static layout_t layout_visit_string(layout_t self, context_t ctx, ast_t ast);
static layout_t layout_visit_while (layout_t self, context_t ctx, ast_t ast);

layout_t layout_create(symtable_t ctx, ast_func function)
{
    ASSERT(ctx      != NULL, "can't create layout for function with null context");
    ASSERT(function != NULL, "can't create layout for null function");

    const size_t par_count = function->pars->size;
    const size_t var_count = function->vars->size;

    layout_t self  = NEW(self, 1);
    self->buffer   = buffer_create(sizeof(*self->symbols), par_count + var_count);
    self->indicies = hashmap_create(par_count + var_count);
    self->function = function;
    self->symbols  = self->buffer->data;
    self->offset   = ctx->indicies->size;

    struct context context = {
        .table = ctx,
        .depth = 0,
        .error = false,
        .exit  = false
    };

    layout_visit(self, &context, (ast_t) function);
    return context.error ? layout_delete(self) : self;
}

layout_t layout_delete(layout_t self)
{
    if (self)
    {
        for (size_t i = 0; i < self->indicies->size; i++)
            self->symbols[i] = (struct symbol) { 0 };

        self->buffer   = buffer_delete(self->buffer);
        self->indicies = hashmap_delete(self->indicies);
        (*self)        = (struct layout) { 0 };
    }

    return DELETE(self);
}

static layout_t layout_visit(layout_t self, context_t ctx, ast_t ast)
{
    ASSERT(self != NULL, "null layout");
    ASSERT(ast  != NULL, "can't visit null ast");

    switch (ast->type)
    {
        case AST_ASSIGN: return layout_visit_assign(self, ctx, ast);
        case AST_BINARY: return layout_visit_binary(self, ctx, ast);
        case AST_CALL:   return layout_visit_call  (self, ctx, ast);
        case AST_FUNC:   return layout_visit_func  (self, ctx, ast);
        case AST_IF:     return layout_visit_if    (self, ctx, ast);
        case AST_LIST:   return layout_visit_list  (self, ctx, ast);
        case AST_NAME:   return layout_visit_name  (self, ctx, ast);
        case AST_NUMBER: return layout_visit_number(self, ctx, ast);
        case AST_RETURN: return layout_visit_return(self, ctx, ast);
        case AST_STRING: return layout_visit_string(self, ctx, ast);
        case AST_WHILE:  return layout_visit_while (self, ctx, ast);
        default:         break;
    }

    FATAL("unknown ast type");
}

static layout_t layout_visit_assign(layout_t self, context_t ctx, ast_t ast)
{
    ASSERT(self && ctx && ast && ast->type == AST_ASSIGN, "invalid arg");

    ast_assign assign = ANY_CAST(ast);
    if (!hashmap_contains(self->indicies, assign->name->lexeme))
        context_error(ctx, assign->name, "reference to undeclared variable");

    layout_visit(self, ctx, assign->value);
    return self;
}

static layout_t layout_visit_binary(layout_t self, context_t ctx, ast_t ast)
{
    ASSERT(self && ctx && ast && ast->type == AST_BINARY, "invalid arg");
    ast_binary binary = ANY_CAST(ast);
    layout_visit(self, ctx, binary->lhs);
    layout_visit(self, ctx, binary->rhs);
    return self;
}

static layout_t layout_visit_call(layout_t self, context_t ctx, ast_t ast)
{
    ASSERT(self && ctx && ast && ast->type == AST_CALL, "invalid arg");

    ast_call call   = ANY_CAST(ast);
    entry_t  index  = hashmap_contains(ctx->table->indicies, call->name->token->lexeme);
    layout_t callee = index
                           ? ctx->table->layouts[index->value]
                           : token_cmp_eq(self->function->name->token, call->name->token)
                                ? self
                                : NULL;

    if (!callee)
    {
        if (!index)
            context_error(ctx, call->name->token, "call to undeclared function");
        return self;
    }

    if (callee->function->pars->size != call->args->size)
    {
        size_t   expect = callee->function->pars->size;
        size_t   got    = call->args->size;
        string_t error  = string_create("expect %zd args but got %zd", expect, got);
        context_error(ctx, callee->function->name->token, error->chars);
        string_delete(error);
        return self;
    }

    for (size_t i = 0; i < call->args->size; i++)
    {
        symbol_t par = &callee->symbols[i];
        ast_t    arg = call->args->nodes[i];
        ASSERT(arg && par->type != TYP_UNK, "invalid call arg");

        if ((par->type == TYP_STR) && (arg->type != AST_STRING))
        {
            if (arg->type == AST_NUMBER)
                context_error(ctx, ((ast_number) arg)->token, "passing 'int' arg to parameter expecting 'const char*'");
            else if (arg->type == AST_NAME)
                context_error(ctx, ((ast_name) arg)->token,   "passing 'int' arg to parameter expecting 'const char*'");
            else
                FATAL("unknown arg type");
        }

        if ((par->type == TYP_INT) && (arg->type != AST_NAME && arg->type != AST_NUMBER))
        {
            if (arg->type == AST_STRING)
                context_error(ctx, ((ast_string) arg)->token, "passing 'const char*' to parameter expecting 'int'");
            else
                FATAL("unknown arg type");
        }
    }

    return self;
}

static layout_t layout_visit_func(layout_t self, context_t ctx, ast_t ast)
{
    ASSERT(self && ctx && ast && ast->type == AST_FUNC, "invalid arg");
    ast_func func = ANY_CAST(ast);
    token_t  name = func->name->token;

    if (hashmap_contains(ctx->table->indicies, name->lexeme))
        context_error(ctx, name, "function with this name already defined in the global scope");

    size_t  index  = 0;
    int64_t offset = 8;

    // parameters
    for (size_t i = 0; i < func->pars->size; i++, index++, offset += 4)
    {
        ast_t node = func->pars->nodes[i];
        ASSERT(node && node->type == AST_NAME, "invalid parameter");

        ast_name par = ANY_CAST(node);
        if (token_cmp_eq(name, par->token))
            context_error(ctx, par->token, "parameter name can't shadow function name");
        else if (hashmap_contains(self->indicies, par->token->lexeme))
            context_error(ctx, par->token, "parameter with this name already defined in the current scope");

        hashmap_insert(self->indicies, par->token->lexeme, index);
        self->symbols[index] = (struct symbol) {
            .type   = TYP_INT,
            .role   = ROL_PAR,
            .name   = par->token,
            .offset = offset,
            .index  = index,
        };
    }

    offset = -4;

    // variables
    for (size_t i = 0; i < func->vars->size; i++, index++, offset -= 4)
    {
        ast_t node = func->vars->nodes[i];
        ASSERT(node && node->type == AST_NAME, "invalid variable");

        ast_name var = ANY_CAST(node);
        if (token_cmp_eq(name, var->token))
            context_error(ctx, var->token, "variable name can't shadow function name");
        else if (hashmap_contains(self->indicies, var->token->lexeme))
            context_error(ctx, var->token, "variable with this name already defined in the current scope");

        hashmap_insert(self->indicies, var->token->lexeme, index);
        self->symbols[index] = (struct symbol) {
            .type   = TYP_INT,
            .role   = ROL_VAR,
            .name   = var->token,
            .offset = offset,
            .index  = index,
        };
    }

    layout_visit(self, ctx, (ast_t) func->stmts);
    if (!ctx->exit)
        context_error(ctx, name, "not all control path in function return a value");

    if (string_str_eq(name->lexeme, "main") && func->pars->size != 0)
        context_error(ctx, name, "subc main function can't have any parameters");

    return self;
}

static layout_t layout_visit_if(layout_t self, context_t ctx, ast_t ast)
{
    ASSERT(self && ctx && ast && ast->type == AST_IF, "invalid arg");
    ctx->depth++;

    ast_if if_stmt = ANY_CAST(ast);
    layout_visit(self, ctx, if_stmt->expr);
    layout_visit(self, ctx, (ast_t) if_stmt->block);

    ctx->depth--;
    return self;
}

static layout_t layout_visit_list(layout_t self, context_t ctx, ast_t ast)
{
    ASSERT(self && ctx && ast && ast->type == AST_LIST, "invalid arg");
    ast_list list = ANY_CAST(ast);

    for (size_t i = 0; i < list->size; i++)
        layout_visit(self, ctx, list->nodes[i]);

    return self;
}

static layout_t layout_visit_name(layout_t self, context_t ctx, ast_t ast)
{
    ASSERT(self && ctx && ast && ast->type == AST_NAME, "invalid arg");
    ast_name name = ANY_CAST(ast);
    string_t text = name->token->lexeme;

    entry_t index = hashmap_contains(self->indicies, text);
    if (!index)
        context_error(ctx, name->token, "reference to undeclared variable");

    return self;
}

static layout_t layout_visit_number(layout_t self, context_t ctx, ast_t ast)
{
    ASSERT(self && ctx && ast && ast->type == AST_NUMBER, "invalid arg");
    return self;
}

static layout_t layout_visit_return(layout_t self, context_t ctx, ast_t ast)
{
    ASSERT(self && ctx && ast && ast->type == AST_RETURN, "invalid arg");
    ast_return ret = ANY_CAST(ast);
    layout_visit(self, ctx, ret->expr);
    if (ctx->depth == 0) ctx->exit = true;
    return self;
}

static layout_t layout_visit_string(layout_t self, context_t ctx, ast_t ast)
{
    ASSERT(self && ctx && ast && ast->type == AST_STRING, "invalid arg");
    return self;
}

static layout_t layout_visit_while(layout_t self, context_t ctx, ast_t ast)
{
    ASSERT(self && ctx && ast && ast->type == AST_WHILE, "invalid arg");
    ctx->depth++;

    ast_while while_stmt = ANY_CAST(ast);
    layout_visit(self, ctx, while_stmt->expr);
    layout_visit(self, ctx, (ast_t) while_stmt->block);

    ctx->depth--;
    return self;
}

// =============================================================================
// symbol table impl
// =============================================================================

static ast_func create_intrinsic_func(const char* name, size_t par_count)
{
    token_t fun_name = token_create(TOK_NAME,   string_create("%s", name), 0, 0);
    token_t par_name = token_create(TOK_NAME,   string_create("%s", ""),   0, 0);
    token_t tok_num  = token_create(TOK_NUMBER, string_create("%s", "0"),  0, 0);

    ast_t pars = ast_list_create();
    ast_t vars = ast_list_create();
    ast_t stms = ast_list_create();
    ast_t fnam = ast_name_create(fun_name);

    for (size_t i = 0; i < par_count; i++)
    {
        if (par_count >= 2)
            string_append(par_name->lexeme, "__x__%zd", i);
        else
            string_append(par_name->lexeme, "__x__");

        ast_list_append(pars, ast_name_create(par_name));
        string_remove(par_name->lexeme, 0, par_name->lexeme->size);
    }

    ast_list_append(stms, ast_return_create(ast_number_create(tok_num)));
    token_delete(fun_name);
    token_delete(par_name);
    token_delete(tok_num);

    return (ast_func) ast_func_create(fnam, pars, vars, stms);
}

static symtable_t symtable_intrinsics(symtable_t self, ast_list functions)
{
    ASSERT(self      != NULL, "can't initialize intrinsics when symbol table is null");
    ASSERT(functions != NULL, "can't initialize intrinsics for null program");

    ast_list intrinsics = ANY_CAST(ast_list_create());
    ast_func says       = create_intrinsic_func("__says", 1);
    ast_func sayn       = create_intrinsic_func("__sayn", 1);
    ast_func sayl       = create_intrinsic_func("__sayl", 0);

    ast_list_append(ANY_CAST(intrinsics), ANY_CAST(says));
    ast_list_append(ANY_CAST(intrinsics), ANY_CAST(sayn));
    ast_list_append(ANY_CAST(intrinsics), ANY_CAST(sayl));

    self->buffer     = buffer_create(sizeof(*self->layouts), functions->size + intrinsics->size);
    self->indicies   = hashmap_create(functions->size);
    self->layouts    = self->buffer->data;
    self->functions  = functions;
    self->intrinsics = intrinsics;

    self->layouts[0]                  = layout_create(self, says);
    self->layouts[0]->symbols[0].type = TYP_STR;
    hashmap_insert(self->indicies, says->name->token->lexeme, 0);

    self->layouts[1]                  = layout_create(self, sayn);
    self->layouts[1]->symbols[0].type = TYP_INT;
    hashmap_insert(self->indicies, sayn->name->token->lexeme, 1);

    self->layouts[2] = layout_create(self, sayl);
    hashmap_insert(self->indicies, sayl->name->token->lexeme, 2);

    return self;
}

symtable_t symtable_create(ast_t program)
{
    ASSERT(program && program->type == AST_LIST, "expect list of function");
    ast_list funcs = ANY_CAST(program);
    bool     error = false;
    bool     main  = false;

    symtable_t self = symtable_intrinsics(NEW(self, 1), funcs);
    for (size_t i = 0; i < funcs->size; i++)
    {
        ast_t ast = funcs->nodes[i];
        ASSERT(ast && ast->type == AST_FUNC, "expect function ast");
        ast_func func = ANY_CAST(ast);

        const size_t index = i + self->intrinsics->size;
        self->layouts[index] = layout_create(self, func);
        hashmap_insert(self->indicies, func->name->token->lexeme, index);
        if (string_str_eq(func->name->token->lexeme, "main")) main = true;
    }

    string_t layout_error = string_create("%s", SUBC_TABLE_ERROR);
    if (hashmap_contains(self->indicies, layout_error))
        error = true;
    string_delete(layout_error);

    if (!main)
    {
        error = true;
        token_report(NULL, "error", "missing main function");
    }

    return error ? symtable_delete(self) : self;
}

symtable_t symtable_delete(symtable_t self)
{
    if (self)
    {
        for (size_t i = 0; i < self->functions->size + self->intrinsics->size; i++)
            self->layouts[i] = layout_delete(self->layouts[i]);

        buffer_delete(self->buffer);
        hashmap_delete(self->indicies);
        ast_delete(ANY_CAST(self->functions));
        ast_delete(ANY_CAST(self->intrinsics));
        (*self) = (struct symtable) { 0 };
    }

    return DELETE(self);
}

layout_t symtable_get_layout(symtable_t self, string_t function)
{
    ASSERT(self     != NULL, "can't retrieve layout in null symbol-table");
    ASSERT(function != NULL, "can't retrieve layout of function with null name");

    entry_t index = hashmap_contains(self->indicies, function);
    ASSERT(index != NULL, "failed to find function layout");
    return self->layouts[index->value];
}

symbol_t symtable_get_symbol(symtable_t self, string_t function, string_t name)
{
    layout_t layout = symtable_get_layout(self, function);
    entry_t index = hashmap_contains(layout->indicies, name);

    ASSERT(index != NULL, "failed to find symbol");
    return &layout->symbols[index->value];
}
