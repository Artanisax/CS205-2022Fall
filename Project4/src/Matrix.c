#include "Matrix.h"
#include <stdio.h>
#include <string.h>
#include <immintrin.h>
#include <omp.h>

Matrix *createMatrix(const size_t row, const size_t col, const float *const entry) {
    Matrix *mat = (Matrix *)malloc(sizeof(Matrix));
    mat->row = row;
    mat->col = col;
    size_t siz = row*col;
    mat->entry = (float *)malloc(sizeof(float)*siz);
    memcpy(mat->entry, entry, sizeof(float)*siz);
    return mat;
}

void deleteMatrix(Matrix *const mat) {
    free(mat->entry);
    free(mat);
}

void copyMatrix(Matrix *const a, const Matrix *const b) {
    a->row = b->row;
    a->col = b->col;
    size_t siz = b->row*b->col;
    a->entry = (float *)realloc(a->entry, sizeof(float)*siz);
    memcpy(a->entry, b->entry, sizeof(float)*siz);
}

Matrix *addMatrix(const Matrix *const a, const Matrix *const b) {
    size_t siz = a->row*a->col;
    float entry[siz];
    for (size_t i = 0; i < siz; ++i)
        entry[i] = a->entry[i]+b->entry[i];
    return createMatrix(a->row, a->col, entry);
}

Matrix *substractMatrix(const Matrix *const a, const Matrix *const b) {
    size_t siz = a->row*a->col;
    float entry[siz];
    for (size_t i = 0; i < siz; ++i)
        entry[i] = a->entry[i]-b->entry[i];
    return createMatrix(a->row, a->col, entry);
}

Matrix *originalMultiplyMatrix(const Matrix *const a, const Matrix *const b) {
    float entry[a->row*b->col];
    bzero(entry, sizeof(entry));
    for (size_t i = 0; i < a->row; ++i)
        for (size_t j = 0; j < b->col; ++j) {
            for (size_t k = 0; k < a->col; ++k)
                entry[i*b->col+j] += a->entry[i*a->col+k]*b->entry[k*b->col+j];
        }
    return createMatrix(a->row, b->col, entry);
}

Matrix *improvedMultiplyMatrix(const Matrix *const a, const Matrix *const b) {
    float entry[a->row*b->col];
    bzero(entry, sizeof(entry));
    for (size_t i = 0; i < a->row; ++i)
        for (size_t k = 0; k < a->col; ++k) {
            float a_ik = a->entry[i*a->col+k];
            size_t ic = i*b->col, kc = k*b->col;
            for (size_t j = 0; j < b->col; ++j)
                entry[ic+j] += a_ik*b->entry[kc+j];
        }
    return createMatrix(a->row, b->col, entry);
}

void printMatrix(const Matrix *const mat) {
    printf("row = %zu, col = %zu\n", mat->row, mat->col);
    for (size_t i = 0; i < mat->row; ++i) {
        for (size_t j = 0; j < mat->col; ++j)
            printf("%.4f\t", mat->entry[i*mat->col+j]);
        putchar('\n');
    }
}
