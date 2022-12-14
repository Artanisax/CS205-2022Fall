#include "Matrix.hpp"

template <typename T>
void Matrix<T>::hard_copy(T *dest, const T *src, const size_t siz)
{
    switch (typeid(T).name())
    {
        case typeid(short).name():
        case typeid(int).name():
        case typeid(long long).name():
        case typeid(float).name():
        case typeid(double).name():
            memcpy(dest, src, siz*sizeof(T));
            break;

        default:
            for (size_t i = 0; i < siz; ++i)
                dest[i] = src[i];
            break;
    }
}

template <typename T>
string Matrix<T>::to_string() const
{
    string s;
    T *p = entry.get();
    size_t area = row*col;
    for (size_t i = 0; i < channel; ++i)
    {
        for (size_t j = 0, head_row = i*area; j < row; ++j)
        {
            for (size_t k = 0, head_col = head_row+j*col; k < col; ++k)
                s += std::to_string(p[head_col+k])+" ";
            s += "\n";
        }
        s += "\n";
    }
    return s;
}