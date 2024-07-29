#include "subc/lexer.h"

// =============================================================================
// fstream lexer impl
// =============================================================================

tkstream_t lex_tokens(ifstream_t source)
{
    ifstream_delete(source);
    return FATAL("lex_tokens unimplemented");
}
