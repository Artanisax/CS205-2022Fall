#include "Matrix.hpp"

using std::cout;

int main () {
    double *e[2] = {new double[2], new double[2]};
    e[0][1] = 1, e[1][0] = 2, e[1][1] = 3;
    Matrix<double> m0(1, 1, 2, e[0]);
    Matrix<double> m1(1, 2, 1, e[1]);
    cout << m0 << m1;
    return 0;
}
