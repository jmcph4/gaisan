/**
 * @file misc.c
 * @author Jack McPherson
 *
 * Implements miscellaneous functionality required by the library (e.g.
 * printing tables of data, printing matrices, etc.).
 *
 * */
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>

#include "matrix.h"
#include "constants.h"
#include "misc.h"

/**
 * Returns the length of the longest string in `strings`
 *
 * @param strings
 *      an `NULL`-terminated array of strings to search
 *
 * @return the length (in characters, ignoring the `NULL` byte) of the longest
 *      string in `strings`, or 0 on failure
 *
 * */
size_t widest_string(char** strings)
{
    if(strings == NULL) /* null guard */
    {
        return 0;
    }

    unsigned int s = 0;
    size_t max_width = 0;

    while(strings[s] != NULL)
    {
        if(strlen(strings[s]) > max_width)
        {
            max_width = strlen(strings[s]);
        }

        s++;
    }

    return max_width;
}

/**
 * Returns the length of the longest string representation of a number in
 *      `doubles`
 *
 * @param doubles
 *      array of values to search
 * @param num_elems
 *      length of `doubles`
 *
 * @return the length (in characters, ignoring the `NULL` byte) of the longest
 *      string representation of all numbers in `doubles`, or 0 on failure
 *
 * */
size_t widest_long_double(long double* doubles, unsigned int num_elems)
{
    if(doubles == NULL) /* null guard */
    {
        return 0;
    }

    size_t max_width = 0; /* max width of all long doubles in list */

    /* allocate list of strings */
    char** strings = calloc(num_elems + 1, sizeof(char*));

    if(strings == NULL) /* allocation check */
    {
        return 0;
    }

    for(unsigned int i=0;i<num_elems;i++)
    {
        strings[i] = calloc(MAX_FLOAT_WIDTH + 1, sizeof(char));

        if(strings[i] == NULL) /* allocation check */
        {
            return 0;
        }
    }

    strings[num_elems] = NULL; /* NULL-terminate */

    /* convert each long double to a string */
    for(unsigned int i=0;i<num_elems;i++)
    {
        snprintf(strings[i], MAX_FLOAT_WIDTH + 1, "%LF", doubles[i]);
    }

    max_width = widest_string(strings);

    /* tidy up */
    for(unsigned int i=0;i<num_elems;i++)
    {
        free(strings[i]);
    }

    free(strings);

    return max_width;
}

/**
 * Prints a table of values to `stdout`
 *
 * @param labels
 *      `NULL`-terminated array of strings constituting the header row of the
 *          table
 * @param data
 *      2-D array of values for the table
 * @param num_rows
 *      the number of rows in the table
 *
 * */
void print_table(char** labels, long double** data, unsigned int num_rows)
{
    if(labels == NULL || data == NULL) /* null guard */
    {
        return;
    }

    /* get number of columns */
    unsigned int num_cols = 0;

    while(labels[num_cols] != NULL)
    {
        num_cols++;
    }

    /* get width of each column */
    size_t max_label_width = widest_string(labels);
    size_t max_data_width = 0;

    for(unsigned int i=0;i<num_cols;i++)
    {
        if(widest_long_double(data[i], num_rows) > max_data_width)
        {
            max_data_width = widest_long_double(data[i], num_rows);
        }
    }

    int padding = MAX(max_label_width, max_data_width);

    /* print header row */
    for(unsigned int i=0;i<num_cols;i++)
    {
        if(i == 0)
        {
            printf("|");
        }

        printf("%*s|", padding + 1, labels[i]);
    }

    printf("\n");
    
    /* print border delineating header row and data */
    unsigned int total_width = num_cols * (padding + 1);

    for(unsigned int i=0;i<total_width;i++)
    {
        putchar('-');
    }

    putchar('\n');

    /* print data */
    for(unsigned int i=0;i<num_rows;i++)
    {
        /* print datum for this row, left-to-right */
        for(unsigned int j=0;j<num_cols;j++)
        {
            if(j == 0)
            {
                printf("|");
            }

            printf("%0*LF|", padding, data[j][i]);
        }

        printf("\n");
    }
}

/**
 * Writes the matrix `mat` to `file`
 *
 * @param file
 *      the file to write the matrix to
 * @param mat
 *      the matrix being printed
 *
 * */
void write_matrix(FILE* file, Matrix* mat)
{
    if(file == NULL || mat == NULL) /* null guard */
    {
        return;
    }

    for(unsigned int i=0;i<mat->rows;i++)
    {
        for(unsigned int j=0;j<mat->cols;j++)
        {
            fprintf(file, "%*s%Lf ", 1, "", mat->cells[i][j]);
        }

        fprintf(file, "\n");
    }
}

/**
 * Prints the matrix `mat` to `stdout`
 *
 * @param mat
 *      the matrix to print
 *
 * */
void print_matrix(Matrix* mat)
{
    if(mat == NULL) /* null guard */
    {
        return;
    }

    write_matrix(stdout, mat);
}

/**
 * Reads a matrix from the file `file`
 *
 * @param file
 *      the file to be read from
 *
 * @return the matrix contained in `file`, or `NULL` on failure
 *
 * */
Matrix* read_matrix(FILE* file)
{
    if(file == NULL) /* null guard */
    {
        return NULL;
    }

    /* buffer variables */
    size_t cap = 0; /* current capacity of buffer */
    size_t len = 0; /* current length of buffer */
    char* buf = calloc(INIT_BUF_LEN, sizeof(char)); /* buffer */

    /* data array variables */
    unsigned int r = 0; /* current row */
    unsigned int c = 0; /* current column */
    unsigned int rows = 1; /* number of rows */
    unsigned int cols = 1; /* number of columns */
    long double** data = calloc(1, sizeof(long double*));
    data[0] = calloc(1, sizeof(long double));

    char ch = '\0'; /* current character */
    bool newline_prev = false;
    bool first_row = true;

    /* main parsing loop */
    while(true)
    {
        ch = fgetc(file); /* fetch new character */

        if(ch == EOF) /* check for EOF */
        {
            break;
        }

        if(isdigit(ch) || ch == '.' || ch == '-') /* value in-process */
        {
            if(len + 1 == cap) /* buffer full, expand */
            {
                cap *= BUF_EXPAND_FACTOR;
                buf = realloc(buf, cap * sizeof(char));
            }
            
            buf[len++] = ch; /* append to buffer */
            newline_prev = false;
        }
        else if(isblank(ch) || ch == '\n') /* captured new value */
        {
            
            buf[len] = '\0'; /* NULL-terminate buffer */
            data[r][c++] = atof(buf); /* save value */ 

            if(isblank(ch)) /* add column */
            {
                if(first_row)
                {
                    data[r] = realloc(data[r], ++cols * sizeof(long double));
                }
                
                newline_prev = false;
            }
            else if(ch == '\n') /* add row */
            {
                if(newline_prev) /* encountered blank line */
                {
                    free(buf);
                    break;
                }
                
                data = realloc(data, ++rows * sizeof(long double*));
                data[++r] = calloc(cols, sizeof(long double));
                
                c = 0; /* reset column index */
                newline_prev = true; /* set newline flag */
                first_row = false;
            }
            
            /* reset buffer */
            free(buf);
            len = 0;
            cap = INIT_BUF_LEN;
            buf = calloc(cap, sizeof(char));
        }
        else /* invalid character */
        {
            /* free data array */
            for(unsigned int i=0;i<rows;i++)
            {
                free(data[i]);
            }

            free(data);
            free(buf);
            
            return NULL;
        }
    }

    /* adjust */
    rows--;

    Matrix* matrix = matrix_init(rows, cols);

    /* copy data into matrix and free as we go */    
    for(unsigned int i=0;i<rows;i++)
    {
        for(unsigned int j=0;j<cols;j++)
        {
            matrix->cells[i][j] = data[i][j];
        }

        free(data[i]);
    }
   
    free(data[rows]);
    free(data);
    
    return matrix;
}

