#include <stdlib.h>
#include <math.h>

#include "matrix.h"

Matrix* gauss_elim(Matrix* A, Matrix* b)
{
    if(A == NULL || b == NULL) /* null guard */
    {
        return matrix_init(0, 0); /* null matrix */
    }

    Matrix* x = matrix_init(A->rows, 1);

    for(unsigned int j=0;j<A->rows;j++)
    {
        if(fabs(A->cells[j][j]) == 0.0) /* zero pivot */
        {
            /* TODO: handle zero pivot */
        }

        for(unsigned int i=j+1;i<A->rows;i++)
        {
            long double mult = A->cells[i][j] / A->cells[j][j];
            matrix_sub_row(i, j, mult, A);
            matrix_sub_row(i, j, mult, b); /* update soln vector */
        }
    }

    /* peform back-substitution */
    for(unsigned int i=A->cols-1;i!=0;i--)
    {
        for(unsigned int j=i+1;j<b->cols;j++)
        {
            for(unsigned int k=0;k<b->cols;k++)
            {
                b->cells[i][k] = b->cells[i][k] - A->cells[i][j] *
                    x->cells[j][k];
            }
        }

        for(unsigned int j=0;j<x->cols;j++)
        {
            x->cells[i][j] = b->cells[i][j] / A->cells[i][i];
        }
    }

    return x;
}

