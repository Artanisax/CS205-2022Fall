#include "Matrix.h"
#include <stdio.h>

int main () {
    entry_t entry[] = {1, 0,
                       0, 1,
                       1, 1};
    Matrix *mat = createMatrix(3, 2, entry);
    GaussianEliminate(mat);
    printMatrix(mat);
    printf("%zu\n", rank(mat));
    return 0;
}
