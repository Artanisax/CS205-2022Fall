#ifndef __MATRIX_H__
#define __MATRIX_H__

#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>

typedef float entry_t;

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
void addMatrix(const Matrix *const a, const Matrix *const b);

// Substraction of to matrices
void substractMatrix(const Matrix *const a, const Matrix *const b);

// Multiply two matrices
void multiplyMatrix(const Matrix *const a, const Matrix *const b);

// Add a scalar to a matrix
void addScalar();

// Subtract a scalar from a matrix
void substractScalar();

// Multiply a matrix with a scalar
void multiplyScalar();

// Find the minimal value of a matrix
entry_t minEntry();

// Find the maximal value of a matrix
entry_t maxEntry();

void print(const Matrix *const mat);

inline Matrix *createMatrix(const size_t row, const size_t col, const entry_t *const entry) {
    if (!entry) {
        puts("Error in createMatrix(): Invalid entry array pointer!");
        return;
    }
    Matrix *mat = (Matrix *)malloc(sizeof(Matrix)); // force convert the pointer type in support of C++
    mat->row = row;
    mat->col = col;
    size_t size = row*col;
    mat->entry = (entry_t *)malloc(sizeof(entry_t)*size);
    for (size_t i = 0; i < size; ++i) mat->entry[i] = entry[i];
    return mat;
}

inline void deleteMatrix(Matrix *const mat) {
    if (!mat) {
        puts("Error in deleteMatrix(): Invalid matrix pointer!");
        return;
    }
    free(mat->entry);
    free(mat);
}

inline void copyMatrix(Matrix *const a, const Matrix *const b) {
    if (!a || !b) {
        puts("Error in copyMatrix(): Invalid matrices' pointer!");
        return;
    }
    a->row = b->row;
    a->col = b->col;
    size_t size = b->row*b->col;
    realloc(a->entry, size);
    for (size_t i = 0; i < size; ++i) a->entry[i] = b->entry[i];
}

inline void addMatrix(const Matrix *const a, const Matrix *const b) {
    
}

inline void substractMatrix(const Matrix *const a, const Matrix *const b) {

}

inline void multiplyMatrix(const Matrix *const a, const Matrix *const b) {

}

inline void addScalar() {

}

inline void substractScalar() {

}

inline void multiplyScalar() {

}

inline entry_t minEntry() {
    return 0.0;
}

inline entry_t maxEntry() {
    return 0.0;
}

inline void print(const Matrix *const mat) {

}

#endif // __MATRIX_H__