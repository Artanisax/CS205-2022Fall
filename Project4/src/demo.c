#include "Matrix.h"
#include <stdio.h>
#include <sys/time.h>
#include <time.h>
#include <stdlib.h>
#include <cblas.h>

#include <immintrin.h>

const size_t N = 128;

int main () {
    srand(time(NULL));
    struct timeval st, ed;
    float *e1 = (float *)aligned_alloc(256, sizeof(float)*N*N),
          *e2 = (float *)aligned_alloc(256, sizeof(float)*N*N),
          *cblas_ans = (float *)aligned_alloc(256, sizeof(float)*N*N);
    #pragma omp parallel for
    for (size_t i = 0; i < N*N; ++i) {
        e1[i] = 1.0*rand()/rand();
        e2[i] = 1.0*rand()/rand();
    }
    Matrix *m1 = createMatrix(N, e1), *m2 = createMatrix(N, e2);
    puts(" Initiallized");

    gettimeofday(&st, NULL);
    Matrix *mp = mul_plain(m1, m2);
    gettimeofday(&ed, NULL);
    printf(" Plain: %.1f %.1lf  %.0fms\n", mp->entry[114], mp->entry[514], 1.0*((ed.tv_sec-st.tv_sec)*1e6+(ed.tv_usec-st.tv_usec))/1e3);

    gettimeofday(&st, NULL);
    Matrix *mi = mul_order_omp(m1, m2);
    gettimeofday(&ed, NULL);
    printf(" Order+OpenMP: %.1f %.1lf  %.0fms\n", mi->entry[114], mi->entry[514], 1.0*((ed.tv_sec-st.tv_sec)*1e6+(ed.tv_usec-st.tv_usec))/1e3);
    gettimeofday(&st, NULL);

    Matrix *mii = mul_order_avx_omp(m1, m2);
    gettimeofday(&ed, NULL);
    printf(" Order+OpenMP+avx2: %.1f %.1lf  %.0fms\n", mii->entry[114], mii->entry[514], 1.0*((ed.tv_sec-st.tv_sec)*1e6+(ed.tv_usec-st.tv_usec))/1e3);
    gettimeofday(&st, NULL);
    
    gettimeofday(&st, NULL);
    Matrix *ms = stressen(m1, m2);
    gettimeofday(&ed, NULL);
    printf(" Stressen: %.1f %.1lf  %.0fms\n", ms->entry[114], ms->entry[514], 1.0*((ed.tv_sec-st.tv_sec)*1e6+(ed.tv_usec-st.tv_usec))/1e3);

    gettimeofday(&st, NULL);
    cblas_sgemm(CblasRowMajor, CblasNoTrans, CblasNoTrans, N, N, N, 1.0, e1, N, e2, N, 0.0, cblas_ans, N);
    gettimeofday(&ed, NULL);
    printf(" OpenBLAS: %.1f %.1lf  %.0fms\n", cblas_ans[114], cblas_ans[514], 1.0*((ed.tv_sec-st.tv_sec)*1e6+(ed.tv_usec-st.tv_usec))/1e3);

    // size_t t = 10000000;
    // float ans = 0;
    // float *temp = (float *)aligned_alloc(256, sizeof(float)*8);
    // for (int i = 0; i < 8; ++i) temp[i] = i;
    // gettimeofday(&st, NULL);
    // for (int i = 0; i < t; ++i)
    //     for (int j = 0; j < 8; j++)
    //         ans += temp[j];
    // gettimeofday(&ed, NULL);
    // printf(" %.1f %.0fms\n", ans, 1.0*((ed.tv_sec-st.tv_sec)*1e6+(ed.tv_usec-st.tv_usec))/1e3);
    // gettimeofday(&st, NULL);
    // float sum[8];
    // __m256 a = _mm256_setzero_ps(), b;
    // for (int i = 0; i < t; ++i) {
    //     b = _mm256_load_ps(temp);
    //     a = _mm256_add_ps(a, b);
    // }
    // a = _mm256_hadd_ps(a, a);
    // a = _mm256_hadd_ps(a, a);
    // _mm256_store_ps(sum, a);
    // ans = sum[0]+sum[4];
    // gettimeofday(&ed, NULL);
    // printf(" %.1f %.0fms\n", ans, 1.0*((ed.tv_sec-st.tv_sec)*1e6+(ed.tv_usec-st.tv_usec))/1e3);

    // free(e1);
    // free(e2);
    // free(cblas_ans);
    // deleteMatrix(m1);
    // deleteMatrix(m2);
    // deleteMatrix(mp);
    // deleteMatrix(mi);
    // deleteMatrix(mii);
    // deleteMatrix(ms);

    return 0;
}
