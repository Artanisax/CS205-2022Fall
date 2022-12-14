#pragma once

#include <cstddef>
#include <memory>
#include <string>
#include <iostream>

using std::shared_ptr;
using std::make_shared;
using std::default_delete;
using std::to_string;
using std::string;
using std::ostream;

template<typename T>
class Matrix
{
private:
	size_t row, col;
	shared_ptr<T> entry;

	static void copy(T *dst, T src, size_t siz)
	{
		for (size_t i = 0; i < siz; ++i) dst[i] = src[i];
	}

public:
	Matrix(const size_t row, const size_t col, const T *entry): row(row), col(col)
	{
		size_t siz = row*col;
		entry = make_shared<T>(new T[siz]);
		copy();
	}

	Matrix(const Matrix &mat): row(mat.row), col(mat.col), entry(mat.entry) {}

	string to_string() const
	{
		string s;
		T *p = entry.get();
		for (size_t i = 0; i < row; ++i)
		{
			for (size_t j = 0, head = i*col; j < col; ++j)
			{
				// s += to_string(p[head+j])+' ';
				std::cout << p[head+j];
			}
			std::cout << '\n';
			// s += '\n';
		}
		return s;
	}

	// Matrix operator=(const Matrix &mat);

	// bool operator==(const Matrix &mat) const;

	// Matrix operator+(const Matrix &mat) const;

	// Matrix operator-(const Matrix &mat) const;

	// Matrix operator*(const Matrix &mat) const;

	// Matrix operator+(const T &x) const;

	// Matrix operator-(const T &x) const;

	// Matrix operator*(const T &x) const;

	// Matrix operator/(const T &x) const;

	// Matrix operator+=(const T &x);

	// Matrix operator-=(const T &x);

	// Matrix operator*=(const T &x);

	// Matrix operator/=(const T &x);

	// friend Matrix operator+(const Matrix &mat, T x);

	// friend Matrix operator-(const Matrix &mat, T x);

	// friend Matrix operator*(const Matrix &mat, T x);

	// friend Matrix operator/(const Matrix &mat, T x);

	// friend ostream &operator<<(ostream &os, const Matrix mat);
};