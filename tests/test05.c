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

    a = 7;
    b = 999;

    __says("test05");                 __sayl();
    __says("    - Expect: 'a != b'"); __sayl();
    __says("    - Actual: ");

    if (a == b) __says("'a == b'");
    if (a != b) __says("'a != b'");

    __sayl();
    __sayl();

    return 5;
}

//! NOLINTEND
