#pragma once

#include <cstddef>
#include <memory>
#include <string>
#include <cstring>
#include <iostream>

using std::shared_ptr;
using std::make_shared;
using std::default_delete;
using std::string;
using std::ostream;

template <typename T>
class Matrix
{
private:
	size_t channel, row, col;
	shared_ptr<T> entry;

	static void hard_copy(T *dest, const T *src, const size_t siz);

	static void set_zero(T *p, size_t siz);

public:
	Matrix(const size_t channel, const size_t row, const size_t col, const T *entry):
		channel(channel), row(row), col(col), entry(new T[row*col*channel], default_delete<T[]>())
	{ if (entry)  hard_copy(this->entry.get(), entry, row*col*channel); }

	Matrix(const Matrix &mat): row(mat.row), col(mat.col), entry(mat.entry) {}

	T get(size_t k, size_t i, size_t j) const
	{ return entry.get()[k*row*col+i*col+j]; }

	string to_string() const;

	void uniquify();

	bool operator==(const Matrix &mat) const;

	Matrix &operator=(const Matrix &mat);

	Matrix operator+(const Matrix &mat) const;

	Matrix operator-(const Matrix &mat) const;

	Matrix operator*(const Matrix &mat) const;

	Matrix operator+(const T &x) const;

	Matrix operator-(const T &x) const;

	Matrix operator*(const T &x) const;

	Matrix operator/(const T &x) const;

	Matrix &operator+=(const Matrix &mat);

	Matrix &operator-=(const Matrix &mat);

	Matrix &operator*=(const Matrix &mat);

	Matrix &operator+=(const T &x);

	Matrix &operator-=(const T &x);

	Matrix &operator*=(const T &x);

	Matrix &operator/=(const T &x);

	friend Matrix operator+(const T &x, const Matrix<T> &mat)
	{ return mat+x; }

	friend Matrix operator-(const T &x, const Matrix<T> &mat)
	{ return mat-x; }

	friend Matrix operator*(const T &x, const Matrix<T> &mat)
	{ return mat*x; }

	friend Matrix operator/(const T &x, const Matrix<T> &mat)
	{ return mat/x; }

	friend ostream &operator<<(ostream &os, const Matrix<T> &mat)
	{ return os << mat.to_string(); }
};

template <typename T>
void Matrix<T>::hard_copy(T *dest, const T *src, const size_t siz)
{
    if (typeid(T) == typeid(short) || typeid(T) == typeid(int) ||
		typeid(T) == typeid(long long) || typeid(T) == typeid(float) ||
		typeid(T) == typeid(double))
		memcpy(dest, src, siz*sizeof(T));
	else
		for (size_t i = 0; i < siz; ++i)
			dest[i] = src[i];
}

template <typename T>
void Matrix<T>::set_zero(T *p, const size_t siz)
{
    if (typeid(T) == typeid(short) || typeid(T) == typeid(int) ||
		typeid(T) == typeid(long long) || typeid(T) == typeid(float) ||
		typeid(T) == typeid(double))
		bzero(p, siz*sizeof(T));
	else
		for (size_t i = 0; i < siz; ++i)
			dest[i] = 0;
}

template <typename T>
string Matrix<T>::to_string() const
{
    string s = "\n";
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
    uniquify();
    channel = mat.channel;
    row = mat.row;
    col = mat.col;
    entry = mat.entry;
}

template <typename T>
Matrix<T> Matrix<T>::operator+(const Matrix &mat) const
{
    Matrix<T> res(this->channel, this->row, this->col, nullptr);
    size_t siz = this->channel*this->row*this->col;
    T *dest = res.entry.get(), *src[2] = {this->entry.get(), mat.entry.get()};
    for (size_t i = 0; i < siz; ++i)
        dest[i] = src[0][i]+src[1][i];
    return res;
}

template <typename T>
Matrix<T> Matrix<T>::operator-(const Matrix &mat) const
{
    Matrix<T> res(channel, row, col, nullptr);
    size_t siz = channel*row*col;
    T *dest = res.entry.get(), *src[2] = {entry.get(), mat.entry.get()};
    for (size_t i = 0; i < siz; ++i)
        dest[i] = src[0][i]-src[1][i];
    return res;
}

template <typename T>
Matrix<T> Matrix<T>::operator*(const Matrix &mat) const
{
    Matrix<T> res(channel, row, mat.col, nullptr);
    T *dest = res.entry.get(), *src[2] = {entry.get(), mat.entry.get()};
    set_zero(dest, channel*row*mat.col);
    for (size_t t = 0, area[3] = {row*col, mat.row*mat.col, row*mat.col};          t < channel; ++t)
    for (size_t i = 0, head_row[3] = {t*area[0], t*area[1], t*area[2]};            i < row;     ++i)
    for (size_t k = 0, head_i = head_row[0]+i*col, head_p = head_row[2]+i*mat.col; k < col;     ++k)
    for (size_t j = 0, ik = head_i+k, head_k = head_row[1]+k*mat.col;              j < mat.col; ++j)
        dest[head_p+j] += src[0][ik]*src[1][head_k+j];
	return res;
}

template <typename T>
Matrix<T> Matrix<T>::operator+(const T &x) const
{
    Matrix<T> res(channel, row, col, nullptr);
    T *dest = res.entry.get(), *src = entry.get();
    for (size_t i = 0, siz = channel*row*col; i < siz; ++i)
        dest[i] = src[i]+x;
    return res;
}

template <typename T>
Matrix<T> Matrix<T>::operator-(const T &x) const
{
    Matrix<T> res(channel, row, col, nullptr);
    T *dest = res.entry.get(), *src = entry.get();
    for (size_t i = 0, siz = channel*row*col; i < siz; ++i)
        dest[i] = src[i]-x;
    return res;
}

template <typename T>
Matrix<T> Matrix<T>::operator*(const T &x) const
{
    Matrix<T> res(channel, row, col, nullptr);
    T *dest = res.entry.get(), *src = entry.get();
    for (size_t i = 0, siz = channel*row*col; i < siz; ++i)
        dest[i] = src[i]*x;
    return res;
}

template <typename T>
Matrix<T> Matrix<T>::operator/(const T &x) const
{
    Matrix<T> res(channel, row, col, nullptr);
    T *dest = res.entry.get(), *src = entry.get();
    for (size_t i = 0, siz = channel*row*col; i < siz; ++i)
        dest[i] = src[i]/x;
    return res;
}

template <typename T>
Matrix<T> &Matrix<T>::operator+=(const Matrix &mat)
{
    uniquify();
    T *dest = entry.get(), *src = mat.entry.get();
    for (size_t i = 0, siz = channel*row*col; i < siz; ++i)
        dest[i] += src[i];
    return *this;
}

template <typename T>
Matrix<T> &Matrix<T>::operator-=(const Matrix &mat)
{
    uniquify();
    T *dest = entry.get(), *src = mat.entry.get();
    for (size_t i = 0, siz = channel*row*col; i < siz; ++i)
        dest[i] -= src[i];
    return *this;
}

template <typename T>
Matrix<T> &Matrix<T>::operator*=(const Matrix &mat)
{
    *this = (*this)*mat;
    return *this;
}

template <typename T>
Matrix<T> &Matrix<T>::operator+=(const T &x)
{
    T *p = entry.get();
    for (size_t i = 0, siz = channel*row*col; i < siz; ++i)
        p[i] += x;
    return *this;
}

template <typename T>
Matrix<T> &Matrix<T>::operator-=(const T &x)
{
    T *p = entry.get();
    for (size_t i = 0, siz = channel*row*col; i < siz; ++i)
        p[i] -= x;
    return *this;
}

template <typename T>
Matrix<T> &Matrix<T>::operator*=(const T &x)
{
    T *p = entry.get();
    for (size_t i = 0, siz = channel*row*col; i < siz; ++i)
        p[i] *= x;
    return *this;
}

template <typename T>
Matrix<T> &Matrix<T>::operator/=(const T &x)
{
    T *p = entry.get();
    for (size_t i = 0, siz = channel*row*col; i < siz; ++i)
        p[i] /= x;
    return *this;
}
