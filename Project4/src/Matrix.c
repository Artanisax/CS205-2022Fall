#include "Matrix.h"
#include <stdio.h>
#include <immintrin.h>

Matrix *createMatrix(const size_t row, const size_t col, const entry_t *const entry) {
    Matrix *mat = (Matrix *)malloc(sizeof(Matrix));
    mat->row = row;
    mat->col = col;
    size_t siz = row*col;
    mat->entry = (entry_t *)malloc(sizeof(entry_t)*siz);
    for (size_t i = 0; i < siz; ++i)
        mat->entry[i] = entry[i];
    MPL_push_front(mat);
    return mat;
}

void deleteMatrix(Matrix *const mat) {
    MPL_erase(mat);
    free(mat->entry);
    free(mat);
}

void copyMatrix(Matrix *const a, const Matrix *const b) {
    a->row = b->row;
    a->col = b->col;
    size_t siz = b->row*b->col;
    a->entry = (entry_t *)realloc(a->entry, sizeof(entry_t)*siz);
    memcpy(a->entry, b->entry, sizeof(entry_t)*siz);
}

Matrix *addMatrix(const Matrix *const a, const Matrix *const b) {
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

Matrix *substractMatrix(const Matrix *const a, const Matrix *const b) {
    size_t size = a->row*a->col;
    entry_t entry[size];
    for (size_t i = 0; i < size; ++i)
        entry[i] = a->entry[i]-b->entry[i];
    return createMatrix(a->row, a->col, entry);
}

Matrix *originalMultiplyMatrix(const Matrix *const a, const Matrix *const b) {
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

Matrix *improvedMultiplyMatrix(const Matrix *const a, const Matrix *const b) {

}

void printMatrix(const Matrix *const mat) {
    printf("row = %zu, col = %zu\n", mat->row, mat->col);
    for (size_t i = 0; i < mat->row; ++i) {
        for (size_t j = 0; j < mat->col; ++j)
            printf(entry_place_holder, mat->entry[i*mat->col+j]);
        putchar('\n');
    }
}
