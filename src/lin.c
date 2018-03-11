#include <stdlib.h>
#include <stdbool.h>

#include "lin.h"

LinSys* linsys_init(Matrix* A, Matrix* b)
{
    if(A == NULL || b == NULL)
    {
        return NULL;
    }

    if(A->rows != b->rows || b->cols != 1) /* bounds check */
    {
        return NULL;
    }

    LinSys* sys = calloc(1, sizeof(LinSys));

    if(sys == NULL) /* allocation check */
    {
        return NULL;
    }
    
    sys->A = matrix_copy(A);

    if(sys->A == NULL) /* check for failure */
    {
        return NULL;
    }

    sys->b = matrix_copy(b);

    if(sys->b == NULL) /* check for failure */
    {
        free(sys->A);
        return NULL;
    }

    /* assign fields */
    sys->A = A;
    sys->b = b;
    sys->x = NULL;

    return sys;
}

void linsys_free(LinSys* linsys)
{
    if(linsys == NULL) /* null guard */
    {
        return;
    }

    free(linsys->A);
    free(linsys->b);
    free(linsys->x);
    free(linsys);
}

void linsys_solve(LinSys* linsys)
{
    if(linsys == NULL)
    {
        return;
    }

    linsys->x = matrix_gauss_elim(linsys->A, linsys->b);
}

bool linsys_underdetermined(LinSys* linsys)
{
    if(linsys == NULL) /* null guard */
    {
        return false;
    }

    if(linsys->A->rows < linsys->A->cols)
    {
        return true;
    }
    
    return false;
}

bool linsys_overdetermined(LinSys* linsys)
{
    if(linsys == NULL) /* null guard */
    {
        return false;
    }

    if(linsys->A->rows > linsys->A->cols)
    {
        return true;
    }
    
    return false;
}

