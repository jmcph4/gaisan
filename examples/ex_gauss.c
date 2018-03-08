#include <stdio.h>
#include <stdlib.h>

#include "matrix.h"
#include "lin.h"
#include "misc.h"

int main(void)
{
    /* initialise matrices */
    Matrix* A = matrix_init(3, 3);
    Matrix* b = matrix_init(3, 1);
    
    A->cells[0][0] = 1.0;
    A->cells[0][1] = 2.0;
    A->cells[0][2] = -1.0;
    A->cells[1][0] = 2.0;
    A->cells[1][1] = 1.0;
    A->cells[1][2] = -2.0;
    A->cells[2][0] = -3.0;
    A->cells[2][1] = 1.0;
    A->cells[2][2] = 1.0;

    b->cells[0][0] = 3.0;
    b->cells[1][0] = 3.0;
    b->cells[2][0] = -6.0;

    Matrix* x = matrix_gauss_elim(A, b);

    print_matrix(x);

    /* tidy up */
    matrix_free(x);
    matrix_free(b);
    matrix_free(A);

    return EXIT_SUCCESS;
}

