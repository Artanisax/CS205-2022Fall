#pragma once

#include <cstddef>
#include <memory>
#include <string>
#include <cstring>
#include <iostream>
// #include <omp.h>

using std::shared_ptr;
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
	size_t channel = 0, row = 0, col = 0;  // Three dimensions of the original matrix
	size_t roi = 0;                        // The starting index of the the region of interest
	size_t r = 0, c = 0;                   // Two dimensions of the interesting matrix
	shared_ptr<T> entry;                   // A shared pointer managing the original data
	// Attention: shared_ptr destruct containing object by calling default_delete<T>().
	// So we need to costomize its deleter into default_delete<T[]>().


public:
	Matrix();

	Matrix(const size_t channel, const size_t row, const size_t col, const T *data);

	// No need for defualt copy constructor can achieve soft copy by using shared_ptr.
	// Matrix(const Matrix &mat);

	Matrix(const Matrix &mat, size_t hr, size_t hc, size_t r, size_t c);

	T get(const size_t k, const size_t i, const size_t j) const;

	T set(const size_t k, const size_t i, const size_t j, const T x);

	string to_string() const;

	void print() const { cout << to_string(); }

	void uniquify();
	
	static bool dimension_match(const Matrix &a, const Matrix &b)
	{ return a.channel == b.channel && a.r == b.r && a.c == b.c; }

	bool operator==(const Matrix &mat) const;

	Matrix operator+(const Matrix &mat) const;

	Matrix operator-(const Matrix &mat) const;

	Matrix operator*(const Matrix &mat) const;

	Matrix operator+(const T x) const;

	Matrix operator-(const T x) const;

	Matrix operator*(const T x) const;

	Matrix operator/(const T x) const;
	
	// No need for defualt assignment can achieve soft copy by using shared_ptr.
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

// Construct an empty Matrix, usually used in exception handling
template <typename T>
Matrix<T>::Matrix(): entry(new T[0], default_delete<T[]>()) {}

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
	r(row), c(col)
{ if (data)  memcpy(entry.get(), data, channel*row*col*sizeof(T)); }

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
	r(r), c(c), roi(mat.roi+hr*col+hc)
{
	if (roi/col+r > row || roi%col+c > col)
	{
		cerr << "Error: Out of Range in " << __func__ << endl;
		*this = Matrix();
	}
}

/**
 * @brief Get the value about corresponding indexes.
 * @param k channel
 * @param i row
 * @param j column
*/
template <typename T>
T Matrix<T>::get(const size_t k, const size_t i, const size_t j) const
{
	if (k >= channel || i >= r || j >= c)
	{
		cerr << "Error: Out of Range in " << __func__ << endl;
		return 0;
	}
	return entry.get()[k*row*col+roi+i*col+j];
}

/**
 * @brief Set a value about corresponding indexes.
 * @param k channel
 * @param i row
 * @param j column
*/
template <typename T>
T Matrix<T>::set(const size_t k, const size_t i, const size_t j, const T x)
{
	if (k >= channel || i >= r || j >= c)
	{ cerr << "Error: Out of Range in " << __func__ << endl; }
	entry.get()[k*row*col+roi+i*col+j] = x;
}

// Covert Matrix into string.
template <typename T>
string Matrix<T>::to_string() const
{
    string s("\n");
    T *p = entry.get();
    for (size_t k = 0, area = row*col; k < channel; ++k)
    {
        for (size_t i = 0, head_i = k*area+roi; i < r; ++i)
        {
            for (size_t j = 0, head_j = head_i+i*col; j < c; ++j)
                s += std::to_string(p[head_j+j])+" ";
            s += "\n";
        }
        s += "\n";
    }
    return s;
}

// Use hard copy to refactor a unique Matrix with the same content.
// If its entry pointer is not shared, then this will do nothing.
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
	if (!dimension_match(*this, mat))  return false;
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
	if (!dimension_match(*this, mat))
	{
		cerr << "\nError: Dimension Mismatch in " << __func__ << endl;
		return Matrix();
	}
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
	if (!dimension_match(*this, mat))
	{
		cerr << "\nError: Dimension Mismatch in " << __func__ << endl;
		return Matrix();
	}
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
	if (channel != mat.channel|| c != mat.r)
	{
		cerr << "\nError: Dimension Mismatch in " << __func__ << endl;
		return Matrix();
	}
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
	if (!dimension_match(*this, mat))
	{
		cerr << "\nError: Dimension Mismatch in " << __func__ << endl;
		return *this = Matrix();
	}
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
	if (!dimension_match(*this, mat))
	{
		cerr << "\nError: Dimension Mismatch in " << __func__ << endl;
		return *this = Matrix();
	}
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
{
	if (channel != mat.channel|| c != mat.r)
	{
		cerr << "\nError: Dimension Mismatch in " << __func__ << endl;
		return *this = Matrix();
	}
	return this = (*this)*mat;
}

template <typename T>
Matrix<T> &Matrix<T>::operator=(const T x)
{
	uniquify();
	T *p = entry.get();
	for (size_t k = 0, area = row*col; k < channel; ++k)
	for (size_t i = 0, head_i = k*area+roi; i < r; ++i)
	for (size_t j = 0, head_j = head_i+i*col; j < c; ++j)
		p[head_j+j] = x;
	return *this;
}

template <typename T>
Matrix<T> &Matrix<T>::operator+=(const T x)
{
	uniquify();
	T *p = entry.get();
	for (size_t k = 0, area = row*col; k < channel; ++k)
	for (size_t i = 0, head_i = k*area+roi; i < r; ++i)
	for (size_t j = 0, head_j = head_i+i*col; j < c; ++j)
		p[head_j+j] += x;
	return *this;
}

template <typename T>
Matrix<T> &Matrix<T>::operator-=(const T x)
{
	uniquify();
	T *p = entry.get();
	for (size_t k = 0, area = row*col; k < channel; ++k)
	for (size_t i = 0, head_i = k*area+roi; i < r; ++i)
	for (size_t j = 0, head_j = head_i+i*col; j < c; ++j)
		p[head_j+j] -= x;
	return *this;
}

template <typename T>
Matrix<T> &Matrix<T>::operator*=(const T x)
{
	uniquify();
	T *p = entry.get();
	for (size_t k = 0, area = row*col; k < channel; ++k)
	for (size_t i = 0, head_i = k*area+roi; i < r; ++i)
	for (size_t j = 0, head_j = head_i+i*col; j < c; ++j)
		p[head_j+j] *= x;
	return *this;
}

template <typename T>
Matrix<T> &Matrix<T>::operator/=(const T x)
{
	uniquify();
	T *p = entry.get();
	for (size_t k = 0, area = row*col; k < channel; ++k)
	for (size_t i = 0, head_i = k*area+roi; i < r; ++i)
	for (size_t j = 0, head_j = head_i+i*col; j < c; ++j)
		p[head_j+j] /= x;
	return *this;
}