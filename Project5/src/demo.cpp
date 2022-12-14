#include "Matrix.hpp"

using std::cout;

int main () {
    double *entry = new double[4];
    entry[1] = 1.1, entry[2] = 2.2, entry[3] = 3.3;
    Matrix mat(1, 2, 2, entry);
    
    return 0;
}
