#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "root.h"

/* some functions to choose from */

long double x_squared(long double x)
{
    return x * x;
}

long double right_shift_x(long double x)
{
    return x + 1;
}

int main(int argc, char** argv)
{
    if(argc != 4) /* usage */
    {
        fprintf(stderr, "usage: ex_bisect a b tol\n");
        return EXIT_FAILURE;
    }

    long double a = atof(argv[1]);
    long double b = atof(argv[2]);
    long double tol = atof(argv[3]);

    printf("Select a function:\n");
    printf("    1. f(x)=x^2\n");
    printf("    2. f(x)=cos(x)\n");
    printf("    3. f(x)=x+1\n");

    /* get user's selection */
    unsigned int choice = 0;
    int elems = scanf("%u", &choice);

    if(elems != 1) /* invalid input */
    {
        return EXIT_FAILURE;
    }

    long double (*f)(long double);

    switch(choice)
    {
        case 1:
            f = &x_squared;
            break;
        case 2:
            f = &cosl;
            break;
        case 3:
            f = &right_shift_x;
            break;
        default:
            fprintf(stderr, "Invalid option.\n");
            return EXIT_FAILURE;
            break;
    }

    /* calculate root */
    long double root = bisect(a, b, f, tol);
    printf("%Lf\n", root);

    return EXIT_SUCCESS;

}

