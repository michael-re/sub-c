#include "subc/codegen.h"
#include "subc/memory.h"

// =============================================================================
// codegen
// =============================================================================
typedef struct codegen
{
    symtable_t context;
    emitter_t  emitter;
    ast_func   func;
    size_t     nn_reg;
} *codegen_t;

static codegen_t codegen_create     (symtable_t context);
static codegen_t codegen_delete     (codegen_t self);
static codegen_t codegen_intrinsics (codegen_t self);
static codegen_t codegen_emit_says  (codegen_t self);
static codegen_t codegen_emit_sayn  (codegen_t self);
static codegen_t codegen_emit_sayl  (codegen_t self);
static codegen_t codegen_branch     (codegen_t self, const char* opcode);
static codegen_t codegen_emit       (codegen_t self, ast_t ast);
static codegen_t codegen_emit_assign(codegen_t self, ast_t ast);
static codegen_t codegen_emit_binary(codegen_t self, ast_t ast);
static codegen_t codegen_emit_call  (codegen_t self, ast_t ast);
static codegen_t codegen_emit_func  (codegen_t self, ast_t ast);
static codegen_t codegen_emit_if    (codegen_t self, ast_t ast);
static codegen_t codegen_emit_list  (codegen_t self, ast_t ast);
static codegen_t codegen_emit_name  (codegen_t self, ast_t ast);
static codegen_t codegen_emit_number(codegen_t self, ast_t ast);
static codegen_t codegen_emit_return(codegen_t self, ast_t ast);
static codegen_t codegen_emit_string(codegen_t self, ast_t ast);
static codegen_t codegen_emit_while (codegen_t self, ast_t ast);

// =============================================================================
// codegen impl
// =============================================================================

static codegen_t codegen_create(symtable_t context)
{
    ASSERT(context != NULL, "can't generate code of null context");
    codegen_t self = NEW(self, 1);
    self->context  = context;
    self->emitter  = emitter_create();
    self->func     = NULL;
    self->nn_reg   = 0;
    return self;
}

static codegen_t codegen_delete(codegen_t self)
{
    if (self)
    {
        self->context = symtable_delete(self->context);
        self->emitter = emitter_delete(self->emitter);
        (*self)       = (struct codegen) { 0 };
    }

    return DELETE(self);
}

static codegen_t codegen_intrinsics(codegen_t self)
{
    ASSERT(self != NULL, "null codegen");
    emitter_emit_code  (self->emitter, "");
    emitter_emit_code  (self->emitter, "@ ===================================================================");
    emitter_emit_code  (self->emitter, "@ subc intrinsics");
    emitter_emit_code  (self->emitter, "@ ===================================================================");
    emitter_emit_code  (self->emitter, "        .data");
    emitter_emit_opcode(self->emitter, ".align", "4");
    emitter_emit_code  (self->emitter, "");
    emitter_emit_code  (self->emitter, "__l0:");
    emitter_emit_opcode(self->emitter, ".asciz", "\"\\r\\n\"");
    emitter_emit_opcode(self->emitter, ".equ",   "__stdout,    0x01");
    emitter_emit_opcode(self->emitter, ".equ",   "__swi_pstr,  0x69");
    emitter_emit_opcode(self->emitter, ".equ",   "__swi_pint,  0x6b");
    emitter_emit_code  (self->emitter, "");
    emitter_emit_opcode(self->emitter, ".global",  "__says, __sayn, __sayl");
    emitter_emit_code  (self->emitter,  "        .text");
    emitter_emit_opcode(self->emitter, ".align", "4");

    codegen_emit_says(self);
    codegen_emit_sayn(self);
    codegen_emit_sayl(self);

    return self;
}

static codegen_t codegen_emit_says(codegen_t self)
{
    ASSERT(self != NULL, "null codegen");
    emitter_emit_code  (self->emitter, "");
    emitter_emit_code  (self->emitter, "@ ===================================================================");
    emitter_emit_code  (self->emitter, "@ intrinsic: int __says(const char* __x__)");
    emitter_emit_code  (self->emitter, "@       prints the given null terminated string to stdout. returns 0.");
    emitter_emit_code  (self->emitter, "@ ===================================================================");
    emitter_emit_code  (self->emitter, "__says:");
    emitter_emit_opcode(self->emitter, "push", "{r1, lr}");
    emitter_emit_opcode(self->emitter, "mov",  "r1, r0");
    emitter_emit_opcode(self->emitter, "ldr",  "r0, =__stdout");
    emitter_emit_opcode(self->emitter, "swi",  "__swi_pstr");
    emitter_emit_opcode(self->emitter, "mov",  "r0, #0");
    emitter_emit_opcode(self->emitter, "pop",  "{r1, lr}");
    emitter_emit_opcode(self->emitter, "bx",   "lr");
    return self;
}

static codegen_t codegen_emit_sayn(codegen_t self)
{
    ASSERT(self != NULL, "null codegen");
    emitter_emit_code  (self->emitter, "");
    emitter_emit_code  (self->emitter, "@ ===================================================================");
    emitter_emit_code  (self->emitter, "@ intrinsic: int __sayn(int __x__)");
    emitter_emit_code  (self->emitter, "@       prints the given number to stdout. returns 0.");
    emitter_emit_code  (self->emitter, "@ ===================================================================");
    emitter_emit_code  (self->emitter, "__sayn:");
    emitter_emit_opcode(self->emitter, "push", "{r1, lr}");
    emitter_emit_opcode(self->emitter, "mov",  "r1, r0");
    emitter_emit_opcode(self->emitter, "ldr",  "r0, =__stdout");
    emitter_emit_opcode(self->emitter, "swi",  "__swi_pint");
    emitter_emit_opcode(self->emitter, "mov",  "r0, #0");
    emitter_emit_opcode(self->emitter, "pop",  "{r1, lr}");
    emitter_emit_opcode(self->emitter, "bx",   "lr");
    return self;
}

static codegen_t codegen_emit_sayl(codegen_t self)
{
    ASSERT(self != NULL, "null codegen");
    emitter_emit_code (self->emitter, "");
    emitter_emit_code (self->emitter, "@ ===================================================================");
    emitter_emit_code (self->emitter, "@ intrinsic: int __sayl(void)");
    emitter_emit_code (self->emitter, "@       prints an empty line to stdout. returns 0.");
    emitter_emit_code (self->emitter, "@ ===================================================================");
    emitter_emit_code (self->emitter, "__sayl:");
    emitter_emit_opcode(self->emitter, "push", "{lr}");
    emitter_emit_opcode(self->emitter, "mov",  "r1, r0");
    emitter_emit_opcode(self->emitter, "ldr",  "r0, =__l0");
    emitter_emit_opcode(self->emitter, "bl",  "__says");
    emitter_emit_opcode(self->emitter, "mov",  "r0, #0");
    emitter_emit_opcode(self->emitter, "pop",  "{lr}");
    emitter_emit_opcode(self->emitter, "bx",   "lr");
    return self;
}

static codegen_t codegen_branch(codegen_t self, const char* opcode)
{
    ASSERT(self   != NULL, "null codegen");
    ASSERT(opcode != NULL, "null branch opcode");

    string_t true_label = emitter_make_label(self->emitter);
    string_t exit_label = emitter_make_label(self->emitter);

    // false
    emitter_emit_opcode(self->emitter, opcode, "%s", true_label->chars);
    emitter_emit_opcode(self->emitter, "ldr", "r0, =0");
    emitter_emit_opcode(self->emitter, "b", "%s", exit_label->chars);

    // true
    emitter_emit_code_label(self->emitter, true_label);
    emitter_emit_opcode(self->emitter, "ldr", "r0, =1");
    emitter_emit_code_label(self->emitter, exit_label);

    string_delete(true_label);
    string_delete(exit_label);
    return self;
}

static codegen_t codegen_emit(codegen_t self, ast_t ast)
{
    ASSERT(self && ast, "invalid args");
    switch (ast->type)
    {
        case AST_ASSIGN: return codegen_emit_assign(self, ast);
        case AST_BINARY: return codegen_emit_binary(self, ast);
        case AST_CALL:   return codegen_emit_call  (self, ast);
        case AST_FUNC:   return codegen_emit_func  (self, ast);
        case AST_IF:     return codegen_emit_if    (self, ast);
        case AST_LIST:   return codegen_emit_list  (self, ast);
        case AST_NAME:   return codegen_emit_name  (self, ast);
        case AST_NUMBER: return codegen_emit_number(self, ast);
        case AST_RETURN: return codegen_emit_return(self, ast);
        case AST_STRING: return codegen_emit_string(self, ast);
        case AST_WHILE:  return codegen_emit_while (self, ast);
        default:         break;
    }

    FATAL("unknown ast type");
}

static codegen_t codegen_emit_assign(codegen_t self, ast_t ast)
{
    ASSERT(self && self->func && ast && ast->type == AST_ASSIGN, "invalid args");

    // emit value - result in r0
    ast_assign assign = ANY_CAST(ast);
    codegen_emit(self, assign->value);

    // store value in r0 into variable offset
    symbol_t variable = symtable_get_symbol(self->context,
                                            self->func->name->token->lexeme,
                                            assign->name->lexeme);
    ASSERT(variable != NULL, "invalid variable");
    emitter_emit_opcode(self->emitter, "str", "r0, [fp, #%d]", (int) variable->offset);
    return self;
}

static codegen_t codegen_emit_binary(codegen_t self, ast_t ast)
{
    ASSERT(self && ast && ast->type == AST_BINARY, "invalid args");
    ast_binary   binary = ANY_CAST(ast);
    const size_t saved  = self->nn_reg;

    // left
    self->nn_reg = 0;
    codegen_emit(self, binary->lhs);

    // right
    self->nn_reg = 1;
    codegen_emit(self, binary->rhs);

    // operator
    self->nn_reg = saved;
    ASSERT(tok_type_is_bop(binary->bop->type), "invalid binary operator");

    // arithmetic
    switch (binary->bop->type)
    {
        case TOK_OP_ADD: emitter_emit_opcode(self->emitter, "add", "r0, r0, r1"); return self;
        case TOK_OP_SUB: emitter_emit_opcode(self->emitter, "sub", "r0, r0, r1"); return self;
        case TOK_OP_MUL: emitter_emit_opcode(self->emitter, "mul", "r0, r0, r1"); return self;
        default:         break;
    }

    // comparison
    emitter_emit_opcode(self->emitter, "cmp", "r0, r1");
    switch (binary->bop->type)
    {
        case TOK_OP_EEQ: return codegen_branch(self, "beq");
        case TOK_OP_NEQ: return codegen_branch(self, "bne");
        case TOK_OP_GE:  return codegen_branch(self, "bge");
        case TOK_OP_GT:  return codegen_branch(self, "bgt");
        case TOK_OP_LE:  return codegen_branch(self, "ble");
        case TOK_OP_LT:  return codegen_branch(self, "blt");
        default:         FATAL("invalid binary operator");
    }
}

static codegen_t codegen_emit_call(codegen_t self, ast_t ast)
{
    ASSERT(self && self->func && ast && ast->type == AST_CALL, "invalid args");
    ast_call call   = ANY_CAST(ast);
    string_t caller = self->func->name->token->lexeme;
    string_t callee = call->name->token->lexeme;

    // emit code to push arguments (right to left)
    for (size_t i = call->args->size; i >= 1; i--)
    {
        ast_t arg = call->args->nodes[i - 1];
        ASSERT(arg != NULL, "invalid arg");
        switch (arg->type)
        {
            case AST_NAME:
            {
                string_t name     = ((ast_name) arg)->token->lexeme;
                symbol_t variable = symtable_get_symbol(self->context, caller, name);
                ASSERT(variable  != NULL, "invalid variable");
                emitter_emit_opcode(self->emitter, "ldr", "r0, [fp, #%d]", (int) variable->offset);
                emitter_emit_opcode(self->emitter, "push", "{r0}");
                break;
            }
            case AST_NUMBER:
            {
                uint64_t number = ((ast_number) arg)->value;
                emitter_emit_opcode(self->emitter, "mov",  "r0, #%d", (int) number);
                emitter_emit_opcode(self->emitter, "push", "{r0}");
                break;
            }
            case AST_STRING:
            {
                string_t label  = emitter_make_label(self->emitter);
                string_t string = ((ast_string) arg)->value;
                emitter_emit_data_label(self->emitter, label);
                emitter_emit_string(self->emitter, string->chars);
                emitter_emit_opcode(self->emitter, "ldr", "r0, =%s", label->chars);
                emitter_emit_opcode(self->emitter, "push", "{r0}");
                string_delete(label);
                break;
            }
            default:
            {
                FATAL("invalid arg");
                break;
            }
        }
    }

    // bytes allocated for args
    const size_t bytes = (4 * call->args->size);

    // branch and link to the target function
    emitter_emit_opcode(self->emitter, "bl", "%s", callee->chars);
    emitter_emit_opcode(self->emitter, "add", "sp, sp, #%d", (int) (bytes));
    return self;
}

static codegen_t codegen_emit_func(codegen_t self, ast_t ast)
{
    ASSERT(self && ast && ast->type == AST_FUNC, "invalid args");
    ast_func func = ANY_CAST(ast);

    // function name
    emitter_emit_code(self->emitter, "");
    emitter_emit_code(self->emitter, "%s:", func->name->token->lexeme->chars);

    // prolog
    emitter_emit_opcode(self->emitter, "push", "{fp, lr}");
    emitter_emit_opcode(self->emitter, "mov", "fp, sp");

    // allocate space for local variables
    const size_t bytes = 4 * func->vars->size;
    emitter_emit_opcode(self->emitter, "sub", "sp, sp, #%d", (int) (bytes));

    // emit body
    ast_func enclosing = self->func;
    self->func         = func;
    codegen_emit(self, (ast_t) func->stmts);
    self->func         = enclosing;

    return self;
}

static codegen_t codegen_emit_if(codegen_t self, ast_t ast)
{
    ASSERT(self && ast && ast->type == AST_IF, "invalid args");
    ast_if if_stmt = ANY_CAST(ast);

    // evaluate condition - result in r0
    codegen_emit(self, if_stmt->expr);

    // test condition
    emitter_emit_opcode(self->emitter, "cmp", "r0, #0");

    // false - skip block
    string_t exit_label = emitter_make_label(self->emitter);
    emitter_emit_opcode(self->emitter, "beq", "%s", exit_label->chars);

    // true - execute block
    codegen_emit(self, (ast_t) if_stmt->block);

    // emit exit label
    emitter_emit_code_label(self->emitter, exit_label);
    string_delete(exit_label);
    return self;
}

static codegen_t codegen_emit_list(codegen_t self, ast_t ast)
{
    ASSERT(self && ast && ast->type == AST_LIST, "invalid args");
    ast_list list = ANY_CAST(ast);
    for (size_t i = 0; i < list->size; i++)
        codegen_emit(self, list->nodes[i]);
    return self;
}

static codegen_t codegen_emit_name(codegen_t self, ast_t ast)
{
    ASSERT(self && self->func && ast && ast->type == AST_NAME, "invalid args");
    ast_name name = ANY_CAST(ast);
    symbol_t symbol = symtable_get_symbol(self->context,
                                          self->func->name->token->lexeme,
                                          name->token->lexeme);
    ASSERT(symbol != NULL, "invalid symbol");

    const int offset = (int) symbol->offset;
    const int reg    = (int) self->nn_reg;
    emitter_emit_opcode(self->emitter, "ldr", "r%d, [fp, #%d]", reg, offset);
    return self;
}

static codegen_t codegen_emit_number(codegen_t self, ast_t ast)
{
    ASSERT(self && ast && ast->type == AST_NUMBER, "invalid args");
    const int number = (int) ((ast_number) ast)->value;
    const int reg    = (int) self->nn_reg;
    emitter_emit_opcode(self->emitter, "ldr", "r%d, =%d", reg, number);
    return self;
}

static codegen_t codegen_emit_return(codegen_t self, ast_t ast)
{
    ASSERT(self && ast && ast->type == AST_RETURN, "invalid args");

    // emit value
    ast_return ret = ANY_CAST(ast);
    codegen_emit(self, ret->expr);

    // emit epilog - remove stack space reserved for variables
    emitter_emit_opcode(self->emitter, "mov", "sp, fp");

    // emit epilog - pop fp from lr from stack
    emitter_emit_opcode(self->emitter, "pop", "{fp, lr}");

    //emit epilog - return to caller
    emitter_emit_opcode(self->emitter, "bx",  "lr");
    return self;
}

static codegen_t codegen_emit_string(codegen_t self, ast_t ast)
{
    ASSERT(self && ast && ast->type == AST_STRING, "invalid args");
    FATAL("should never visit ast_string");
}

static codegen_t codegen_emit_while(codegen_t self, ast_t ast)
{
    ASSERT(self && ast && ast->type == AST_WHILE, "invalid args");
    ast_while while_stmt = ANY_CAST(ast);
    string_t  start_label = emitter_make_label(self->emitter);
    string_t  exit_label  = emitter_make_label(self->emitter);

    // loop label
    emitter_emit_code_label(self->emitter, start_label);

    // test condition
    codegen_emit(self, while_stmt->expr);
    emitter_emit_opcode(self->emitter, "cmp", "r0, #0");

    // false - exit
    emitter_emit_opcode(self->emitter, "beq", "%s", exit_label->chars);

    // true - loop to start
    codegen_emit(self, (ast_t) while_stmt->block);
    emitter_emit_opcode(self->emitter, "b", start_label->chars);
    emitter_emit_code_label(self->emitter, exit_label);

    string_delete(start_label);
    string_delete(exit_label);
    return self;
}

// =============================================================================
// arm v7 code generator impl
// =============================================================================

emitter_t gen_armv7asm(symtable_t symtable)
{
    codegen_t codegen = codegen_create(symtable);
    codegen_emit(codegen, (ast_t) codegen->context->functions);
    emitter_t emitter = codegen_intrinsics(codegen)->emitter;

    codegen->emitter = NULL;
    codegen_delete(codegen);
    return emitter;
}
