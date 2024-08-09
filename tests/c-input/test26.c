// ? uncomment the lines below to compile this file on a normal c compiler
// #include <stdio.h>
// #define __says(x) (0 * printf("%s", x))
// #define __sayn(x) (0 * printf("%d", x))
// #define __sayl(x) (0 * printf("\n"))
// ?

//! NOLINTBEGIN

int sub1(int a, int b, int c, int d, int e, int f)
{
    __sayl();
    __says("test26: v4");                __sayl();
    __says("    - Expect: 6 5 4 3 2 1"); __sayl();
    __says("    - Actual: ");

    __sayn(a); __says(" ");
    __sayn(b); __says(" ");
    __sayn(c); __says(" ");
    __sayn(d); __says(" ");
    __sayn(e); __says(" ");
    __sayn(f); __says(" ");
    __sayl();

    return 0;
}

int sub2(int a, int b, int c, int d, int e, int f)
{
    int aa;
    int bb;
    int cc;
    int dd;
    int ee;
    int ff;

    __sayl();
    __says("test26: v2");                __sayl();
    __says("    - Expect: 1 2 3 4 5 6"); __sayl();
    __says("    - Actual: ");

    __sayn(a); __says(" ");
    __sayn(b); __says(" ");
    __sayn(c); __says(" ");
    __sayn(d); __says(" ");
    __sayn(e); __says(" ");
    __sayn(f); __says(" ");
    __sayl();

    aa = a;
    bb = b;
    cc = c;
    dd = d;
    ee = e;
    ff = f;

    __sayl();
    __says("test26: v3");                __sayl();
    __says("    - Expect: 1 2 3 4 5 6"); __sayl();
    __says("    - Actual: ");

    __sayn(aa); __says(" ");
    __sayn(bb); __says(" ");
    __sayn(cc); __says(" ");
    __sayn(dd); __says(" ");
    __sayn(ee); __says(" ");
    __sayn(ff); __says(" ");
    __sayl();

    sub1(ff,ee,dd,cc,bb,aa);

    return 0;
}

int main()
{
    int a;
    int b;
    int c;
    int d;
    int e;
    int f;
    int i;

    a = 1;
    b = 2;
    c = 3;
    d = 4;
    e = 5;
    f = 6;

    __says("test26: v1");                __sayl();
    __says("    - Expect: 1 2 3 4 5 6"); __sayl();
    __says("    - Actual: ");

    __sayn(a); __says(" ");
    __sayn(b); __says(" ");
    __sayn(c); __says(" ");
    __sayn(d); __says(" ");
    __sayn(e); __says(" ");
    __sayn(f); __says(" ");
    __sayl();

    sub2(a, b, c, d, e, f);
    __sayl();

    return 26;
}

//! NOLINTEND
