// ? uncomment the lines below to compile this file on a normal c compiler
// #include <stdio.h>
// #define __says(x) (0 * printf("%s", x))
// #define __sayn(x) (0 * printf("%d", x))
// #define __sayl(x) (0 * printf("\n"))
// ?

//! NOLINTBEGIN

int add(int x, int y)
{
    return x + y;
}

int swap(int a, int b)
{
    int i;
    i = add(b, a);
    return i;
}

int main()
{
    int i;

    __says("test27");          __sayl();
    __says("    - Expect: 8"); __sayl();
    __says("    - Actual: ");

    i = swap(7, 1);
    __sayn(i);
    __sayl();
    return 27;
}

//! NOLINTEND
