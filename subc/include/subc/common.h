#pragma once

#ifndef SUBC_COMMON_H
#define SUBC_COMMON_H

//! NOLINTBEGIN

#include <limits.h>
#include <assert.h>
#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

// =============================================================================
// exit code
// =============================================================================
enum exit_code
{
    EX_SUCCESS       =  0, //* successful execution of program
    EX_FAILURE       =  1, //* unsuccessful execution of program
    EX_USAGE         = 64, //* command line usage error
    EX_DATA_ERROR    = 65, //* data format error
    EX_NO_INPUT      = 66, //* cannot open input
    EX_NO_USER       = 67, //* addressee unknown
    EX_NO_HOST       = 68, //* host name unknown
    EX_UNAVAILABLE   = 69, //* service unavailable
    EX_SOFTWARE      = 70, //* internal software error
    EX_OS_ERROR      = 71, //* system error (e.g., can't fork)
    EX_OS_FILE       = 72, //* critical os file missing
    EX_CANT_CREATE   = 73, //* can't create (user) output file
    EX_IO_ERROR      = 74, //* input/output error
    EX_TEMP_FAIL     = 75, //* temp failure; user is invited to retry
    EX_PROTOCOL      = 76, //* remote error in protocol
    EX_NO_PERMISSION = 77, //* permission denied
    EX_CONFIG        = 78, //* configuration error
};

//! NOLINTEND

#endif // SUBC_COMMON_H
