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
	size_t channel, row, col, roi, r, c;
	shared_ptr<T> entry;

public:
	Matrix(const size_t channel, const size_t row, const size_t col, const T *entry);

	Matrix(const Matrix &mat);

	T get(size_t k, size_t i, size_t j) const;

	string to_string() const;

	void print() const;

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
Matrix<T>::Matrix(const size_t channel, const size_t row, const size_t col, const T *entry):
	channel(channel), row(row), col(col), r(row), c(col), entry(new T[channel*row*col], default_delete<T[]>())
{
	T *p = this->entry.get();
	if (entry)  memcpy(p, entry, channel*row*col*sizeof(T));
	roi = 0;
}

template <typename T>
Matrix<T>::Matrix(const Matrix<T> &mat): channel(mat.channel), row(mat.row), col(mat.col),
	roi(mat.roi), r(mat.r), c(mat.c), entry(mat.entry) {}

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