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

void matrix_sub_row(unsigned int a, unsigned int b, long double k,
        Matrix* matrix);

#endif /* MATRIX_H_ */

