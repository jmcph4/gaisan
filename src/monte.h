/**
 * @file monte.h
 * @author Jack McPherson
 *
 * Declarations of Monte Carlo methods.
 *
 * */
#ifndef MONTE_H_
#define MONTE_H_

#include <stdbool.h>

long double monte_carlo(bool (*memb)(long double*, unsigned int),
        unsigned int dim, long double** dom, unsigned int n);


#endif /* MONTE_H_ */

