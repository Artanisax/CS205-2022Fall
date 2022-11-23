#ifndef __MATRIX_H__
#define __MATRIX_H__

#include <stddef.h>
#include <stdbool.h>

// Global settings
// If needed, change them to more precise floating type (e.g. double "%.4lf\t")
typedef float entry_t; // The data type of matrix entries
#define entry_place_holder "%.2f\t" // The precision and format used for output

// Matrix structure
typedef struct Matrix{
    size_t row, col;
    entry_t *entry;
} Matrix;


Matrix *createMatrix(const size_t row, const size_t col, const entry_t *const entry);

void deleteMatrix(Matrix *const mat);

void copyMatrix(Matrix *const a, const Matrix *const b);

Matrix *addMatrix(const Matrix *const a, const Matrix *const b);

Matrix *substractMatrix(const Matrix *const a, const Matrix *const b);

Matrix *originalMultiplyMatrix(const Matrix *const a, const Matrix *const b);

Matrix *improvedMultiplyMatrix(const Matrix *const a, const Matrix *const b);

void printMatrix(const Matrix *const mat);

#endif // __MATRIX_H__
