#ifndef OPT_H_
#define OPT_H_

long double newton_min(long double a, long double (*f)(long double),
        long double tol);

long double golden_section_min(long double a, long double b,
        long double (*f)(long double), long double tol);

#endif /* OPT_H_ */

