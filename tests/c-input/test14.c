// ? uncomment the lines below to compile this file on a normal c compiler
// #include <stdio.h>
// #define __says(x) (0 * printf("%s", x))
// #define __sayn(x) (0 * printf("%d", x))
// #define __sayl(x) (0 * printf("\n"))
// ?

//! NOLINTBEGIN

int fib(int n)
{
    int nm1;
    int nm2;
    int fm1;
    int fm2;

    if (n == 0) { return 0; }
    if (n == 1) { return 1; }

    nm1 = n - 1;
    fm1 = fib(nm1);

    nm2 = n - 2;
    fm2 = fib(nm2);
    return fm1 + fm2;
}

int main()
{
    int f;

    __says("test14");            __sayl();
    __says("    - Expect: 377"); __sayl();
    __says("    - Actual: ");

    f = fib(14);
    __sayn(f);
    __sayl();
    __sayl();

    return 14;
}

//! NOLINTEND
