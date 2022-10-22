#ifndef __MATRIX_H__
#define __MATRIX_H__

#include <stddef.h>

// Global settings
typedef float entry_t;
#define entry_place_holder "%.2f\t"
// If needed, change these to other types (e.g. double "%.4lf")

typedef struct {
    size_t row, col;
    entry_t *entry;
} Matrix;

// Create an empty matrix
Matrix *createMatrix(const size_t row, const size_t col, const entry_t *const entry);

// Delete a matrix
void deleteMatrix(Matrix *const mat);

// copy data from b to a
void copyMatrix(Matrix *const a, const Matrix *const b);

// Add two matrices
Matrix *addMatrix(const Matrix *const a, const Matrix *const b);

// Substraction of to matrices
Matrix *substractMatrix(const Matrix *const a, const Matrix *const b);

// Multiply two matrices
Matrix *multiplyMatrix(const Matrix *const a, const Matrix *const b);

// Add a scalar to a matrix
void addScalar(const Matrix *mat, const entry_t x);

// Subtract a scalar from a matrix
void substractScalar(const Matrix *mat, const entry_t x);

// Multiply a matrix with a scalar
void multiplyScalar(const Matrix *mat, const entry_t x);

// Divide a matrix with a scalar
void divideScalar(const Matrix *mat, const entry_t x);

// Compare two entry elements
entry_t min(entry_t a, entry_t b);

entry_t max(entry_t a, entry_t b);

// Find the minimal value of a matrix
entry_t minEntry(const Matrix *const mat);

// Find the maximal value of a matrix
entry_t maxEntry(const Matrix *const mat);

void print(const Matrix *const mat);

#endif // __MATRIX_H__