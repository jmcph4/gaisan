/**
 * @file constants.h
 * @author Jack McPherson
 *
 * Definitions of various mathematical and technical constants.
 *
 * */
#ifndef CONSTANTS_H_
#define CONSTANTS_H_

/**
 * suitable default step size for most finite difference methods
 * 
 * */
#define DEFAULT_STEP_SIZE 0.000000001

/**
 * suitable default tolerance for most numerical methods
 *
 * */
#define DEFAULT_TOLERANCE 0.000000001

/**
 * the golden ratio, phi
 * 
 * */
#define GOLDEN_RATIO 1.6180339887498948482

/**
 * initial buffer capacity
 *
 * */
#define INIT_BUF_LEN 32

/**
 * default buffer expansion factor
 *
 * */
#define BUF_EXPAND_FACTOR 2

/**
 * default minimum size for Strassen base case
 *
 * */
#define STRASSEN_MIN_SIZE 3

/**
 * default number of submatrices for Strassen's algorithm
 *
 * */
#define STRASSEN_NUM_SUBMATRICES 4

#endif /* CONSTANTS_H_ */

