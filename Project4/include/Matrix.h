#ifndef __MATRIX_H__
#define __MATRIX_H__

#include <stddef.h>
#include <stdbool.h>

// Matrix structure
typedef struct Matrix{
    size_t row, col;
    float *entry;
} Matrix;


Matrix *createMatrix(const size_t row, const size_t col, const float *const entry);

void deleteMatrix(Matrix *const mat);

void copyMatrix(Matrix *const a, const Matrix *const b);

Matrix *addMatrix(const Matrix *const a, const Matrix *const b);

Matrix *substractMatrix(const Matrix *const a, const Matrix *const b);

Matrix *originalMultiplyMatrix(const Matrix *const a, const Matrix *const b);

Matrix *improvedMultiplyMatrix(const Matrix *const a, const Matrix *const b);

void printMatrix(const Matrix *const mat);

#endif // __MATRIX_H__
