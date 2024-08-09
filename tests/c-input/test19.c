// ? uncomment the lines below to compile this file on a normal c compiler
// #include <stdio.h>
// #define __says(x) (0 * printf("%s", x))
// #define __sayn(x) (0 * printf("%d", x))
// #define __sayl(x) (0 * printf("\n"))
// ?

//! NOLINTBEGIN

int five(int p0, int p1, int p2, int p3, int p4)
{
    int n;
    n = p0 + p1;
    n = n + p2;
    n = n + p3;
    n = n + p4;
    return n;
}

int main()
{
    int s;
    s = five(0, 1, 2, 3, 4);

    __says("test19");           __sayl();
    __says("    - Expect: 10"); __sayl();
    __says("    - Actual: ");

    __sayn(s);
    __sayl();
    __sayl();

    return 19;
}

//! NOLINTEND
