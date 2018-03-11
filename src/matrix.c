/**
 * @file matrix.c
 * @author Jack McPherson
 *
 * Implements methods for manipulating matrices (and, by extension, vectors).
 *
 * */
#include <stdlib.h>
#include <math.h>
#include <time.h>

#include "matrix.h"

/**
 * Initialises a matrix with `rows` rows and `cols` columns (zero-initialised)
 *
 * @param rows
 *      number of rows in the matrix
 * @param cols
 *      number of columns in the matrix
 * @return pointer to initialised matrix or `NULL` pointer on failure
 *
 * */
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

/**
 * Frees memory consumed by `matrix`
 *
 * @param matrix
 *      the matrix to be free'd
 * */
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

/**
 * Performs a (deep) copy of `matrix`
 *
 * @param matrix
 *      the matrix to be copied
 * 
 * @return pointer to copy of matrix or `NULL` on failure
 *
 * */
Matrix* matrix_copy(Matrix* matrix)
{
    if(matrix == NULL) /* null guard */
    {
        return NULL;
    }

    if(matrix->rows == 0 || matrix->cols == 0) /* trivial case */
    {
        return matrix_init(matrix->rows, matrix->cols);
    }

    Matrix* res = matrix_init(matrix->rows, matrix->cols);

    if(res == NULL) /* check for failure */
    {
        return NULL;
    }

    /* traverse matrix, copying elementwise */
    for(unsigned int i=0;i<matrix->rows;i++)
    {
        for(unsigned int j=0;j<matrix->cols;j++)
        {
            res->cells[i][j] = matrix->cells[i][j];
        }
    }

    return res;
}

/**
 * Subtracts `k` times row `b` from row `a` (`k` defaults to 1).
 *
 * @param a
 *      index of row being subtracted from
 * @param b
 *      index of row being subtracted
 * @param k
 *      scalar factor that row at index `b` is scaled by
 * @param matrix
 *      the matrix being operated on
 *
 * */
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

/**
 * Add two matrices, `a` and `b`
 *
 * @param a
 *      first matrix
 * @param b
 *      second matrix
 *
 * @return result of `a` + `b` or `NULL` on failure
 *
 * */
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

/**
 * Multiplies `matrix` by a scalar `k`
 *
 * @param k
 *      scalar to multiply the matrix `matrix` by
 * @param matrix
 *      the matrix being scaled
 *
 * @return result of `k` * `matrix`, or `NULL` on failure
 *
 * */
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

/**
 * Subtracts the matrix `b` from `a`
 *
 * @param a
 *      the matrix being subtracted from
 * @param b
 *      the matrix subtracted from `a`
 *
 * @return the result of `a` - `b`, or `NULL` on failure
 *
 * */
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

/**
 * Multiplies two matrices, `a` and `b`
 *
 * @param a
 *      LHS matrix
 * @param b
 *      RHS matrix
 *
 * @return result of `a` * `b`, or `NULL` on failure
 *
 * */
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

Matrix* matrix_randmat(unsigned int rows, unsigned int cols)
{
    if(rows == 0 || cols == 0) /* trivial case */
    {
        return matrix_init(rows, cols);
    }

    Matrix* matrix = matrix_init(rows, cols);

    srand(time(NULL));

    for(unsigned int i=0;i<rows;i++)
    {
        for(unsigned int j=0;j<cols;j++)
        {
            matrix->cells[i][j] = rand();
        }
    }

    return matrix;
}

/**
 * Returns the augmented matrix obtained by appending the matrix `b` onto the
 *      right of the matrix `a`.
 *
 * @param a
 *      the LHS matrix
 * @param b
 *      the RHS matrix
 *
 * @return the augmented matrix with `b` on the right, or `NULL` on failure
 *
 * */
Matrix* matrix_right_augment(Matrix* a, Matrix* b)
{
    if(a == NULL || b == NULL) /* null guard */
    {
        return NULL;
    }

    if(a->rows != b->rows) /* bounds check */
    {
        return NULL;
    }

    Matrix* c = matrix_init(a->rows, a->cols + b->cols);

    for(unsigned int i=0;i<c->rows;i++)
    {
        for(unsigned int j=0;j<c->cols;j++)
        {
            if(j < a->cols)
            {
                c->cells[i][j] = a->cells[i][j];
            }
            else
            {
                c->cells[i][j] = b->cells[i][j-a->cols];
            }
        }
    }

    return c;
}

/**
 * Returns the augmented matrix obtained by appending the matrix `b` onto the
 *      bottom of the matrix `a`.
 *
 * @param a
 *      the top matrix
 * @param b
 *      the bottom matrix
 *
 * @return the augmented matrix with `b` on the bottom, or `NULL` on failure
 *
 * */
Matrix* matrix_bottom_augment(Matrix* a, Matrix* b)
{
    if(a == NULL || b == NULL) /* null guard */
    {
        return NULL;
    }

    if(a->cols != b->cols) /* bounds check */
    {
        return NULL;
    }

    Matrix* c = matrix_init(a->rows + b->rows, a->cols);

    for(unsigned int i=0;i<c->rows;i++)
    {
        for(unsigned int j=0;j<c->cols;j++)
        {
            if(i < a->rows)
            {
                c->cells[i][j] = a->cells[i][j];
            }
            else
            {
                c->cells[i][j] = b->cells[i-a->rows][j];
            }
        }
    }

    return c;
}

/**
 * Performs Gaussian elimination on the system defined by `Ax=b`
 *
 * @param A
 *      the matrix containing the coefficients of each equation in the system
 * @param b
 *      the vector containing the RHS values of each equation in the system
 *
 * @return the vector `x` containing the solutions of the system `Ax=b`, or
 *      `NULL` on failure
 *
 * */
Matrix* matrix_gauss_elim(Matrix* A, Matrix* b)
{
    if(A == NULL || b == NULL) /* null guard */
    {
        return matrix_init(0, 0); /* null matrix */
    }

    if(A->cols != A->rows) /* bounds check */
    {
        return NULL;
    }

    Matrix* x = matrix_init(A->cols, 1);

    for(unsigned int j=0;j<A->rows;j++)
    {
        if(fabs(A->cells[j][j]) == 0.0) /* zero pivot */
        {
            matrix_free(x);
            return NULL;
        }

        for(unsigned int i=j+1;i<A->rows;i++)
        {
            long double mult = A->cells[i][j] / A->cells[j][j];
            matrix_sub_row(i, j, mult, A);
            matrix_sub_row(i, j, mult, b); /* update soln vector */
        }
    }

    /* perform back-substitution */
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

