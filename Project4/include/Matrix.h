#ifndef __MATRIX_H__
#define __MATRIX_H__

#include <stddef.h>
#include <stdbool.h>

// Matrix structure
typedef struct Matrix{
    size_t n;
    float *entry;
} Matrix;


Matrix *createMatrix(const size_t n, const float *const entry);

void deleteMatrix(Matrix *const mat);

Matrix *mul_plain(const Matrix *const a, const Matrix *const b);

Matrix *mul_order_omp(const Matrix *const a, const Matrix *const b);

Matrix *mul_order_avx_omp(const Matrix *const a, const Matrix *const b);

Matrix *addMatrix(const Matrix *const a, const Matrix *const b);

Matrix *subMatrix(const Matrix *const a, const Matrix *const b);

Matrix **divide_matrix(const Matrix *const mat);

void printMatrix(const Matrix *const mat);

#endif // __MATRIX_H__
