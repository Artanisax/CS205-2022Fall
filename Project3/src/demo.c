#include "Matrix.h"
#include <stdio.h>

int main () {
    entry_t e1[] = {1, 0,
                    0, 1,
                    1, 1},
            e2[] = {1, 1, 0.5,
                    0.5, 0.5, 1};
    Matrix *m1 = createMatrix(3, 2, e1), *m2 = createMatrix(2, 3, e2);
    
    GaussianEliminate(m1);
    printMatrix(m1);
    printMatrix(m1+1);

    substractScalar(m2, 0.5);
    Matrix *result = multiplyMatrix(m1, m2);
    printf("rank = %zu\n", rank(result));

    copyMatrix(m1, result);
    printMatrix(m1);

    deleteMatrix(result);
    MPL_clear();
    return 0;
}
