#include "Matrix.h"
#include <stdio.h>

int main () {
    entry_t entry[] = {1, 0, 2,
                       0, 1, 0,
                       1, 0, 1};
    Matrix *mat = createIMatrix(3);
    printMatrix(mat);
    printf(entry_place_holder"\n", det(mat));
    return 0;
}
