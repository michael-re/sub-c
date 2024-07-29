#pragma once

#ifndef SUBC_LEXER_H
#define SUBC_LEXER_H

#include "subc/common.h"
#include "subc/fstream.h"
#include "subc/token.h"

// =============================================================================
// fstream lexer
// =============================================================================
tkstream_t lex_tokens(ifstream_t source);

#endif // SUBC_LEXER_H
