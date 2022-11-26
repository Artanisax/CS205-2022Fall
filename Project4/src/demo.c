#include "Matrix.h"
#include <stdio.h>
#include <sys/time.h>
#include <time.h>
#include <stdlib.h>

float e1[10000000], e2[10000000];

int main () {
    srand(time(NULL));
    struct timeval st, ed;
    #pragma omp parallel for
    for (size_t i = 0; i < 10000000; ++i) {
        e1[i] = 1.0*rand()/rand();
        e2[i] = 1.0*rand()/rand();
    }
    puts(" Initiallized");
    Matrix *m1 = createMatrix(1024, 1024, e1), *m2 = createMatrix(1024, 1024, e2);
    puts(" Created");
    gettimeofday(&st, NULL);
    Matrix *mp = mul_plain(m1, m2);
    gettimeofday(&ed, NULL);
    printf(" %.4f %.4lf  %.0fms\n", mp->entry[114], mp->entry[514], 1.0*((ed.tv_sec-st.tv_sec)*1e6+(ed.tv_usec-st.tv_usec))/1e3);
    deleteMatrix(mul_loop_order_omp(m1, m2));
    gettimeofday(&st, NULL);
    Matrix *mi = mul_loop_order_omp(m1, m2);
    gettimeofday(&ed, NULL);
    printf(" %.4f %.4lf  %.0fms\n", mi->entry[114], mi->entry[514], 1.0*((ed.tv_sec-st.tv_sec)*1e6+(ed.tv_usec-st.tv_usec))/1e3);
    deleteMatrix(m1);
    deleteMatrix(m2);
    deleteMatrix(mp);
    deleteMatrix(mi);
    return 0;
}
