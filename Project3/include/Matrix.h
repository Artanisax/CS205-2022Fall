#ifndef __MATRIX_H__
#define __MATRIX_H__

#include <stddef.h>

// Global settings
typedef float entry_t;
#define entry_place_holder "%.2f\t"
// If needed, change these to other types (e.g. double "%.4lf")

// Matrix structure
typedef struct {
    size_t row, col;
    entry_t *entry;
} Matrix;

Matrix *createMatrix(const size_t row, const size_t col, const entry_t *const entry);

void deleteMatrix(Matrix *const mat);

void copyMatrix(Matrix *const a, const Matrix *const b);

Matrix *addMatrix(const Matrix *const a, const Matrix *const b);

Matrix *substractMatrix(const Matrix *const a, const Matrix *const b);

Matrix *multiplyMatrix(const Matrix *const a, const Matrix *const b);

void addScalar(const Matrix *mat, const entry_t x);

void substractScalar(const Matrix *mat, const entry_t x);

void multiplyScalar(const Matrix *mat, const entry_t x);

void divideScalar(const Matrix *mat, const entry_t x);

entry_t min(entry_t a, entry_t b);

entry_t max(entry_t a, entry_t b);

entry_t minEntry(const Matrix *const mat);

// Find the maximal value of a matrix
entry_t maxEntry(const Matrix *const mat);

void print(const Matrix *const mat);

#endif // __MATRIX_H__
