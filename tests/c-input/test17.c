// ? uncomment the lines below to compile this file on a normal c compiler
// #include <stdio.h>
// #define __says(x) (0 * printf("%s", x))
// #define __sayn(x) (0 * printf("%d", x))
// #define __sayl(x) (0 * printf("\n"))
// ?

//! NOLINTBEGIN

int days_in_month(int m)
{
    if (m == 2)  { return 28; }
    if (m == 4)  { return 30; }
    if (m == 6)  { return 30; }
    if (m == 9)  { return 30; }
    if (m == 11) { return 30; }
    return 31;
}

int date_diff(int y1, int m1, int d1, int y2, int m2, int d2)
{
    int days_in_y1;
    int days_in_m1;
    int days_in_date1;
    int days_in_y2;
    int days_in_m2;
    int days_in_date2;

    days_in_y1    = 365 * y1;
    days_in_m1    = days_in_month(m1);
    days_in_date1 = days_in_y1    + days_in_m1;
    days_in_date1 = days_in_date1 + d1;

    days_in_y2    = 365 * y2;
    days_in_m2    = days_in_month(m2);
    days_in_date2 = days_in_y2    + days_in_m2;
    days_in_date2 = days_in_date2 + d2;

    return days_in_date1 - days_in_date2;
}

int main()
{
    int year1;
    int days1;
    int month2;
    int diff;
    int n;
    int i;

    year1  = 51;
    days1  = 7;
    month2 = 4;

    __says("test17");             __sayl();
    __says("    - Expect: 1454"); __sayl();
    __says("    - Actual: ");

    diff = date_diff(year1, 6, days1, 47, month2, 13);
    __sayn(diff);
    __sayl();
    __sayl();

    return diff;
}

//! NOLINTEND
