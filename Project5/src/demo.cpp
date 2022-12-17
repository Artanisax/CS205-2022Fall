#include "Matrix.hpp"

using std::cout;

int main () {
    double *e[2] = {new double[4], new double[4]};
    e[0][0] = 0, e[0][1] = 1, e[0][2] = 2, e[0][3] = 3;
    e[1][0] = 2, e[1][1] = 3, e[1][2] = 1, e[1][3] = 0;
    Matrix<double> m0(2, 1, 2, e[0]);
    Matrix<double> m1(2, 2, 1, e[1]);
    cout << m0*m1;
    return 0;
}
