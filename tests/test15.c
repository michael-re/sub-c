// ? uncomment the lines below to compile this file on a normal c compiler
// #include <stdio.h>
// #define __says(x) (0 * printf("%s", x))
// #define __sayn(x) (0 * printf("%d", x))
// #define __sayl(x) (0 * printf("\n"))
// ?

//! NOLINTBEGIN

int fac(int n)
{
    int nm1;
    int ans;

    if (n == 0)
    {
        return 1;
    }

    nm1 = n - 1;
    ans = fac(nm1);
    return n * ans;
}

int main()
{
    int f;

    __says("test15");             __sayl();
    __says("    - Expect: 5040"); __sayl();
    __says("    - Actual: ");

    f = fac(7);
    __sayn(f);
    __sayl();
    __sayl();

    return 15;
}

//! NOLINTEND
