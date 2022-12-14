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

// List node of valid matrix pointers
typedef struct MatrixPointer{
    Matrix *mat;
    struct MatrixPointer *next;
} MatrixPointer;

void MPL_push_front(Matrix *const mat);

void MPL_erase(const Matrix *const mat);

void MPL_clear();

bool check_matrix_pointer(const Matrix *const mat);

Matrix *createMatrix(const size_t row, const size_t col, const entry_t *const entry);

Matrix *createIMatrix(const size_t n);

void deleteMatrix(Matrix *const mat);

void copyMatrix(Matrix *const a, const Matrix *const b);

Matrix *addMatrix(const Matrix *const a, const Matrix *const b);

Matrix *substractMatrix(const Matrix *const a, const Matrix *const b);

Matrix *multiplyMatrix(const Matrix *const a, const Matrix *const b);

void addScalar(const Matrix *mat, const entry_t x);

void substractScalar(const Matrix *mat, const entry_t x);

void multiplyScalar(const Matrix *mat, const entry_t x);

void divideScalar(const Matrix *mat, const entry_t x);

entry_t minEntry(const Matrix *const mat);

entry_t maxEntry(const Matrix *const mat);

void GaussianEliminate(Matrix *const mat);

entry_t trace(const Matrix *const mat);

entry_t det(const Matrix *const mat);

size_t rank(const Matrix *const mat);

void printMatrix(const Matrix *const mat);

#endif // __MATRIX_H__
