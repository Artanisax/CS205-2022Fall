#include "Matrix.h"
#include <stdio.h>
#include <string.h>
#include <immintrin.h>
#include <omp.h>

Matrix *createMatrix(const size_t n, const float *const entry) {
    Matrix *mat = (Matrix *)malloc(sizeof(Matrix));
    mat->n = n;
    size_t siz = n*n;
    mat->entry = (float *)aligned_alloc(256, sizeof(float)*siz);
    if (entry)
        memcpy(mat->entry, entry, sizeof(float)*siz);
    return mat;
}

void deleteMatrix(Matrix *const mat) {
    free(mat->entry);
    free(mat);
}

Matrix *mul_plain(const Matrix *const a, const Matrix *const b) {
    size_t n = a->n;
    Matrix *ret = createMatrix(n, NULL);
    bzero(ret->entry, sizeof(float)*(n*n));
    for (size_t i = 0; i < n; ++i) {
        for (size_t j = 0; j < n; ++j)
            for (size_t k = 0; k < n; ++k)
                ret->entry[i*n+j] += a->entry[i*n+k]*b->entry[k*n+j];
    }
    return ret;
}

Matrix *mul_order_omp(const Matrix *const a, const Matrix *const b) {
    size_t n = a->n;
    Matrix *ret = createMatrix(n, NULL);
    bzero(ret->entry, sizeof(float)*(n*n));
    #pragma omp parallel for
    for (size_t i = 0; i < n; ++i) {
        size_t in = i*n;
        for (size_t k = 0; k < n; ++k) {
            float a_ik = a->entry[in+k];
            size_t kn = k*n;
            for (size_t j = 0; j < n; ++j)
                ret->entry[in+j] += a_ik*b->entry[kn+j];
        }
    }
    return ret;
}

Matrix *mul_order_avx_omp(const Matrix *const a, const Matrix *const b) {
    size_t n = a->n;
    Matrix *ret = createMatrix(a->n, NULL);
    bzero(ret->entry, sizeof(float)*(n*n));
    #pragma omp parallel for
    for (size_t i = 0; i < n; ++i) {
        size_t in = i*n;
        for (size_t k = 0; k < n; ++k) {
            float aik = a->entry[in+k];
            size_t kn = k*n;
            __m256 a_ik = _mm256_set_ps(aik, aik, aik, aik, aik, aik, aik, aik);
            for (size_t j = 0; j < n; j += 8)
                _mm256_store_ps(ret->entry+in+j, 
                                _mm256_add_ps(_mm256_load_ps(ret->entry+in+j),
                                              _mm256_mul_ps(a_ik, _mm256_load_ps(b->entry+kn+j))));
        }
    }
    return ret;
}

Matrix *addMatrix(const Matrix *const a, const Matrix *const b) {
    size_t n = a->n;
    Matrix *ret = createMatrix(n, NULL);
    size_t siz = n*n;
    #pragma omp parallel for
    for (size_t i = 0; i < siz; i += 8)
        _mm256_store_ps(ret->entry+i, _mm256_add_ps(_mm256_load_ps(a->entry+i),
                                                    _mm256_load_ps(b->entry+i)));
    return ret;
}

Matrix *subMatrix(const Matrix *const a, const Matrix *const b) {
    size_t n = a->n;
    Matrix *ret = createMatrix(n, NULL);
    size_t siz = n*n;
    #pragma omp parallel for
    for (size_t i = 0; i < siz; i += 8)
        _mm256_store_ps(ret->entry+i, _mm256_sub_ps(_mm256_load_ps(a->entry+i),
                                                    _mm256_load_ps(b->entry+i)));
    return ret;
}

Matrix **divide_matrix(const Matrix *const mat) {
    size_t N = mat->n, n = mat->n>>1;
    Matrix **ret = malloc(sizeof(Matrix)*4);
    #pragma omp parallel for
    for (size_t i = 0; i < 4; ++i) ret[i] = createMatrix(n, NULL);
    #pragma omp parallel for
    for (size_t i = 0; i < n; ++i)
        memcpy(ret[0]->entry+i*n, mat->entry+i*N, sizeof(float)*n);
    #pragma omp parallel for
    for (size_t i = 0; i < n; ++i)
        memcpy(ret[1]->entry+i*n, mat->entry+i*N+n, sizeof(float)*n);
    #pragma omp parallel for
    for (size_t i = 0; i < n; ++i)
        memcpy(ret[2]->entry+i*n, mat->entry+(n+i)*N, sizeof(float)*n);
    #pragma omp parallel for
    for (size_t i = 0; i < n; ++i)
        memcpy(ret[3]->entry+i*n, mat->entry+(n+i)*N+n, sizeof(float)*n);
    return ret;
}

Matrix *stressen(const Matrix *const mat) {
    size_t n = mat->n;
    
}

void printMatrix(const Matrix *const mat) {
    size_t n = mat->n;
    printf("n = %zu", n);
    for (size_t i = 0; i < n; ++i) {
        for (size_t j = 0; j < n; ++j)
            printf("%.1f\t", mat->entry[i*n+j]);
        putchar('\n');
    }
}
