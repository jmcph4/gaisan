/**
 * @file misc.h
 * @author Jack McPherson
 *
 * Declarations for miscellaneous functions.
 *
 * */
#ifndef MISC_H_
#define MISC_H_

#include <stdio.h>
#include <stdbool.h>

#include "matrix.h"

#define MAX_FLOAT_WIDTH 40

#define MAX(a,b) (((a) > (b)) ? (a) : (b))

void print_table(char** labels, long double** data, unsigned int num_rows);

void write_matrix(FILE* file, Matrix* mat);
Matrix* read_matrix(FILE* file);

void print_matrix(Matrix* mat);

#endif /* MISC_H_ */

