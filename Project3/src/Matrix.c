#include "Matrix.h"
#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>

inline Matrix *createMatrix(const size_t row, const size_t col, const entry_t *const entry) {
    if (!entry) {
        puts("Error in createMatrix(): Invalid entry array pointer!");
        return NULL;
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

inline Matrix *addMatrix(const Matrix *const a, const Matrix *const b) {
    if (!a || !b) {
        puts("Error in addMatrix(): Invalid matrices' pointer!");
        return;
    }
    if (a->row != b->row || a->col != b->col) {
        puts("Error in addMatrix(): Inequal dementions!");
        return NULL;
    }
    size_t size = a->row*a->col;
    entry_t result[size];
    for (size_t i = 0; i < size; ++i) result[i] = a->entry[i]+b->entry[i];
    return createMatrix(a->row, a->col, result);
}

inline Matrix *substractMatrix(const Matrix *const a, const Matrix *const b) {
    if (!a || !b) {
        puts("Error in substractMatrix(): Invalid matrices' pointer!");
        return;
    }
    if (a->row != b->row || a->col != b->col) {
        puts("Error in substractMatrix(): Inequal dementions!");
        return NULL;
    }
    size_t size = a->row*a->col;
    entry_t result[size];
    for (size_t i = 0; i < size; ++i) result[i] = a->entry[i]-b->entry[i];
    return createMatrix(a->row, a->col, result);
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