#include "Matrix.hpp"

using std::cout;

const size_t N = 128;

int main () {
    double entry[4] = {0.0, 1.0, 2.0, 3.0};
    Matrix mat(2, 2, &entry);
    cout << mat.to_string();
    return 0;
}
