// ? uncomment the lines below to compile this file on a normal c compiler
// #include <stdio.h>
// #define __says(x) (0 * printf("%s", x))
// #define __sayn(x) (0 * printf("%d", x))
// #define __sayl(x) (0 * printf("\n"))
// ?

//! NOLINTBEGIN

// test21.c - 4 local variables

int main()
{
    int a;
    int b;
    int c;

    a = 1;
    b = 2;
    c = 3;

    __says("test21");              __sayl();
    __says("    - Expect: 1 2 3"); __sayl();
    __says("    - Actual: ");

    __sayn(a); __says(" ");
    __sayn(b); __says(" ");
    __sayn(c); __says(" ");
    __sayl();
    __sayl();

    return 21;
}

//! NOLINTEND
