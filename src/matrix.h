/**
 * @file matrix.h
 * @author Jack McPherson
 *
 * Declarations for matrix methods.
 *
 * */
#ifndef MATRIX_H_
#define MATRIX_H_

typedef struct
{
    unsigned int rows;
    unsigned int cols;
    long double** cells;
} Matrix;

/* initialisation */
Matrix* matrix_init(unsigned int rows, unsigned int cols);
void matrix_free(Matrix* matrix);
Matrix* matrix_copy(Matrix* matrix);

void matrix_add_row(Matrix* matrix);
void matrix_add_col(Matrix* matrix);
void matrix_pop_row(Matrix* matrix);
void matrix_pop_col(Matrix* matrix);

/* elementary row operations */
void matrix_sub_row(unsigned int a, unsigned int b, long double k,
        Matrix* matrix);

/* arithmetic operations */
Matrix* matrix_add(Matrix* a, Matrix* b);
Matrix* matrix_scale(long double k, Matrix* matrix);
Matrix* matrix_subtract(Matrix* a, Matrix* b);
Matrix* matrix_multiply(Matrix* a, Matrix* b);

Matrix* matrix_transpose(Matrix* matrix);

/* utilities */
Matrix* matrix_randmat(unsigned int rows, unsigned int cols);
Matrix* matrix_right_augment(Matrix* a, Matrix* b);
Matrix* matrix_bottom_augment(Matrix* a, Matrix* b);

Matrix* matrix_gauss_elim(Matrix* A, Matrix* b);
Matrix* strassen(Matrix* a, Matrix* b);

#endif /* MATRIX_H_ */

