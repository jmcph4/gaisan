#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>

#include "monte.h"

#define D 2

bool within(long double* vec, unsigned int dim)
{
    if(vec == NULL)
    {
        return false;
    }

    if(dim != 2)
    {
        return false;
    }

    return vec[0]*vec[0] + vec[1]*vec[1] <= 1;
}

int main(int argc, char** argv)
{
    if(argc != 2) /* usage */
    {
        fprintf(stderr, "usage: ex_monte_carlo n\n");
        return EXIT_FAILURE;
    }

    unsigned int n = atoi(argv[1]); /* number of points */

    /* allocate space for domain array */
    long double** domain = calloc(D, sizeof(long double*));

    if(domain == NULL)
    {
        perror("calloc");
        return EXIT_FAILURE;
    }

    for(unsigned int i=0;i<D;i++)
    {
        domain[i] = calloc(D, sizeof(long double));

        if(domain[i] == NULL)
        {
            perror("calloc");
            return EXIT_FAILURE;
        }
    }

    /* hard-code domain */
    domain[0][0] = 0;
    domain[0][1] = 10;
    domain[1][0] = 0;
    domain[1][1] = 10;

    long double area = monte_carlo(&within, D, domain, n);

    if(area == NAN)
    {
        perror("monte_carlo");
        return EXIT_FAILURE;
    }

    printf("%Lf\n", area);

    free(domain); /* tidy up */

    return EXIT_SUCCESS;
}

