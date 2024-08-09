// ? uncomment the lines below to compile this file on a normal c compiler
// #include <stdio.h>
// #define __says(x) (0 * printf("%s", x))
// #define __sayn(x) (0 * printf("%d", x))
// #define __sayl(x) (0 * printf("\n"))
// ?

//! NOLINTBEGIN

int fac(int n)
{
    int product;
    int i;

    product = 1;
    i       = 1;

    while (i <= n)
    {
        product = product * i;
        i       = i + 1;
    }

    return product;
}

int main()
{
    int upto;
    int answer;

    __says("test11");             __sayl();
    __says("    - Expect: 5040"); __sayl();
    __says("    - Actual: ");

    upto   = 7;
    answer = fac(upto);

    __sayn(answer);
    __sayl();
    __sayl();

    return 11;
}

//! NOLINTEND
