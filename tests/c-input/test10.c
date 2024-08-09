// ? uncomment the lines below to compile this file on a normal c compiler
// #include <stdio.h>
// #define __says(x) (0 * printf("%s", x))
// #define __sayn(x) (0 * printf("%d", x))
// #define __sayl(x) (0 * printf("\n"))
// ?

//! NOLINTBEGIN

int max(int x, int y)
{
    if (x >  y) return x;
    if (x <= y) return y;
    return 0 - 999;
}

int main()
{
    int a;
    int b;
    int ans;

    a = 3;
    b = 4;

    __says("test10");          __sayl();
    __says("    - Expect: 4"); __sayl();
    __says("    - Actual: ");

    ans = max(a, b);
    __sayn(ans);
    __sayl();
    __sayl();

    return 10;
}

//! NOLINTEND
