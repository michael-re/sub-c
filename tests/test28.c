// ? uncomment the lines below to compile this file on a normal c compiler
// #include <stdio.h>
// #define __says(x) (0 * printf("%s", x))
// #define __sayn(x) (0 * printf("%d", x))
// #define __sayl(x) (0 * printf("\n"))
// ?

//! NOLINTBEGIN

int main()
{
    int x;
    int y;
    int z;

    x = 3;
    y = 4 - 4;

    z = x + y;
    z = y + x;

    z = 4 * z;
    z = z * 8;

    return 28;
}

//! NOLINTEND
