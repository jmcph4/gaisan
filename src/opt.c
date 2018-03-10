/**
 * @file opt.c
 * @author Jack McPherson
 *
 * Implements methods for optimising real-valued functions of various
 * dimensions.
 *
 * */
#include <stdlib.h>
#include <math.h>

#include "constants.h"
#include "diff.h"
#include "opt.h"

long double golden_section_min(long double a, long double b,
        long double (*f)(long double), long double tol)
{
    if(f == NULL) /* null guard */
    {
        return NAN;
    }

    if(tol <= 0)
    {
        return NAN;
    }

    if(a == b)
    {
        return NAN;
    }

    long double min = 0.0;

    long double x_1 = a;
    long double x_2 = b;
    long double x_3 = 0.0;
    long double x_4 = 0.0;

    long double df = 0.0; /* derivative at current minimum */

    do
    {
        x_3 = (x_1 + x_2) / GOLDEN_RATIO;

        /* choose x_4 */
        if(fabs(x_1 - x_3) > fabs(x_2 - x_3)) /* left */
        {
            x_4 = (x_1 + x_3) / 2;

            /* tighten bounds */
            if(f(x_3) < f(x_4)) /* right */
            {
                min = x_3;
                x_1 = x_4;
                df = cdiff(min, f, DEFAULT_STEP_SIZE);
                continue;
            }
            else /* left */
            {
                min = x_4;
                x_2 = x_3;
                df = cdiff(min, f, DEFAULT_STEP_SIZE);
                continue;
            }
        }
        else /* right */
        {
            x_4 = (x_2 + x_3) / 2;

            /* tighten bounds */
            if(f(x_3) < f(x_4)) /* left */
            {
                min = x_3;
                x_2 = x_4;
                df = cdiff(min, f, DEFAULT_STEP_SIZE);
                continue;
            }
            else /* right */
            {
                min = x_4;
                x_1 = x_3;
                df = cdiff(min, f, DEFAULT_STEP_SIZE);
                continue;
            }
        }
    }
    while(fabs(df) > tol);

    return min;
}


