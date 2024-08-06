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
    x = 1;

    __says("test01");          __sayl();
    __says("    - Expect: 1"); __sayl();
    __says("    - Actual: ");  __sayn(x);

    __sayl();
    __sayl();

    return x;
}

//! NOLINTEND
