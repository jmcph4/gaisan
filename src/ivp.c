/**
 * @file ivp.c
 * @author Jack McPherson
 *
 * Implements methods for solving Initial Value Problems (IVPs).
 *
 * */
#include <stdlib.h>
#include <math.h>

#include "constants.h"
#include "diff.h"
#include "ivp.h"

/**
 * Solves the IVP `y'(t)=f(t,y), y(a)=y_0` via Euler's method
 *
 * @param a
 *      start of the solution interval
 * @param b
 *      end of the solution interval 
 * @param y_0
 *      initial value of IVP (i.e. value of `f(a)`)
 * @param f
 *      the function to integrate
 * @param h
 *      the step size to use for Euler's method calculations
 * 
 * @return array of x, y pairs constituting the solution of the IVP
 *
 * */
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

