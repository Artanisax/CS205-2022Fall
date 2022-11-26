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
    if (entry)
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
    Matrix *ret = createMatrix(a->row, a->col, NULL);
    size_t siz = a->row*a->col;
    for (size_t i = 0; i < siz; ++i)
        ret->entry[i] = a->entry[i]+b->entry[i];
    return ret;
}

Matrix *mul_plain(const Matrix *const a, const Matrix *const b) {
    Matrix *ret = createMatrix(a->row, b->col, NULL);
    bzero(ret->entry, sizeof(float)*(a->row*b->col));
    for (size_t i = 0; i < a->row; ++i) {
        for (size_t j = 0; j < b->col; ++j)
            for (size_t k = 0; k < a->col; ++k)
                ret->entry[i*b->col+j] += a->entry[i*a->col+k]*b->entry[k*b->col+j];
    }
    return ret;
}

Matrix *mul_loop_order_omp(const Matrix *const a, const Matrix *const b) {
    Matrix *ret = createMatrix(a->row, b->col, NULL);
    bzero(ret->entry, sizeof(float)*(a->row*b->col));
    #pragma omp parallel for
    for (size_t i = 0; i < a->row; ++i)
        for (size_t k = 0; k < a->col; ++k) {
            float a_ik = a->entry[i*a->col+k];
            size_t ic = i*b->col, kc = k*b->col;
            for (size_t j = 0; j < b->col; ++j)
                ret->entry[ic+j] += a_ik*b->entry[kc+j];
        }
    return ret;
}

Matrix *mul_avx_omp(const Matrix *const a, const Matrix *const b) {

}

void printMatrix(const Matrix *const mat) {
    printf("row = %zu, col = %zu\n", mat->row, mat->col);
    for (size_t i = 0; i < mat->row; ++i) {
        for (size_t j = 0; j < mat->col; ++j)
            printf("%.4f\t", mat->entry[i*mat->col+j]);
        putchar('\n');
    }
}
