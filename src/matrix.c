#include <stdlib.h>

#include "matrix.h"

Matrix* matrix_init(unsigned int rows, unsigned int cols)
{
    if(rows == 0 || cols == 0) /* bounds check */
    {
        return NULL;
    }

    Matrix* matrix = calloc(1, sizeof(Matrix));

    if(matrix == NULL) /* allocation check */
    {
        return NULL;
    }

    matrix->rows = rows;
    matrix->cols = cols;

    matrix->cells = calloc(rows, sizeof(long double*));

    if(matrix->cells == NULL) /* allocation check */
    {
        return NULL;
    }

    /* allocate columns */
    for(unsigned int i=0;i<rows;i++)
    {
        matrix->cells[i] = calloc(cols, sizeof(long double));

        if(matrix->cells[i] == NULL) /* allocation check */
        {
            free(matrix->cells);
            return NULL;
        }
    }

    return matrix;
}

void matrix_free(Matrix* matrix)
{
    if(matrix == NULL) /* null guard */
    {
        return;
    }

    /* free columns */
    for(unsigned int i=0;i<matrix->rows;i++)
    {
        free(matrix->cells[i]);
    }

    free(matrix->cells); /* free rows */
    matrix->rows = 0;
    matrix->cols = 0;
}

void matrix_sub_row(unsigned int a, unsigned int b, long double k,
        Matrix* matrix)
{
    if(matrix == NULL) /* null guard */
    {
        return;
    }

    if(a == 0 || b == 0|| a - 1 > matrix->rows || b - 1 > matrix->rows ||
            a == b) /* bounds check row indices */
    {
        return;
    }

    long double factor = k == 0.0 ? 1.0 : k;

    /* iteratively subtract from each row */
    for(unsigned int i=0;i<matrix->cols;i++)
    {
        matrix->cells[a][i] -= factor * matrix->cells[b][i];
    }
}

Matrix* matrix_add(Matrix* a, Matrix* b)
{
    if(a == NULL || b == NULL) /* null guard */
    {
        return NULL;
    }

    if(a->rows != b->rows || a->cols != b->cols) /* bounds check */
    {
        return NULL;
    }

    Matrix* res = matrix_init(a->rows, a->cols);

    if(res != NULL) /* check for failure */
    {
        return NULL;
    }

    /* traverse both matrices, adding elementwise */
    for(unsigned int i=0;i<a->rows;i++)
    {
        for(unsigned int j=0;j<a->cols;j++)
        {
            res->cells[i][j] = a->cells[i][j] + b->cells[i][j];
        }
    }

    return res;
}

Matrix* matrix_scale(long double k, Matrix* matrix)
{
    if(matrix == NULL) /* null guard */
    {
        return NULL;
    }

    Matrix* res = matrix_init(matrix->rows, matrix->cols);

    if(res == NULL) /* check for failure */
    {
        return NULL;
    }

    /* traverse matrix, multiplying by scalar k */
    for(unsigned int i=0;i<matrix->rows;i++)
    {
        for(unsigned int j=0;j<matrix->cols;j++)
        {
            res->cells[i][j] = k * matrix->cells[i][j];
        }
    }

    return res;
}

Matrix* matrix_subtract(Matrix* a, Matrix* b)
{
    if(a == NULL || b == NULL) /* null guard */
    {
        return NULL;
    }

    Matrix* negated_b = matrix_scale(-1, b);
    
    if(negated_b == NULL) /* check for failure */
    {
        return NULL;
    }

    Matrix* res = matrix_add(a, negated_b);

    free(negated_b);

    return res;
}

Matrix* matrix_multiply(Matrix* a, Matrix* b)
{
    if(a == NULL || b == NULL) /* null guard */
    {
        return NULL;
    }

    if(a->cols != b->rows) /* bounds check */
    {
        return NULL;
    }

    Matrix* res = matrix_init(a->rows, b->cols);

    if(res == NULL) /* check for failure */
    {
        return NULL;
    }

    long double sum = 0.0; /* sum */

    /* traverse matrices, performing naive matrix multiplication */
    for(unsigned int i=0;i<a->rows;i++)
    {
        for(unsigned int j=0;j<b->cols;j++)
        {
            sum = 0.0;

            for(unsigned int k=0;k<a->cols;k++)
            {
                sum += a->cells[i][k] * b->cells[k][j];
            }

            res->cells[i][j] = sum;
        }
    }

    return res;
}

