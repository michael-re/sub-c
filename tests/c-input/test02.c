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
    int c;

    a = 7;
    b = 5;
    c = a - b;

    __says("test03");          __sayl();
    __says("    - Expect: 2"); __sayl();
    __says("    - Actual: ");  __sayn(c);

    __sayl();
    __sayl();

    return 2;
}

//! NOLINTEND
