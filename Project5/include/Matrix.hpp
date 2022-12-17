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

	static void set_zero(T *p, size_t siz);

public:
	Matrix(const size_t channel, const size_t row, const size_t col, const T *entry):
		channel(channel), row(row), col(col), entry(new T[row*col*channel], default_delete<T[]>())
	{ if (entry)  hard_copy(this->entry.get(), entry, row*col*channel); }

	Matrix(const Matrix &mat): row(mat.row), col(mat.col), entry(mat.entry) {}

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

	template <typename t>
	friend Matrix operator+(const t &x, const Matrix<t> &mat);

	template <typename t>
	friend Matrix operator-(const t &x, const Matrix<t> &mat);

	template <typename t>
	friend Matrix operator*(const t &x, const Matrix<t> &mat);

	template <typename t>
	friend Matrix operator/(const t &x, const Matrix<t> &mat);

	template <typename t>
	friend ostream &operator<<(ostream &os, const Matrix<t> mat);
};