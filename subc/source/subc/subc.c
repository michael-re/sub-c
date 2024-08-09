// #define SUBC_DEBUG_MODE
#include "subc/subc.h"

int main(const int argc, const char* argv[])
{
    const char* input  = argc >= 2 ? argv[1] : NULL;
    const char* output = argc >= 3 ? argv[2] : input;

    if (input == NULL)
    {
        printf("\nusage: ./subc <file> \n\n");
        return EX_DATA_ERROR;
    }

    ifstream_t file   = open_file   (input);
    tkstream_t tokens = tokenize    (input, file);
    ast_t      ast    = parse       (input, tokens);
    symtable_t table  = analyze     (input, ast);
    emitter_t  armv7  = gen_armv7asm(table);

    if (!armv7)
    {
        printf("\nerror generating arm-v7 asm for '%s'\n\n", input);
        return EX_DATA_ERROR;
    }

    if (!emitter_save(armv7, output))
    {
        printf("\nerror writing generated arm-v7 asm file to disk\n\n");
        return EX_IO_ERROR;
    }

    return EX_SUCCESS;
}

ifstream_t open_file(const char* name)
{
    ifstream_t file = ifstream_create(name);
    if (!file)
    {
        printf("\nerror opening file '%s'\n\n", name ? name : "????");
        exit(EX_IO_ERROR);
    }

    if (DEBUG_OPEN_FILE)
    {
        printf("\n=== DEBUG file stream beg ===\n");
        printf("%.*s\n", (int) file->size, file->data);
        printf("=== DEBUG file stream end ===\n\n");
    }

    return file;
}

tkstream_t tokenize(const char* name, ifstream_t source)
{
    tkstream_t tokens = lex_tokens(source);
    if (!tokens)
    {
        printf("\nerror tokenizing '%s'\n\n", name ? name : "????");
        exit(EX_DATA_ERROR);
    }

    if (DEBUG_TOKENIZE)
    {
        printf("=== DEBUG token stream beg ===\n\n");
        for (size_t i = 0; i < tokens->size; i++)
        {
            token_t token = tokens->tokens[i];
            ASSERT(token       != NULL,    "corrupted token stream");
            ASSERT(token->type != TOK_BAD, "corrupted token stream");
            printf("[ %02d %-16s ] ", token->type, tok_type_string(token->type));
            printf("(%04zd:%04zd) ",  token->line, token->column);
            printf("%s\n",            token->lexeme->chars);
        }
        printf("=== DEBUG token stream end ===\n\n");
    }

    return tokens;
}

ast_t parse(const char* name, tkstream_t source)
{
    ast_t ast = parse_program(source);
    if (!ast)
    {
        printf("\nerror parsing '%s'\n\n", name ? name : "????");
        exit(EX_DATA_ERROR);
    }

    if (DEBUG_PARSE)
    {
        printf("=== DEBUG abstract syntax tree beg ===\n\n");
        pretty_print_ast(ast);
        printf("=== DEBUG abstract syntax tree end ===\n\n");
    }

    return ast;
}

symtable_t analyze(const char* name, ast_t source)
{
    symtable_t table = symtable_create(source);
    if (!table)
    {
        printf("\nerror generating symbol table for '%s'\n\n", name ? name : "????");
        exit(EX_DATA_ERROR);
    }

    if (DEBUG_ANALYZE)
    {
                printf("\n=== DEBUG symbol table beg ===\n");
        for (size_t i = 0; i < table->indicies->size; i++)
        {
            const layout_t layout = table->layouts[i];
            ASSERT(layout != NULL, "invalid symbol table");

            printf("\n=====================================\n");
            printf("| layout(%zd:%zd): '%s' \n", layout->function->name->token->line,
                                                 layout->function->name->token->column,
                                                 layout->function->name->token->lexeme->chars);

            printf("+---------+---------+---------+-----\n");
            printf("| type    | role    | offset  | name\n");
            printf("+---------+---------+---------+-----\n");
            for (size_t j = 0; j < table->layouts[i]->indicies->size; j++)
            {
                const symbol_t symbol = &layout->symbols[j];
                const char*    type   = sym_type_string(symbol->type);
                const char*    role   = sym_role_string(symbol->role);
                const int64_t  offset = symbol->offset;
                printf("| %s   %s   %04" PRId64 "      '%s'\n", type, role, offset, symbol->name->lexeme->chars);
            }

            printf("| index: %04zd\n", layout->offset);
        }
        printf("\n=== DEBUG symbol table end ===\n\n");
    }

    return table;
}
