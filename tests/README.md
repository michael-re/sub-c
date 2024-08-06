# Sub-C Test Files

This directory contains **example Sub-C programs** that are compatible with
the strict subset of C supported by the sub-c compiler. These example programs
are used to demonstrate and test the capabilities of the compiler.

## Compilation on a Normal C Compiler

If you wish to compile these example programs using a standard C compiler
(e.g., GCC, Clang, or MSVC), you'll need to define the following intrinsics:

```c
#include <stdio.h>
#define __says(x) (0 * printf("%s", x))
#define __sayn(x) (0 * printf("%d", x))
#define __sayl(x) (0 * printf("\n"))
```

These intrinsics simulate specific output functions within the Sub-C language.
However, they have been commented out in all test files in this directory.
To compile the example programs using a normal C compiler, you must uncomment
these lines in each test file.
