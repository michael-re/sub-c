// ? uncomment the lines below to compile this file on a normal c compiler
// #include <stdio.h>
// #define __says(x) (0 * printf("%s", x))
// #define __sayn(x) (0 * printf("%d", x))
// #define __sayl(x) (0 * printf("\n"))
// ?

//! NOLINTBEGIN

int main()
{
    int counter;
    int test;

    __says("test07");          __sayl();
    __says("    - Expect: 3"); __sayl();
    __says("    - Actual: ");

    counter = 1;
    while (counter < 5)
    {
        test = counter == 3;
        if (test == 1) __sayn(counter);
        counter = counter + 1;
    }

    __sayl();
    __sayl();

    return 7;
}

//! NOLINTEND
