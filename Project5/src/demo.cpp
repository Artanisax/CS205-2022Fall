#include "Matrix.hpp"

int main () {
    double *e = new double(16);
    for (size_t i = 0; i < 16; ++i)
        e[i] = i;
    
    return 0;
}
