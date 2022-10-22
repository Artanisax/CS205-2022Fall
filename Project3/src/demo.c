#include "Matrix.h"

int main () {
    entry_t ea[] = {1, 0, 1, 1,
                    0, 1, 0, 1,
                    1, 0, 1, 1},
            eb[] = {1, 0, 1,
                    0, 1, 0,
                    0, 0, 1,
                    0, 0, 0};
    Matrix *a = createMatrix(3, 4, ea), *b = createMatrix(4, 3, eb);
    multiplyScalar(a, 1.3);
    print(a);
    return 0;
}