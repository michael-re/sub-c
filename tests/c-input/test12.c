// ? uncomment the lines below to compile this file on a normal c compiler
// #include <stdio.h>
// #define __says(x) (0 * printf("%s", x))
// #define __sayn(x) (0 * printf("%d", x))
// #define __sayl(x) (0 * printf("\n"))
// ?

//! NOLINTBEGIN

int add(int a, int b)
{
    int x;
    x = a + b;
    return x;
}

int main()
{
    int m;
    int n;
    int y;
    int i;

    m = 123;
    n = 456;

    i = __says("test12");            i = __sayl();
    i = __says("    - Expect: 579"); i = __sayl();
    i = __says("    - Actual: ");

    y = add(m, n);
    i = __sayn(y);
    i = __sayl();
    i = __sayl();

    return 12;
}

//! NOLINTEND
