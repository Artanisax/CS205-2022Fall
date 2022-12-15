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

template <typename T>
void Matrix<T>::uniquify()
{
    if (entry.unique())  return;
    T *src = entry.get();
    entry = shared_ptr<T>(new T[row*col*channel], default_delete<T[]>());
    hard_copy(entry.get(), src, this->channel*this->row*this->col);
}

template <typename T>
bool Matrix<T>::operator==(const Matrix &mat) const
{
    if (channel != mat.channel || row != mat.row || col != mat.col)
        return false;
    if (entry.get() == mat.entry.get())
        return true;
    size_t siz = channel*row*col;
    T *p[2] = {entry.get(), mat.entry.get()};
    for (size_t i = 0; i < siz; ++i)
        if (p[0][i] != p[1][i])  return false;
    return true;
}

template <typename T>
Matrix<T> &Matrix<T>::operator=(const Matrix &mat)
{
    this->channel = mat.channel;
    this->row = mat.row;
    this->col = mat.col;
    this->entry = mat.entry;
}

template <typename T>
Matrix<T> Matrix<T>::operator+(const Matrix &mat) const
{
    Matrix<T> res(this->channel, this->row, this->col, nullptr);
    size_t siz = this->channel*this->row*this->col;
    T *p = res.entry.get(), *src[2] = {this->entry.get(), mat.entry.get()};
    for (size_t i = 0; i < siz; ++i)
        p[i] = src[0][i]+src[1][i];
    return res;
}

template <typename T>
Matrix<T> Matrix<T>::operator-(const Matrix &mat) const
{
    Matrix<T> res(this->channel, this->row, this->col, nullptr);
    size_t siz = this->channel*this->row*this->col;
    T *p = res.entry.get(), *src[2] = {this->entry.get(), mat.entry.get()};
    for (size_t i = 0; i < siz; ++i)
        p[i] = src[0][i]-src[1][i];
    return res;
}