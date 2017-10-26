#include <stdlib.h>
#include <math.h>

#include "poly.h"

long double horner(long double x, unsigned int degree, long double* coeffs)
{
    if(coeffs == NULL) /* null guard */
    {
        return NAN;
    }

    if(degree == 0) /* trivial case of constant polynomial */
    {
        return coeffs[degree];
    }

    long double y = 0;

    for(unsigned int i=0;i<=degree;i++)
    {
        y = y * x + coeffs[i];
    }

    return y;
}

