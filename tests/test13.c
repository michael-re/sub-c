// ? uncomment the lines below to compile this file on a normal c compiler
// #include <stdio.h>
// #define __says(x) (0 * printf("%s", x))
// #define __sayn(x) (0 * printf("%d", x))
// #define __sayl(x) (0 * printf("\n"))
// ?

//! NOLINTBEGIN

int plus2(int b)
{
    return b + 2;
}

int one(int a)
{
    int res;
    res = plus2(a);
    return res;
}

int main()
{
    int x;

    __says("test13");           __sayl();
    __says("    - Expect: 77"); __sayl();
    __says("    - Actual: ");

    x = one(75);
    __sayn(x);
    __sayl();
    __sayl();

    return 13;
}

//! NOLINTEND