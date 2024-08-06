// ? uncomment the lines below to compile this file on a normal c compiler
// #include <stdio.h>
// #define __says(x) (0 * printf("%s", x))
// #define __sayn(x) (0 * printf("%d", x))
// #define __sayl(x) (0 * printf("\n"))
// ?

//! NOLINTBEGIN

// test24.c : 6 variables

int main()
{
    int a;
    int b;
    int c;
    int d;
    int e;

    a = 1;
    b = 2;
    c = 3;
    d = 4;
    e = 5;

    __says("test24");                  __sayl();
    __says("    - Expect: 1 2 3 4 5"); __sayl();
    __says("    - Actual: ");

    __sayn(a); __says(" ");
    __sayn(b); __says(" ");
    __sayn(c); __says(" ");
    __sayn(d); __says(" ");
    __sayn(e); __says(" ");
    __sayl();
    __sayl();

    return 24;
}

//! NOLINTEND
