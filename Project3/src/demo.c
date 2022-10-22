#include "Matrix.h"

int main () {
    float entry[] = {1, 1, 1, 1, 1, 1, 1, 1, 1};
    Matrix *mat = createMatrix(3, 3, entry);
    printf("%p %p\n", mat, mat->entry);
    deleteMatrix(mat);
    
    printf("%p %p\n", mat, mat->entry);
    return 0;
}