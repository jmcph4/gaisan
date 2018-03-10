/**
 * @file root.h
 * @author Jack McPherson
 *
 * Declarations for rootfinding methods.
 *
 * */
#ifndef ROOT_H_
#define ROOT_H_

long double bisect(long double a, long double b, long double (*f)(long double),
        long double tol);
long double fixed_point(long double a, long double (*f)(long double),
        long double tol);
long double newton(long double a, long double (*f)(long double),
        long double tol);

#endif /* ROOT_H_ */

