#include "Matrix.hpp"

int main () {
    double *e[2] = {new double[4], new double[4]};
    e[0][0] = 1, e[0][1] = 2, e[0][2] = 3, e[0][3] = 4;
    e[1][0] = 4, e[1][1] = 3, e[1][2] = 2, e[1][3] = 1;
    Matrix<double> m0(2, 1, 2, e[0]);
    Matrix<double> m1(2, 1, 2, e[1]);
    cout << m0 << m1 << (m0+=m1);
    // double *entry = new double[4];
    // entry[0] = 0, entry[1] = 1, entry[2] = 2, entry[3] = 3;
    // Matrix<double> m0(2, 1, 2, entry);
    // Matrix<double> m1(2, 1, 2, entry);
    // cout << (m0-m1).to_string();
    return 0;
}
