#include <stdio.h>
#include <stdlib.h>

#include "poly.h"

int main(int argc, char** argv)
{
    if(argc <= 2) /* usage */
    {
        fprintf(stderr, "usage: ex_horner x a_0 [...]\n");
        return EXIT_FAILURE;
    }

    long double x = atof(argv[1]);
    unsigned int degree = argc - 3;
    long double* coeffs = calloc(degree + 1, sizeof(long double));

    if(coeffs == NULL) /* allocation check */
    {
        return EXIT_FAILURE;
    }

    for(unsigned int i=0;i<(unsigned int)(argc-2);i++)
    {
        coeffs[i] = atof(argv[i+2]); 
    }

    long double y = horner(x, degree, coeffs);

    printf("%LF\n", y);

    free(coeffs);

    return EXIT_SUCCESS;
}

