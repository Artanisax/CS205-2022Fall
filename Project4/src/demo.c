#include "Matrix.h"
#include <stdio.h>
#include <sys/time.h>
#include <time.h>
#include <stdlib.h>

int main () {
    srand(time(NULL));
    struct timeval st, ed;
    float e1[1000000], e2[1000000];
    for (size_t i = 0; i < 100000; ++i) e1[i] = 1.0*rand()/rand();
    for (size_t i = 0; i < 100000; ++i) e2[i] = 1.0*rand()/rand();
    puts(" Initiallized");
    Matrix *m1 = createMatrix(2000, 500, e1), *m2 = createMatrix(500, 2000, e2);
    puts(" Created");
    gettimeofday(&st, NULL);
    Matrix *mo = originalMultiplyMatrix(m1, m2);
    gettimeofday(&ed, NULL);
    printf(" %.4f %.0f\n", mo->entry[114], 1.0*((ed.tv_sec-st.tv_sec)*1e6+(ed.tv_usec-st.tv_usec))/1e3);
    // printMatrix(mo);
    gettimeofday(&st, NULL);
    Matrix *mi = improvedMultiplyMatrix(m1, m2);
    gettimeofday(&ed, NULL);
    printf(" %.4f %.0f\n", mi->entry[114], 1.0*((ed.tv_sec-st.tv_sec)*1e6+(ed.tv_usec-st.tv_usec))/1e3);
    // printMatrix(mi);
    return 0;
}
