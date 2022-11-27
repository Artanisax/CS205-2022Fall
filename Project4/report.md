[toc]

# CS205-2022Fall Project 4

## Optimizing Matrix Multiplication

**Name:** 陈康睿

**SID:** 12110524

## Part 1 - Analysis & Implementation

In this project, I try to use localization, SIMD, Multithreading, Memory alignment and compile options to accelerate matrix multiplication. I also tried Stressen algorithm.

To simplify and concentrate on optimization, I only implemented functions supporting square matrices. (For Stressen algorithm, square matrices with order $$2^n$$ especially)



### Libraries

```c
#include <stddef.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <immintrin.h>
#include <omp.h>
#include <sys/time.h>
#include <time.h>
#include <stdlib.h>
#include <cblas.h> // OpenBLAS
```





### Data Structure & Basic Operations

It is worth a mention that the memory allocated for entry is aligned for acceleration.

```c
typedef struct Matrix{
    size_t n;
    float *entry;
} Matrix;

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
```



### Plain Multiplication

Simple and basic ijk 3-layer loops implication

```c
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
```



### $$1^{st}$$ Optimization - Loop order & Localization

Noted that in the original loops, each time we need to calculate $$k*n$$, which is time-consuming. We are using row majored storage so we can just exchange j and k so that $$i*n$$ and $$i*k$$,  which can be pre-calculated, then we only need to plus $$j$$ each time.

```c
Matrix *mul_order(const Matrix *const a, const Matrix *const b) {
    size_t n = a->n;
    Matrix *ret = createMatrix(n, NULL);
    bzero(ret->entry, sizeof(float)*(n*n));
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
```



### $$2^{nd}$$ Optimization - SIMD

Since my computer CPU is Intel® i7-10875H, which supports AVX2, so I used Intel® Intrinsics to vectorize multiplication. (When allocate memory for matrix entries, I aligned their address. So we can use `_mm256_###_p#()` other than slower ones `_mm256_###u_p#()` )

```c
Matrix *mul_order_avx_omp(const Matrix *const a, const Matrix *const b) {
    size_t n = a->n;
    Matrix *ret = createMatrix(a->n, NULL);
    bzero(ret->entry, sizeof(float)*(n*n));
    for (size_t i = 0; i < n; ++i) {
        size_t in = i*n;
        for (size_t k = 0; k < n; ++k) {
            float aik = a->entry[in+k];
            size_t kn = k*n;
            // all b[k][j] needs to be multiplied by a[i][k] for c[i][j]
            __m256 a_ik = _mm256_set_ps(aik, aik, aik, aik, aik, aik, aik, aik);
            for (size_t j = 0; j < n; j += 8)
                // ret->entry[in+j] += a_ik*b->entry[kn+j]
                // directly call functions to calculate to save more time
                // though do not read elegant... 
                _mm256_store_ps(ret->entry+in+j, 
                                _mm256_add_ps(
                                    _mm256_load_ps(ret->entry+in+j),
                                    			  _mm256_mul_ps(a_ik, 
                                                  _mm256_load_ps(b->entry+kn+j))));
        }
    }
    return ret;
}
```



### $$3^{rd}$$ Optimization - Multithreading

By the independence inside the loop, we can use OpenMP and put `#pragma omp parallel for ` right before the outermost `for`. 

```c
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
```

### An Unsuccessful Attempt on Stressen Algorithm





## Part 2 - Results & Verification

## Part 3 - Self-review

## Part 4 - Codes & Comments

Please see [My GitHub Repository](https://github.com/Artanisax/CS205-2022Fall/tree/main/Project4)

aligned_alloc

fun discovery: avx2 is more precise than normal arithmetic operation on `float`

