/**
 * @file diff.c
 * @author Jack McPherson
 *
 * Implements finite difference methods for approximating the derivatives of
 * real-valued functions at given points.
 *
 * */
#include <stdlib.h>
#include <math.h>

#include "diff.h"

/**
 * Calculates the forward finite difference of the function `f` at `x` using
 * step size `h`.
 *
 * @param x
 *      value at which to calculate the forward difference of `f`
 * @param f
 *      function to evaluate
 * @param h
 *      step size to use in finite difference calculation
 *
 * @return forward difference of `f` at `x`
 *
 * */
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

/**
 * Calculates the backward finite difference of the function `f` at `x` using
 * step size `h`.
 *
 * @param x
 *      value at which to calculate the backward difference of `f`
 * @param f
 *      function to evaluate
 * @param h
 *      step size to use in finite difference calculation
 *
 * @return backward difference of `f` at `x`
 *
 * */
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

/**
 * Calculates the central finite difference of the function `f` at `x` using
 * step size `h`.
 *
 * @param x
 *      value at which to calculate the central difference of `f`
 * @param f
 *      function to evaluate
 * @param h
 *      step size to use in finite difference calculation
 *
 * @return central difference of `f` at `x`
 *
 * */
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

