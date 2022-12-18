#include "Matrix.hpp"

int main () {
    // double *e[2] = {new double[4], new double[4]};
    // e[0][0] = 1, e[0][1] = 2, e[0][2] = 3, e[0][3] = 4;
    // e[1][0] = 4, e[1][1] = 3, e[1][2] = 2, e[1][3] = 1;
    // Matrix<double> m0(2, 1, 2, e[0]);
    // Matrix<double> m1(2, 1, 2, e[1]);
    // cout << m0 << ((m0+=1)/=0.5) << m0;
    double *e = new double[16];
    for (size_t i = 0; i < 16; ++i)
        e[i] = i;
    Matrix mat(1, 4, 4, e);
    Matrix m1(mat, 0, 0, 2, 2), m2(mat, 2, 2, 2, 2);
    cout << mat << m1 << m2 << m1*m2/2;
    cout << m1.get(1, 0, 1) << '\n';
    // cout << mat.entry.use_count() << endl;
    return 0;
}
