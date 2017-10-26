#include <stdlib.h>
#include <math.h>

#include "constants.h"
#include "diff.h"
#include "ivp.h"

long double** euler(long double a, long double b, long double y_0,
        long double (*f)(long double, long double), long double h)
{
    if(f == NULL) /* null guard */
    {
        return NULL;
    }

    if(h == 0)
    {
        return NULL;
    }

    if(a == b)
    {
        return NULL;
    }

    unsigned int n = ceil((a + b) / h); /* number of steps */

    /* allocate solution array */
    long double** soln = calloc(2, sizeof(long double*));

    if(soln == NULL) /* allocation check */
    {
        return NULL;
    }

    soln[0] = calloc(n, sizeof(long double));

    if(soln[0] == NULL) /* allocation check */
    {
        free(soln);
        return NULL;
    }

    soln[1] = calloc(n, sizeof(long double));

    if(soln[1] == NULL) /* allocation check */
    {
        free(soln[0]);
        free(soln);
        return NULL;
    }

    /* setup aliases */
    long double* t = soln[0];
    long double* y = soln[1];

    t[0] = a; /* initial point */
    y[0] = y_0; /* initial value */

    /* iterate */
    for(unsigned int i=1;i<n;i++)
    {
        t[i] = t[i-1] + h;
        y[i] = y[i-1] + h * f(t[i], y[i]);
    }

    return soln;
}

