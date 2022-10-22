#ifndef __MATRIX_H__
#define __MATRIX_H__

#include <stddef.h>

/**
 * Global settings
 * If needed, change them to others (e.g. double "%.4lf\t")
*/
typedef float entry_t; // The data type of matrix entries
#define entry_place_holder "%.2f\t" // The precision used for output

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

entry_t min_entry(entry_t a, entry_t b);

entry_t max_entry(entry_t a, entry_t b);

entry_t minEntry(const Matrix *const mat);

entry_t maxEntry(const Matrix *const mat);

entry_t trace(const Matrix *const mat);

entry_t rank(const Matrix *const mat);

void print(const Matrix *const mat);

#endif // __MATRIX_H__
