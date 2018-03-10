/**
 * @file monte.c
 * @author Jack McPherson
 *
 * Implements various Monte Carlo methods (and supporting code).
 *
 * */
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <math.h>

#include "monte.h"

/**
 * Generates a pseudorandom vector of length `dim`, bounded elementwise by
 *      `bounds`
 *
 * @param dim
 *      the dimension of the vector
 * @param bounds
 *      a 2-D array of bounds on each element of the resultant vector
 *
 * @return a pseudorandom vector of length `dim`, or `NULL` on failure
 *
 * */
long double* random_vector(unsigned int dim, long double** bounds)
{
    if(bounds == NULL) /* null guard */
    {
        return NULL;
    }

    if(dim == 0) /* bounds check */
    {
        return NULL;
    }

    long double* vec = calloc(dim, sizeof(long double));

    if(vec == NULL) /* allocation check */
    {
        return NULL;
    }

    /* integer bounds */
    unsigned int a = 0;
    unsigned int b = 0;

    for(unsigned int i=0;i<dim;i++) /* construct vector elementwise */
    {
        a = floorl(bounds[i][0]);
        b = floorl(bounds[i][1]);

        vec[i] = rand() % (b + 1 - a) + a;
    }

    return vec;
}

/**
 * Returns the area of the `n`-gon of size `dim`
 *
 * @param dim
 *      the dimension of the object
 * @param dom
 *      the domain of the object
 *
 * @return the "area" of the `n`-gon
 *
 * */
long double nbox_area(unsigned int dim, long double** dom)
{
    if(dom == NULL) /* null guard */
    {
        return NAN;
    }

    if(dim == 0) /* bounds check */
    {
        return NAN;
    }

    /* calculate area of n-dimensional "box" */
    long double area = 1.0;

    for(unsigned int i=0;i<dim;i++) /* accumulate each dimension */
    {
       area *= labs(dom[i][1] - dom[i][0]);
    }

    return area;
}

/**
 * Returns the "area" of the `dim`-dimensional object defined by the membership
 *      function `memb` using the Monte Carlo object
 *
 * @param memb
 *      a Boolean function defining whether a given point is inside the object
 * @param dim
 *      the dimension of the object
 * @param dom
 *      a 2-D array defining the domain of the arena
 * @param n
 *      the number of points to use in the Monte Carlo
 *
 * @return the "area" of the `dim`-dimensional object
 *
 * */
long double monte_carlo(bool (*memb)(long double*, unsigned int),
        unsigned int dim, long double** dom, unsigned int n)
{
    if(memb == NULL || dom == NULL) /* null guard */
    {
        return NAN;
    }

    if(n == 0 || dim == 0) /* bounds check */
    {
        return NAN;
    }

    srand(time(NULL)); /* seed PRNG */

    unsigned int hits = 0;
    long double* vec = NULL;

    for(unsigned int i=0;i<n;i++) /* generate points */
    {
        vec = random_vector(dim, dom); /* construct random point */

        if(vec == NULL) /* check for failure */
        {
            return NAN;
        }

        if(memb(vec, dim)) /* check for membership */
        {
            hits++; /* increment hit counter */
        }

        free(vec);
    }

    /* calculate answer */
    long double total_area = nbox_area(dim, dom);
    long double prop = (long double)hits / (long double)n;
    long double area = total_area * prop;

    return area;
}

