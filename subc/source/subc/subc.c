#include "subc/subc.h"

int main(const int argc, const char* argv[])
{
    const char* input = argc >= 2 ? argv[1] : NULL;

    if (input == NULL)
    {
        printf("\nusage: ./subc <file> \n\n");
        return EX_DATA_ERROR;
    }

    ifstream_t file   = open_file(input);
    tkstream_t tokens = tokenize(input, file);

    tkstream_delete(tokens);
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
        printf("\n=== DEBUG open file beg ===\n");
        printf("%.*s\n", (int) file->size, file->data);
        printf("=== DEBUG open file end ===\n\n");
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
        printf("=== DEBUG tokenize beg ===\n\n");
        FATAL("debug tokenize unimplemented");
        printf("=== DEBUG tokenize end ===\n\n");
    }

    return tokens;
}
