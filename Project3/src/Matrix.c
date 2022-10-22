#include "Matrix.h"
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <stdio.h>

// Create an empty matrix with given row, col and entry[]
inline Matrix *createMatrix(const size_t row, const size_t col, const entry_t *const entry) {
    if (!entry) {
        puts("Error in createMatrix(): Invalid entry array pointer!");
        return NULL;
    }
    Matrix *mat = (Matrix *)malloc(sizeof(Matrix)); // convert the pointer type ny force in support of C++
    mat->row = row;
    mat->col = col;
    size_t size = row*col;
    mat->entry = (entry_t *)malloc(sizeof(entry_t)*size);
    for (size_t i = 0; i < size; ++i)
        mat->entry[i] = entry[i];
    return mat;
}

// Delete a matrix(*mat)
inline void deleteMatrix(Matrix *const mat) {
    if (!mat) {
        puts("Error in deleteMatrix(): Invalid matrix pointer!");
        return;
    }
    free(mat->entry);
    free(mat);
}

// Copy data from *b to *a
inline void copyMatrix(Matrix *const a, const Matrix *const b) {
    if (!a || !b) {
        puts("Error in copyMatrix(): Invalid matrix pointers!");
        return;
    }
    a->row = b->row;
    a->col = b->col;
    size_t size = b->row*b->col;
    a->entry = realloc(a->entry, size);
    memcpy(a->entry, b->entry, sizeof(entry_t)*size);
}

// Add up a *and *b
inline Matrix *addMatrix(const Matrix *const a, const Matrix *const b) {
    if (!a || !b) {
        puts("Error in addMatrix(): Invalid matrix pointers!");
        return NULL;
    }
    if (a->row != b->row || a->col != b->col) {
        puts("Error in addMatrix(): Inequal dementions!");
        return NULL;
    }
    size_t size = a->row*a->col;
    entry_t entry[size];
    for (size_t i = 0; i < size; ++i)
        entry[i] = a->entry[i]+b->entry[i];
    return createMatrix(a->row, a->col, entry);
}

// Substract *b from *a
inline Matrix *substractMatrix(const Matrix *const a, const Matrix *const b) {
    if (!a || !b) {
        puts("Error in substractMatrix(): Invalid matrix pointers!");
        return NULL;
    }
    if (a->row != b->row || a->col != b->col) {
        puts("Error in substractMatrix(): Inequal dementions!");
        return NULL;
    }
    size_t size = a->row*a->col;
    entry_t entry[size];
    for (size_t i = 0; i < size; ++i)
        entry[i] = a->entry[i]-b->entry[i];
    return createMatrix(a->row, a->col, entry);
}

// Multiply *a and *b
inline Matrix *multiplyMatrix(const Matrix *const a, const Matrix *const b) {
    if (!a || !b) {
        puts("Error in multiplyMatrix(): Invalid matrix pointers!");
        return NULL;
    }
    if (a->row != b->col || a->col != b->row) {
        puts("Error in multiplyMatrix(): Unpaired dementions!");
        return NULL;
    }
    size_t size = a->row*b->col;
    entry_t entry[size];
    for (size_t i = 0; i < a->row; ++i)
        for (size_t j = 0; j < b->col; ++j) {
            entry[i*b->col+j] = 0.0;
            for (size_t k = 0; k < a->col; ++k)
                entry[i*b->col+j] += a->entry[i*a->col+k]*b->entry[k*b->col+j];
        }
    return createMatrix(a->row, b->col, entry);
}

// Add a scalar(x) to a matrix(*mat)
inline void addScalar(const Matrix *mat, const entry_t x) {
    if (!mat) {
        puts("Error in addScalar(): Invalid matrix pointer!");
        return;
    }
    for (size_t i = 0; i < mat->row*mat->col; ++i)
        mat->entry[i] += x;
}

// Subtract a scalar(x) from a matrix(*mat)
inline void substractScalar(const Matrix *mat, const entry_t x) {
    if (!mat) {
        puts("Error in substractScalar(): Invalid matrix pointer!");
        return;
    }
    for (size_t i = 0; i < mat->row*mat->col; ++i)
        mat->entry[i] -= x;
}

// Multiply a matrix(*mat) with a scalar(x)
inline void multiplyScalar(const Matrix *mat, const entry_t x) {
    if (!mat) {
        puts("Error in multiplyScalar(): Invalid matrix pointer!");
        return;
    }
    for (size_t i = 0; i < mat->row*mat->col; ++i)
        mat->entry[i] *= x;
}

// Divide a matrix(*mat) with a scalar(x)
inline void divideScalar(const Matrix *mat, const entry_t x) {
    if (!mat) {
        puts("Error in multiplyScalar(): Invalid matrix pointer!");
        return;
    }
    for (size_t i = 0; i < mat->row*mat->col; ++i)
        mat->entry[i] /= x;
}

// Find the minimum of two entry elements a and b
inline entry_t min(entry_t a, entry_t b) {
    return a < b ? a : b;
}

// Find the maximum of two entry elements a and b
inline entry_t max(entry_t a, entry_t b) {
    return a > b ? a : b;
}

// Find the minimum entry of a matrix(*mat)
inline entry_t minEntry(const Matrix *const mat) {
    if (!mat) {
        puts("Error in minEntry(): Invalid matrix pointer!");
        return NAN;
    }
    entry_t result = mat->entry[0];
    for (size_t i = 1; i < mat->row*mat->col; ++i)
        result = min(result, mat->entry[i]);
    return result;
}

// Find the maximum entry of a matrix(*mat)
inline entry_t maxEntry(const Matrix *const mat) {
    if (!mat) {
        puts("Error in maxEntry(): Invalid matrix pointer!");
        return NAN;
    }
    entry_t result = mat->entry[0];
    for (size_t i = 1; i < mat->row*mat->col; ++i)
        result = max(result, mat->entry[i]);
    return result;
}

// Print a matrix(*mat)
inline void print(const Matrix *const mat) {
    if (!mat) {
        puts("Error in print(): Invalid matrix pointer!");
        return;
    }
    printf("row = %ld, col = %ld\n", mat->row, mat->col);
    for (size_t i = 0; i < mat->row; ++i) {
        for (size_t j = 0; j < mat->col; ++j)
            printf(entry_place_holder, mat->entry[i*mat->col+j]);
        putchar('\n');
    }
}
