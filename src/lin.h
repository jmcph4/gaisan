#ifndef LIN_H_
#define LIN_H_

#include <stdbool.h>

#include "matrix.h"

typedef struct
{
    Matrix* A;
    Matrix* b;
    Matrix* x;
} LinSys;

LinSys* linsys_init(Matrix* A, Matrix* b);
void linsys_free(LinSys* linsys);

void linsys_solve(LinSys* linsys);

bool linsys_underdetermined(LinSys* linsys);
bool linsys_overdetermined(LinSys* linsys);

#endif /* LIN_H_ */

