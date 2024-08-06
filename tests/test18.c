// ? uncomment the lines below to compile this file on a normal c compiler
// #include <stdio.h>
// #define __says(x) (0 * printf("%s", x))
// #define __sayn(x) (0 * printf("%d", x))
// #define __sayl(x) (0 * printf("\n"))
// ?

//! NOLINTBEGIN

int main()
{
    int actual;
    actual = 999;

    __says("test18");          __sayl();
    __says("    - Expect: 9"); __sayl();
    __says("    - Actual: ");

    if (3 == 3)
        actual = 9;

    __sayn(actual);
    __sayl();
    __sayl();

    return 18;
}

//! NOLINTEND
