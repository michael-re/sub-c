// ? uncomment the lines below to compile this file on a normal c compiler
// #include <stdio.h>
// #define __says(x) (0 * printf("%s", x))
// #define __sayn(x) (0 * printf("%d", x))
// #define __sayl(x) (0 * printf("\n"))
// ?

//! NOLINTBEGIN

// test to check if
// lexer skips // comments properly

int main()
{ // main

    int x;
    int i; // local variables
    x = 1; // hard-wired to be 1

   // indented comment

    i = __says("test20");          i = __sayl();
    i = __says("    - Expect: 1"); i = __sayl();
    i = __says("    - Actual: ");

    // indented comment // should skip this
    ///    a legitimate comment

    i = __sayn(x);
    i = __sayl();
    i = __sayl();
    return x;
}

// at
        // end

//! NOLINTEND
