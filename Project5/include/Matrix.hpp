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
using std::cout;
using std::cerr;
using std::endl;

template <typename T>
class Matrix
{
private:
	size_t channel, row, col;  // Three dimensions of the original matrix
	size_t roi;                // The starting index of the the region of interest
	size_t r, c;               // Two dimensions of the interesting matrix
	shared_ptr<T> entry;       // A shared pointer managing the original data

public:
	Matrix(const size_t channel, const size_t row, const size_t col, const T *data);

	Matrix(const Matrix &mat);

	Matrix(const Matrix &mat, size_t hr, size_t hc, size_t r, size_t c);

	T get(size_t k, size_t i, size_t j) const;

	string to_string() const;

	void print() const { cout << to_string(); }

	void uniquify();

	bool operator==(const Matrix &mat) const;

	Matrix operator+(const Matrix &mat) const;

	Matrix operator-(const Matrix &mat) const;

	Matrix operator*(const Matrix &mat) const;

	Matrix operator+(const T x) const;

	Matrix operator-(const T x) const;

	Matrix operator*(const T x) const;

	Matrix operator/(const T x) const;
	
	// Matrix &operator=(const Matrix &mat);

	Matrix &operator+=(const Matrix &mat);

	Matrix &operator-=(const Matrix &mat);

	Matrix &operator*=(const Matrix &mat);
	
	Matrix &operator=(const T x);

	Matrix &operator+=(const T x);

	Matrix &operator-=(const T x);

	Matrix &operator*=(const T x);

	Matrix &operator/=(const T x);

	friend Matrix operator+(const T x, const Matrix<T> &mat)
	{ return mat+x; }

	friend Matrix operator-(const T x, const Matrix<T> &mat)
	{ return mat-x; }

	friend Matrix operator*(const T x, const Matrix<T> &mat)
	{ return mat*x; }

	friend Matrix operator/(const T x, const Matrix<T> &mat)
	{ return mat/x; }

	friend ostream &operator<<(ostream &os, const Matrix<T> &mat)
	{ return os << mat.to_string(); }
};

/**
 * @brief Construct a matrix with interest on its whole region.
 * @param channel the number of channels
 * @param row the number of rows
 * @param col the number of columns
 * @param data the pointer pointing to the original data
*/
template <typename T>
Matrix<T>::Matrix(const size_t channel, const size_t row, const size_t col, const T *data):
	channel(channel), row(row), col(col), entry(new T[channel*row*col], default_delete<T[]>()),
	roi(0), r(row), c(col)
{ if (data)  memcpy(entry.get(), data, channel*row*col*sizeof(T)); }

/**
 * @brief Soft copy construct with an exist matrix.
 * @param mat the source matrix
*/
template <typename T>
Matrix<T>::Matrix(const Matrix<T> &mat): channel(mat.channel), row(mat.row), col(mat.col),
	roi(mat.roi), r(mat.r), c(mat.c), entry(mat.entry) {}

/**
 * @brief Construct a sub matrix from an exist matrix
 * @param mat the source matrix
 * @param hr the head row of the roi
 * @param hc the head column of the roi
 * @param r the rows of the roi
 * @param c the columns of the roi
*/
template <typename T>
Matrix<T>::Matrix(const Matrix<T> &mat, size_t hr, size_t hc, size_t r, size_t c):
	channel(mat.channel), row(mat.row), col(mat.col), entry(mat.entry),
	r(r), c(c), roi(mat.roi+hr*col+hc) {}

template <typename T>
T Matrix<T>::get(const size_t k, const size_t i, const size_t j) const
{
	if (i >= r || j >= c)
	{
		cerr << "\nError: Out of Range in " << __func__ << endl;
		return 0;
	}
	return entry.get()[k*row*col+roi+i*col+j];
}

template <typename T>
string Matrix<T>::to_string() const
{
    string s("\n");
    T *p = entry.get();
    for (size_t k = 0, area = row*col; k < channel; ++k)
    {
        for (size_t i = 0, head_i = k*area; i < row; ++i)
        {
            for (size_t j = 0, head_j = head_i+i*col; j < col; ++j)
                s += std::to_string(p[head_j+j])+" ";
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
	Matrix<T> res(channel, r, c, nullptr);
	T *dest = res.entry.get(), *src = entry.get();
	for (size_t k = 0, len = c*sizeof(T); k < channel; ++k)
	for (size_t i = 0, krc = k*r*c, krcr = k*row*col+roi; i < r; ++i)
		memcpy(dest+krc+i*c, src+krcr+i*col, len);
	*this = res;
}

template <typename T>
bool Matrix<T>::operator==(const Matrix<T> &mat) const
{
	if (r != mat.r || c != mat.c)  return false;
	T *p[2] = {entry.get(), mat.entry.get()};
	if (p[0] == p[1] && roi == mat.roi)  return true;
	for (size_t k = 0, area[2] = {row*col, mat.row*mat.col}; k < channel; ++k)
	for (size_t i = 0, head_i[2] = {k*area[0]+roi, k*area[1]+mat.roi}; i < r; ++i)
	for (size_t j = 0, head_j[2] = {head_i[0]+i*col, head_i[1]+i*mat.col}; j < c; ++j)
		if (p[0][head_j[0]+j] != p[1][head_j[1]+j])  return false;
	return true;
}

template <typename T>
Matrix<T> Matrix<T>::operator+(const Matrix<T> &mat) const
{
	Matrix<T> res(*this);
	res.uniquify();
	T *dest = res.entry.get(), *src = mat.entry.get();
	for (size_t k = 0, area[2] = {mat.row*mat.col, r*c}; k < channel; ++k)
	for (size_t i = 0, head_i[2] = {k*area[0]+mat.roi, k*area[1]}; i < r; ++i)
	for (size_t j = 0, head_j[2] = {head_i[0]+i*mat.col, head_i[1]+i*c}; j < c; ++j)
		dest[head_j[1]+j] += src[head_j[0]+j];
	return res;
}

template <typename T>
Matrix<T> Matrix<T>::operator-(const Matrix<T> &mat) const
{
	Matrix<T> res(*this);
	res.uniquify();
	T *dest = res.entry.get(), *src = mat.entry.get();
	for (size_t k = 0, area[2] = {mat.row*mat.col, r*c}; k < channel; ++k)
	for (size_t i = 0, head_i[2] = {k*area[0]+mat.roi, k*area[1]}; i < r; ++i)
	for (size_t j = 0, head_j[2] = {head_i[0]+i*mat.col, head_i[1]+i*c}; j < c; ++j)
		dest[head_j[1]+j] -= src[head_j[0]+j];
	return res;
}

template <typename T>
Matrix<T> Matrix<T>::operator*(const Matrix<T> &mat) const
{
	Matrix<T> res(channel, r, mat.c, nullptr);
	T *dest = res.entry.get(), *src[2] = {entry.get(), mat.entry.get()};
	bzero(dest, channel*res.row*res.col*sizeof(T));
	for (size_t t = 0, area[3] = {row*col, mat.row*mat.col, res.row*res.col}; t < channel; ++t)
	for (size_t i = 0, head_i[3] = {t*area[0]+roi, t*area[1]+mat.roi, t*area[2]}; i < r; ++i)
	for (size_t k = 0, head0 = head_i[0]+i*col, head2 = head_i[2]+i*res.col; k < c; ++k)
	for (size_t j = 0, head1 = head_i[1]+k*mat.col, src0 = src[0][head0+k]; j < mat.c; ++j)
		dest[head2+j] += src0*src[1][head1+j];
	return res;
}

template <typename T>
Matrix<T> Matrix<T>::operator+(const T x) const
{
	Matrix<T> res(*this);
	res.uniquify();
	T *p = res.entry.get();
	for (size_t i = 0, siz = channel*r*c; i < siz; ++i)
		p[i] += x;
	return res;
}

template <typename T>
Matrix<T> Matrix<T>::operator-(const T x) const
{
	Matrix<T> res(*this);
	res.uniquify();
	T *p = res.entry.get();
	for (size_t i = 0, siz = channel*r*c; i < siz; ++i)
		p[i] -= x;
	return res;
}

template <typename T>
Matrix<T> Matrix<T>::operator*(const T x) const
{
	Matrix<T> res(*this);
	res.uniquify();
	T *p = res.entry.get();
	for (size_t i = 0, siz = channel*r*c; i < siz; ++i)
		p[i] *= x;
	return res;
}

template <typename T>
Matrix<T> Matrix<T>::operator/(const T x) const
{
	Matrix<T> res(*this);
	res.uniquify();
	T *p = res.entry.get();
	for (size_t i = 0, siz = channel*r*c; i < siz; ++i)
		p[i] /= x;
	return res;
}

template <typename T>
Matrix<T> &Matrix<T>::operator+=(const Matrix<T> &mat)
{
	uniquify();
	T *dest = entry.get(), *src = mat.entry.get();
	for (size_t k = 0, area[2] = {mat.row*mat.col, row*col}; k < channel; ++k)
	for (size_t i = 0, head_i[2] = {k*area[0]+mat.roi, k*area[1]+roi}; i < r; ++i)
	for (size_t j = 0, head_j[2] = {head_i[0]+i*mat.col, head_i[1]+i*col}; j < c; ++j)
		dest[head_j[1]+j] += src[head_j[0]+j];
	return *this;
}

template <typename T>
Matrix<T> &Matrix<T>::operator-=(const Matrix<T> &mat)
{
	uniquify();
	T *dest = entry.get(), *src = mat.entry.get();
	for (size_t k = 0, area[2] = {mat.row*mat.col, row*col}; k < channel; ++k)
	for (size_t i = 0, head_i[2] = {k*area[0]+mat.roi, k*area[1]+roi}; i < r; ++i)
	for (size_t j = 0, head_j[2] = {head_i[0]+i*mat.col, head_i[1]+i*col}; j < c; ++j)
		dest[head_j[1]+j] -= src[head_j[0]+j];
	return *this;
}

template <typename T>
Matrix<T> &Matrix<T>::operator*=(const Matrix<T> &mat)
{ return this = (*this)*mat; }

template <typename T>
Matrix<T> &Matrix<T>::operator=(const T x)
{
	uniquify();
	T *p = entry.get();
	for (size_t k = 0, area = row*col; k < channel; ++k)
	for (size_t i = 0, head_i = k*area+roi; i < r; ++i)
	for (size_t j = 0, head_j = head_i+i*col; j < c; ++j)
		dest[head_j+j] = x;
}

template <typename T>
Matrix<T> &Matrix<T>::operator+=(const T x)
{
	uniquify();
	T *p = entry.get();
	for (size_t k = 0, area = row*col; k < channel; ++k)
	for (size_t i = 0, head_i = k*area+roi; i < r; ++i)
	for (size_t j = 0, head_j = head_i+i*col; j < c; ++j)
		dest[head_j+j] += x;
}

template <typename T>
Matrix<T> &Matrix<T>::operator-=(const T x)
{
	uniquify();
	T *p = entry.get();
	for (size_t k = 0, area = row*col; k < channel; ++k)
	for (size_t i = 0, head_i = k*area+roi; i < r; ++i)
	for (size_t j = 0, head_j = head_i+i*col; j < c; ++j)
		dest[head_j+j] -= x;
}

template <typename T>
Matrix<T> &Matrix<T>::operator*=(const T x)
{
	uniquify();
	T *p = entry.get();
	for (size_t k = 0, area = row*col; k < channel; ++k)
	for (size_t i = 0, head_i = k*area+roi; i < r; ++i)
	for (size_t j = 0, head_j = head_i+i*col; j < c; ++j)
		dest[head_j+j] *= x;
}

template <typename T>
Matrix<T> &Matrix<T>::operator/=(const T x)
{
	uniquify();
	T *p = entry.get();
	for (size_t k = 0, area = row*col; k < channel; ++k)
	for (size_t i = 0, head_i = k*area+roi; i < r; ++i)
	for (size_t j = 0, head_j = head_i+i*col; j < c; ++j)
		dest[head_j+j] /= x;
}