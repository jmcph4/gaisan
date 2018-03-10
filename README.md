# Gaisan  - fast numerical methods in computational science #
---

A C library of numerical methods in computational science, designed to be fast,
friendly, and accurate.

Some methods Gaisan implements or plans to implement:

- Finite difference
    - Forward difference
    - Backward difference
    - Central difference
- Rootfinding
    - Bisection
    - Fixed-point iteration
    - Newton's method
- Optimisation
    - Golden section search
    - Newton's method
- Linear systems
    - Gaussian elimination
- IVPs
    - Euler's method
- BVPs
    - Shooting method
    - Finite difference
- Monte Carlo methods
    - Integration

## Build ##

To build the library, simply

    make

To generate documentation, run

    make docs

To build the examples (in the `examples/` directory), run

    make examples

## Examples ##
Gaisan contains full, working examples in the `examples/` directory; however, here are some snippets:

### Solve an IVP ###
Solving IVPs is easy in Gaisan. Here's a trivial example using Euler's method:

    long double f(long double t, long double y) {return t;}

    /* ... */

    /* solve y'=t from 0 to 10, with y(0)=1 (taking 1/2 steps) */
    long double** solution = euler(0, 10, 1, &f, 0.5);

That's it! Gaisan even provides convenient helper functions for working with numerical data. Let's print a table of values for our solution:

    char* headings[3] = {"t", "y", NULL};
    print_table(headings, solution, 20); /* print_table needs number of steps to print (i.e. 10*0.5=20) */    

This outputs:

    |         t|      y(t)|
    --------------------
    |00.000000|01.000000|
    |00.500000|01.250000|
    |01.000000|01.750000|
    |01.500000|02.500000|
    |02.000000|03.500000|
    |02.500000|04.750000|
    |03.000000|06.250000|
    |03.500000|08.000000|
    |04.000000|10.000000|
    |04.500000|12.250000|
    |05.000000|14.750000|
    |05.500000|17.500000|
    |06.000000|20.500000|
    |06.500000|23.750000|
    |07.000000|27.250000|
    |07.500000|31.000000|
    |08.000000|35.000000|
    |08.500000|39.250000|
    |09.000000|43.750000|
    |09.500000|48.500000|

### Solve a linear system ###
Gaisan also makes solving linear systems a breeze. Once again, we use a naive method: Gaussian elimination.

Firstly, we need to allocate space for our matrices:

    Matrix* A = matrix_init(3, 3);
    Matrix* b = matrix_init(3, 1);

Note that this is equivalent to the following Matlab code:

    A = zeros([3 3]);
    b = zeros([3 1]);

Now we need to populate our matrices with our coefficients and RHS solutions. In practice, this can be done more gracefully, but we'll hard code our values for now:

    /* coefficient matrix */
    A->cells[0][0] = 1.0;
    A->cells[0][1] = 2.0;
    A->cells[0][2] = -1.0;
    A->cells[1][0] = 2.0;
    A->cells[1][1] = 1.0;
    A->cells[1][2] = -2.0;
    A->cells[2][0] = -3.0;
    A->cells[2][1] = 1.0;
    A->cells[2][2] = 1.0;

    /* RHS vector */
    b->cells[0][0] = 3.0;
    b->cells[1][0] = 3.0;
    b->cells[2][0] = -6.0;

Now for the actual solution:

    Matrix* x = gauss_elim(A, b);

That's it!

Just like our last example, Gaisan also provides (an even nicer) helper method for printing our data:

    print_matrix(x);

Which outputs:

     0.000000
     3.000000
    -3.000000

