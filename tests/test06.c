// ? uncomment the lines below to compile this file on a normal c compiler
// #include <stdio.h>
// #define __says(x) (0 * printf("%s", x))
// #define __sayn(x) (0 * printf("%d", x))
// #define __sayl(x) (0 * printf("\n"))
// ?

//! NOLINTBEGIN

int main()
{
    int a;
    int b;

    a = 0;
    b = 5;

    __says("test06");                  __sayl();
    __says("    - Expect: 0 1 2 3 4"); __sayl();
    __says("    - Actual: ");

    while (a < b)
    {
        __sayn(a);
        __says(" ");
        a = a + 1;
    }

    __sayl();
    __sayl();

    a = 6;
    return a;
}

//! NOLINTEND
