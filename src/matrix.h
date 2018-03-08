#ifndef MATRIX_H_
#define MATRIX_H_

typedef struct
{
    unsigned int rows;
    unsigned int cols;
    long double** cells;
} Matrix;

Matrix* matrix_init(unsigned int rows, unsigned int cols);
void matrix_free(Matrix* matrix);
Matrix* matrix_copy(Matrix* matrix);

void matrix_sub_row(unsigned int a, unsigned int b, long double k,
        Matrix* matrix);

Matrix* matrix_add(Matrix* a, Matrix* b);
Matrix* matrix_scale(long double k, Matrix* matrix);
Matrix* matrix_subtract(Matrix* a, Matrix* b);
Matrix* matrix_multiply(Matrix* a, Matrix* b);

Matrix* matrix_gauss_elim(Matrix* A, Matrix* b);

#endif /* MATRIX_H_ */

