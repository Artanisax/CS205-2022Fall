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
    Matrix **ret = malloc(sizeof(Matrix *)*4);

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

Matrix *merge_matrix(Matrix **mat) {
    size_t n = mat[0]->n, N = mat[0]->n<<1;
    Matrix *ret = createMatrix(N, NULL);

    #pragma omp parallel for
    for (size_t i = 0; i < n; ++i)
        memcpy(ret->entry+i*N, mat[0]->entry+i*n, sizeof(float)*n);
    #pragma omp parallel for
    for (size_t i = 0; i < n; ++i)
        memcpy(ret->entry+i*N+n, mat[1]->entry+i*n, sizeof(float)*n);
    #pragma omp parallel for
    for (size_t i = 0; i < n; ++i)
        memcpy(ret->entry+(n+i)*N, mat[2]->entry+i*n, sizeof(float)*n);
    #pragma omp parallel for
    for (size_t i = 0; i < n; ++i)
        memcpy(ret->entry+(n+i)*N+n, mat[3]->entry+i*n, sizeof(float)*n);

    return ret;
}

Matrix *stressen(const Matrix *const a, const Matrix *const b) {
    size_t n = a->n;
    if (n <= 64)
        return mul_order_avx_omp(a, b);
    
    Matrix *ret, *mat[7], *c[4],
           **sub_a = divide_matrix(a),
           **sub_b = divide_matrix(b);
    #pragma omp parallel sections
    {
        #pragma omp section
        {
            Matrix *t[2];
            t[0] = subMatrix(sub_a[1], sub_a[3]);
            t[1] = addMatrix(sub_b[2], sub_b[3]);
            mat[0] = stressen(t[0], t[1]);
            deleteMatrix(t[0]);
            deleteMatrix(t[1]);
        }
        #pragma omp section
        {
            Matrix *t[2];
            t[0] = addMatrix(sub_a[0], sub_a[3]);
            t[1] = addMatrix(sub_b[0], sub_b[3]);
            mat[1] = stressen(t[0], t[1]);
            deleteMatrix(t[0]);
            deleteMatrix(t[1]);
        }
        #pragma omp section
        {
            Matrix *t[2];
            t[0] = subMatrix(sub_a[2], sub_a[0]);
            t[1] = addMatrix(sub_b[0], sub_b[1]);
            mat[2] = stressen(t[0], t[1]);
            deleteMatrix(t[0]);
            deleteMatrix(t[1]);
        }
        #pragma omp section
        {
            Matrix *temp;
            temp = addMatrix(sub_a[0], sub_a[1]);
            mat[3] = stressen(temp, sub_b[3]);
            deleteMatrix(temp);
        }
        #pragma omp section
        {
            Matrix *temp;
            temp = subMatrix(sub_b[1], sub_b[3]);
            mat[4] = stressen(temp, sub_a[0]);
            deleteMatrix(temp);
        }
        #pragma omp section
        {
            Matrix *temp;
            temp = subMatrix(sub_b[2], sub_b[0]);
            mat[5] = stressen(temp, sub_a[3]);
            deleteMatrix(temp);
        }
        #pragma omp section
        {
            Matrix *temp;
            temp = addMatrix(sub_a[2], sub_a[3]);
            mat[6] = stressen(temp, sub_b[0]);
            deleteMatrix(temp);
        }
    }

    #pragma omp parallel sections
    {
        #pragma omp section
        {
            Matrix *t[2];
            t[0] = addMatrix(mat[0], mat[1]);
            t[1] = subMatrix(mat[5], mat[3]);
            c[0] = addMatrix(t[0], t[1]);
            deleteMatrix(t[0]);
            deleteMatrix(t[1]);
        }
        #pragma omp section
        {
            c[1] = addMatrix(mat[3], mat[4]);
        }
        #pragma omp section
        {
            c[2] = addMatrix(mat[5], mat[6]);
        }
        #pragma omp section
        {
            Matrix *t[2];
            t[0] = addMatrix(mat[1], mat[2]);
            t[1] = subMatrix(mat[4], mat[6]);
            c[3] = addMatrix(t[0], t[1]);
            deleteMatrix(t[0]);
            deleteMatrix(t[1]);
        }
    }

    #pragma omp parallel for
    for (size_t i = 0; i < 4; ++i) {
        deleteMatrix(sub_a[i]);
        deleteMatrix(sub_b[i]);
    }
    free(sub_a);
    free(sub_b);

    ret = merge_matrix(c);
    #pragma omp parallel for
    for(size_t i = 0; i < 4; ++i)
        deleteMatrix(c[i]);
    
    return ret;
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
