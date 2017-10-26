#ifndef IVP_H_
#define IVP_H_

long double** euler(long double a, long double b, long double y_0,
        long double (*f)(long double, long double), long double h);

#endif /* IVP_H_ */

