// ? uncomment the lines below to compile this file on a normal c compiler
// #include <stdio.h>
// #define __says(x) (0 * printf("%s", x))
// #define __sayn(x) (0 * printf("%d", x))
// #define __sayl(x) (0 * printf("\n"))
// ?

//! NOLINTBEGIN

// test23.c - 4 local variables + 4 parameters

int sub(int a, int b, int c, int d)
{
    __sayn(a); __says(" ");
    __sayn(b); __says(" ");
    __sayn(c); __says(" ");
    __sayn(d); __says(" ");
    return 0;
}

int main()
{
    int aa;
    int bb;
    int cc;
    int dd;

    aa = 1;
    bb = 2;
    cc = 3;
    dd = 4;

    __says("test23");                __sayl();
    __says("    - Expect: 1 2 3 4"); __sayl();
    __says("    - Actual: ");

    sub(aa, bb, cc, dd);
    __sayl();
    __sayl();

    return 23;
}

//! NOLINTEND
