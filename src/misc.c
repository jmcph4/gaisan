#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "matrix.h"
#include "misc.h"

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

void print_matrix(Matrix* mat)
{
    if(mat == NULL) /* null guard */
    {
        return;
    }

    for(unsigned int i=0;i<mat->rows;i++)
    {
        for(unsigned int j=0;j<mat->cols;j++)
        {
            printf("%*s%Lf ", 1, "", mat->cells[i][j]);
        }

        printf("\n");
    }
}

