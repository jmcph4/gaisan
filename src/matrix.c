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

