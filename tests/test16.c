// ? uncomment the lines below to compile this file on a normal c compiler
// #include <stdio.h>
// #define __says(x) (0 * printf("%s", x))
// #define __sayn(x) (0 * printf("%d", x))
// #define __sayl(x) (0 * printf("\n"))
// ?

//! NOLINTBEGIN

int sub1(int p0, int p1, int p2, int p3, int p4, int p5)
{
    int x;
    x = p0 + p1;
    x = x - p2;
    x = x - p3;
    x = x + p4;
    x = x + p5;
    return x;
}

int sub2(int p0, int p1, int p2, int p3, int p4, int p5)
{
    int y;
    y = sub1(p5, p4, p3, p2, p1, p0);
    return y;
}

int main()
{
    int v0;
    int v1;
    int v2;
    int v3;
    int v4;
    int v5;
    int v6;

    v0 = 16;
    v1 = 17;
    v2 = 18;
    v3 = 19;
    v4 = 20;
    v5 = 21;

    __says("test16");           __sayl();
    __says("    - Expect: 37"); __sayl();
    __says("    - Actual: ");

    v6 = sub2(v0, v1, v2, v3, v4, v5);
    __sayn(v6);
    __sayl();
    __sayl();

    return 16;
}

//! NOLINTEND
