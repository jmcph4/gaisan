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
#include <string.h>
#include <limits.h>

#include "constants.h"
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

    free(matrix);
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

/********************* Elementary Row Operations (EROs) **********************/

/**
 * Swaps rows `a` and `b`
 *
 * @param a
 *      index of first row
 * @param b
 *      index of second row
 * @param matrix
 *      the matrix being operated on
 *
 * */
void matrix_swap_rows(unsigned int a, unsigned int b, Matrix* matrix)
{
    if(matrix == NULL)
    {
        return;
    }

    /* bounds check */
    if(a > matrix->rows || b > matrix->rows || a == b)
    {
        return;
    }

    long double* tmp = calloc(matrix->cols, sizeof(long double));

    if(tmp == NULL)
    {
        return;
    }

    memcpy(tmp, matrix->cells[a], matrix->rows * sizeof(long double));
    memcpy(matrix->cells[b], matrix->cells[a],
            matrix->rows * sizeof(long double));
    memcpy(matrix->cells[b], tmp, matrix->rows * sizeof(long double));

    free(tmp);
}

/**
 * Scales row `a` by factor `k` (`k` defaults to 1)
 *
 * @param a
 *      index of row being scaled
 * @param k
 *      factor to scale row by
 * @param matrix
 *      the matrix being operated on
 *
 * */
void matrix_scale_row(unsigned int a, long double k, Matrix* matrix)
{
    if(matrix == NULL) /* null guard */
    {
        return;
    }

    if(a > matrix->rows) /* bounds check */
    {
        return;
    }

    long double factor = k == 0.0 ? 1.0 : k;

    /* iteratively scale each row */
    for(unsigned int i=0;i<matrix->cols;i++)
    {
        matrix->cells[a][i] *= factor;
    }
}

/**
 * Adds `k` times row `b` to row `a` (`k` defaults to 1)
 *
 * @param a
 *      index of row being added to
 * @param b
 *      index of row being added
 * @param k
 *      scalar factor that row at index `b` is scaled by
 * @param matrix
 *      the matrix being operated on
 *
 * */
void matrix_add_row(unsigned int a, unsigned int b, long double k,
        Matrix* matrix)
{
    if(matrix == NULL) /* null guard */
    {
        return;
    }

    /* bounds check */
    if(a > matrix->rows || b > matrix->rows || a == b)
    {
        return;
    }

    long double factor = k == 0.0 ? 1.0 : k;

    /* iteratively add to each row */
    for(unsigned int i=0;i<matrix->cols;i++)
    {
        matrix->cells[a][i] += factor * matrix->cells[b][i];
    }
}

void matrix_append_row(Matrix* matrix)
{
    if(matrix == NULL) /* null guard */
    {
        return;
    }

    matrix->cells = realloc(matrix->cells,
            (matrix->rows + 1) * sizeof(long double*));
    matrix->cells[matrix->rows] = calloc(matrix->cols, sizeof(long double));
    matrix->rows++;
}

void matrix_append_col(Matrix* matrix)
{
    if(matrix == NULL) /* null guard */
    {
        return;
    }

    for(unsigned int i=0;i<matrix->rows;i++)
    {
        matrix->cells[i] = realloc(matrix->cells[i],
                (matrix->cols + 1) * sizeof(long double));
        matrix->cells[i][matrix->cols] = 0;
    }

    matrix->cols++;
}

void matrix_drop_row(Matrix* matrix)
{
    if(matrix == NULL) /* null guard */
    {
        return;
    }

    matrix->cells = realloc(matrix->cells,
            (matrix->rows - 1) * sizeof(long double*));
    matrix->rows--;
}

void matrix_drop_col(Matrix* matrix)
{
    if(matrix == NULL) /* null guard */
    {
        return;
    }

    for(unsigned int i=0;i<matrix->rows;i++)
    {
        matrix->cells[i] = realloc(matrix->cells[i],
                (matrix->cols - 1) * sizeof(long double));
    }

    matrix->cols--;
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

    if(res == NULL) /* check for failure */
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

/**
 * Transposes the matrix `matrix`
 *
 * @param matrix
 *      the matrix to be transposed
 *
 * @return the transpose of the matrix, or `NULL` on failure
 *
 * */
Matrix* matrix_transpose(Matrix* matrix)
{
    if(matrix == NULL) /* null guard */
    {
        return NULL;
    }

    Matrix* transpose = matrix_init(matrix->cols, matrix->rows);

    if(transpose == NULL)
    {
        return NULL;
    }

    for(unsigned int i=0;i<matrix->rows;i++)
    {
        for(unsigned int j=0;j<matrix->cols;j++)
        {
            transpose->cells[j][i] = matrix->cells[i][j];
        }
    }

    return transpose;
}

/**
 * Returns a `rows` by `cols` random matrix
 *
 * @param rows
 *      the number of rows
 * @params cols
 *      the number of columns
 *
 * @return a random matrix of specified dimensions
 *
 * */
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
        return NULL;
    }

    if(A->cols != A->rows) /* bounds check */
    {
        return NULL;
    }

    Matrix* x = matrix_init(A->cols, b->cols);

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
            matrix_add_row(i, j, -1 * mult, A);
            matrix_add_row(i, j, -1 * mult, b); /* update soln vector */
        }
    }

    /* perform back-substitution */
    for(unsigned int i=A->cols-1;i!=UINT_MAX;i--)
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

void pad_to_power_2(Matrix* matrix)
{
    if(matrix == NULL) /* null guard */
    {
        return;
    }

    unsigned int n = matrix->rows;
    n--;
    n |= n >> 1;
    n |= n >> 2;
    n |= n >> 4;
    n |= n >> 8;
    n |= n >> 16;
    n++;

    for(unsigned int i=matrix->rows;i<n;i++)
    {
        matrix_append_row(matrix);
        matrix_append_col(matrix);
    }
}

void trim(unsigned int n, Matrix* matrix)
{
    if(matrix == NULL) /* null guard */
    {
        return;
    }

    if(n >= matrix->rows) /* trivial case */
    {
        return;
    }

    unsigned int start = matrix->rows;

    for(unsigned int i=0;i<start-n;i++)
    {
        matrix_drop_row(matrix);
        matrix_drop_col(matrix);
    }
}

/**
 * Multiplies the two matrices `a` and `b` using Strassen's algorithm
 *
 * Note that both matrices must be square and that, for matrices of dimension
 *      less than or equal to STRASSEN_MIN_SIZE, this method is equivalent
 *      to naive matrix multiplication.
 *
 * @param a
 *      the LHS matrix
 * @param b
 *      the RHS matrix
 *
 * @return the result of `a` * `b`, or `NULL` on failure
 *
 * */
Matrix* strassen(Matrix* a, Matrix* b)
{
    if(a == NULL || b == NULL) /* null guard */
    {
        return NULL;
    }

    /* bounds check */
    if(a->rows != b-> rows || a->cols != b->cols || a->rows != a->cols)
    {
        return NULL;
    }

    if(a->rows <= STRASSEN_MIN_SIZE) /* base case */
    {
        return matrix_multiply(a, b);
    }

    unsigned int original_size = a->rows;

    /* padding */
    if(!((a->rows != 0) && ((a->rows & (a->rows - 1)) == 0)))
    {
        pad_to_power_2(a);
        pad_to_power_2(b);
    }

    unsigned int n = a->rows / 2;

    /* submatrices */
    Matrix* A[4];
    Matrix* B[4];
    Matrix* C[4];
    Matrix* P[7];

    /* initialise submatrices */
    for(unsigned int i=0;i<STRASSEN_NUM_SUBMATRICES;i++)
    {
        A[i] = matrix_init(n, n);
        B[i] = matrix_init(n, n);
        C[i] = matrix_init(n, n);
    }

    /* extract submatrices */
    for(unsigned int i=0;i<n;i++)
    {
        for(unsigned int j=0;j<n;j++)
        {
            /* top-left */
            A[0]->cells[i][j] = a->cells[i][j];
            B[0]->cells[i][j] = b->cells[i][j];

            /* top-right */
            A[1]->cells[i][j] = a->cells[i][n+j];
            B[1]->cells[i][j] = b->cells[i][n+j];

            /* bottom-left */
            A[2]->cells[i][j] = a->cells[n+i][j];
            B[2]->cells[i][j] = b->cells[n+i][j];

            /* bottom-right */
            A[3]->cells[i][j] = a->cells[n+i][n+j];
            B[3]->cells[i][j] = b->cells[n+i][n+j];
        }
    }
    
    /* calculate component matrices */
    P[0] = strassen(A[0], matrix_subtract(B[1], B[3]));
    P[1] = strassen(matrix_add(A[0], A[1]), B[3]);
    P[2] = strassen(matrix_add(A[2], A[3]), B[0]);
    P[3] = strassen(A[3], matrix_subtract(B[2], B[0]));
    P[4] = strassen(matrix_add(A[0], A[3]), matrix_add(B[0], B[3]));
    P[5] = strassen(matrix_subtract(A[1], A[3]), matrix_add(B[2], B[3]));
    P[6] = strassen(matrix_subtract(A[0], A[2]), matrix_add(B[0], B[1]));

    /* calculate submatrices of result matrix */
    C[0] = matrix_add(matrix_subtract(matrix_add(P[4], P[3]), P[1]), P[5]);
    C[1] = matrix_add(P[0], P[1]);
    C[2] = matrix_add(P[2], P[3]);
    C[3] = matrix_subtract(matrix_add(P[0], P[4]), matrix_add(P[2], P[6]));
    
    /* join */
    Matrix* top_row = matrix_right_augment(C[0], C[1]);
    Matrix* bottom_row = matrix_right_augment(C[2], C[3]);
    Matrix* c = matrix_bottom_augment(top_row, bottom_row);

    /* trim padding (if any) */
    if(c->rows != original_size)
    {
        trim(original_size, c);
    }

    /* free matrices */
    matrix_free(top_row);
    matrix_free(bottom_row);

    for(unsigned int i=0;i<STRASSEN_NUM_SUBMATRICES;i++)
    {
        matrix_free(A[i]);
        matrix_free(B[i]);
        matrix_free(C[i]);
    }
    
    return c;
}

