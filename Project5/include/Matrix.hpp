#pragma once

#include <cstddef>
#include <memory>
#include <vector>
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

public:
	Matrix(const size_t channel, const size_t row, const size_t col, const T *entry):
		channel(channel), row(row), col(col), entry(new T[row*col*channel], default_delete<T[]>())
	{ hard_copy(this->entry.get(), entry, row*col*channel); }

	Matrix(const Matrix &mat): row(mat.row), col(mat.col), entry(mat.entry) {}

	string to_string() const;

	bool operator==(const Matrix &mat) const;

	Matrix operator=(const Matrix &mat);

	Matrix operator+(const Matrix &mat) const;

	Matrix operator-(const Matrix &mat) const;

	Matrix operator*(const Matrix &mat) const;

	Matrix operator+(const T &x) const;

	Matrix operator-(const T &x) const;

	Matrix operator*(const T &x) const;

	Matrix operator/(const T &x) const;

	Matrix operator+=(const T &x);

	Matrix operator-=(const T &x);

	Matrix operator*=(const T &x);

	Matrix operator/=(const T &x);

	friend Matrix operator+(const T &x, const Matrix &mat);

	friend Matrix operator-(const T &x, const Matrix &mat);

	friend Matrix operator*(const T &x, const Matrix &mat);

	friend Matrix operator/(const T &x, const Matrix &mat);

	friend ostream &operator<<(ostream &os, const Matrix mat);
};