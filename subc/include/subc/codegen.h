#pragma once

#ifndef SUBC_CODEGEN_H
#define SUBC_CODEGEN_H

#include "subc/common.h"
#include "subc/emitter.h"
#include "subc/table.h"

// =============================================================================
// arm v7 code generator
// =============================================================================
emitter_t gen_armv7asm(symtable_t symtable);

#endif // SUBC_CODEGEN_H
