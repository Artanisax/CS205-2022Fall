#ifndef __MATRIX_H__
#define __MATRIX_H__

#include <stddef.h>
#include <stdlib.h>

typedef struct {
    size_t col, row;
    float *entry;
    float rank, addend;
}Matrix;

// Create an empty matrix
Matrix createMatrix(const size_t col, const size_t row);

// Delete a matrix
void deleteMatrix();

// copy a matrix
void copyMatrix();

// Add two matrices
void addMatrix();

// Substraction of to matrices
void substractMatrix();

// Multiply two matrices
void multiplyMatrix();

// Add a scalar to a matrix
void addScalar();

// Subtract a scalar from a matrix
void substractScalar();

// Multiply a matrix with a scalar
void multiplyScalar();

// Find the minimal value of a matrix
double minEntry();

// Find the maximal value of a matrix
double maxEntry();

#endif // __MATRIX_H__