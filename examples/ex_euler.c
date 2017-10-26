#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "ivp.h"
#include "misc.h"

long double f(long double t, long double y)
{
    return t;
}

int main(int argc, char** argv)
{
    if(argc != 5) /* usage */
    {
        fprintf(stderr, "usage: ex_euler a b y_0 h\n");
        return EXIT_FAILURE;
    }

    /* parameters to Euler's method */
    long double a = atof(argv[1]);
    long double b = atof(argv[2]);
    long double y_0 = atof(argv[3]);
    long double h = atof(argv[4]);

    long double** solution = euler(a, b, y_0, &f, h);

    if(solution == NULL)
    {
        perror("euler");
        return EXIT_FAILURE;
    }

    /* print results */
    unsigned int num_steps = ceil((a + b) / h);
    char* labels[3] = {"t", "y(t)", NULL};
    print_table(labels, solution, num_steps);

    /* tidy up */
    free(solution[0]);
    free(solution[1]);
    free(solution);

    return EXIT_SUCCESS;
}

