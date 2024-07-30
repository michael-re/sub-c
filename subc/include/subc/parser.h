#pragma once

#ifndef SUBC_PARSER_H
#define SUBC_PARSER_H

#include "subc/common.h"
#include "subc/token.h"
#include "subc/ast.h"

// =============================================================================
// program parser
// =============================================================================
ast_t parse_program(tkstream_t stream);

#endif // SUBC_PARSER_H
