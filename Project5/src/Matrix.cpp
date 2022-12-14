#include "Matrix.hpp"



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