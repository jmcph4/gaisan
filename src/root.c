/**
 * @file root.c
 * @author Jack McPherson
 *
 * Implements rootfinding methods for real-valued functions.
 *
 * */
#include <stdlib.h>
#include <math.h>

#include "constants.h"
#include "diff.h"
#include "root.h"

long double bisect(long double a, long double b, long double (*f)(long double),
        long double tol)
{
    if(f == NULL) /* null guard */
    {
        return NAN;
    }

    if(tol <= 0.0)
    {
        return NAN;
    }

    if(a == b)
    {
        return NAN;
    }

    long double A = a;
    long double B = b;
    long double root = 0.0;

    do
    {
        root = (A + B) / 2;

        if(f(A) * f(root) < 0) /* root on left */
        {
            B = root;
        }
        else if(f(B) * f(root) < 0) /* root on right */
        {
            A = root;
        }
    }
    while(fabs(f(root)) > tol);

    return root;
}

long double newton(long double a, long double (*f)(long double),
        long double tol)
{
    if(f == NULL)
    {
        return NAN;
    }

    if(tol <= 0.0)
    {
        return NAN;
    }

    long double root = a;

    do
    {
        root -= (f(root)) / (cdiff(root, f, DEFAULT_STEP_SIZE));
    }
    while(fabs(f(root)) > tol);

    return root;
}

