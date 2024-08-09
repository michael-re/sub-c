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
    int r;
    r = 3;

    __says("test09");          __sayl();
    __says("    - Expect: 7"); __sayl();
    __says("    - Actual: ");

    r = add(r, 4);
    __sayn(r);
    __sayl();
    __sayl();

    return 9;
}

//! NOLINTEND
