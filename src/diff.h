/**
 * @file diff.h
 * @author Jack McPherson
 *
 * Declarations for finite difference methods.
 *
 * */
#ifndef DIFF_H_
#define DIFF_H_

long double fdiff(long double x, long double (*f)(long double), long double h);
long double bdiff(long double x, long double (*f)(long double), long double h);
long double cdiff(long double x, long double (*f)(long double), long double h);

#endif /* DIFF_H_ */

