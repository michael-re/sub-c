// ? uncomment the lines below to compile this file on a normal c compiler
// #include <stdio.h>
// #define __says(x) (0 * printf("%s", x))
// #define __sayn(x) (0 * printf("%d", x))
// #define __sayl(x) (0 * printf("\n"))
// ?

//! NOLINTBEGIN

int bigger(int a, int b)
{
    int res;
    res = a - b;

    if (res >= 0)
        return 1;

    return 0 - 1;
}

int main()
{
    int result;

    __says("test08");             __sayl();
    __says("    - Expect: '>='"); __sayl();
    __says("    - Actual: ");

    result = bigger(9, 4);
    if (result >= 0) __says("'>='");
    if (result <  0) __says("'<'");

    __sayl();
    __sayl();

    return 8;
}

//! NOLINTEND
