#include "Matrix.h"

int main () {
    entry_t ea[] = {1, 0, 1, 1,
                    0, 1, 0, 1,
                    1, 0, 1, 1},
            eb[] = {1, 0, 1,
                    0, 1, 0,
                    0, 0, 1,
                    0, 0, 0};
    Matrix *a = createMatrix(3, 4, ea), *b = createMatrix(3, 4, eb);
    multiplyScalar(a, 1.4);
    Matrix *result = substractMatrix(a, b);
    print(result);
    return 0;
}
