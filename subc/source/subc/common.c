#include <stdio.h>
#include <stdlib.h>

#include "subc/common.h"

void* fatal_error(const char*  reason,
                  const char*  file,
                  const char*  function,
                  const size_t line)
{
    assert(reason   != NULL);
    assert(file     != NULL);
    assert(function != NULL);

    //! NOLINTBEGIN
    fprintf(stderr, "\n");
    fprintf(stderr, "fatal error in file: %s:%zd\n", file, line);
    fprintf(stderr, "           function: %s\n",     function);
    fprintf(stderr, "               line: %zd\n",    line);
    fprintf(stderr, "             reason: %s\n",     reason);
    fprintf(stderr, "\n");
    //! NOLINTEND

    abort();
    return NULL;
}

void* assert_error(const char*  condition,
                   const char*  reason,
                   const char*  file,
                   const char*  function,
                   const size_t line)
{
    assert(condition != NULL);
    assert(reason    != NULL);
    assert(file      != NULL);
    assert(function  != NULL);

    //! NOLINTBEGIN
    fprintf(stderr, "\n");
    fprintf(stderr, "assertion failed in file: %s:%zd\n", file, line);
    fprintf(stderr, "                function: %s\n",     function);
    fprintf(stderr, "                    line: %zd\n",    line);
    fprintf(stderr, "        failed assertion: %s\n",     condition);
    fprintf(stderr, "                  reason: %s\n",     reason);
    fprintf(stderr, "\n");
    //! NOLINTEND

    abort();
    return NULL;
}
