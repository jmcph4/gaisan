#include <stdlib.h>
#include <math.h>

#include "diff.h"

long double fdiff(long double x, long double (*f)(long double), long double h)
{
    if(f == NULL)
    {
        return NAN;
    }

    if(h == 0.0)
    {
        return NAN;
    }

    return (f(x + h) - f(x)) / h;
}

long double bdiff(long double x, long double (*f)(long double), long double h)
{
    if(f == NULL)
    {
        return NAN;
    }

    if(h == 0.0)
    {
        return NAN;
    }

    return (f(x) - f(x - h)) / h;
}

long double cdiff(long double x, long double (*f)(long double), long double h)
{
    if(f == NULL)
    {
        return NAN;
    }

    if(h == 0.0)
    {
        return NAN;
    }

    return (f(x + h) - f(x - h)) / (2 * h);
}

