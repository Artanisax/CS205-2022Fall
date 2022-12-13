#pragma once

#include <cstddef>
#include <memory>

using std::shared_ptr;
using std::make_shared;

template<typename T>
class Matrix
{
private:
	size_t row, col;
	shared_ptr<T> entry;

public:
	Matrix(size_t row, size_t col, T *entry): row(row), col(col)
	{	entry = make_shared(entry); }

	Matrix(Matrix mat): row(mat.row), col(mat.col), entry(mat.entry) {}

	Matrix operator=(Matrix &mat);

	bool operator==(Matrix &mat);

	Matrix operator+(Matrix &mat);

	Matrix operator-(Matrix &mat);

	Matrix operator*(Matrix &mat);

	Matrix operator+(T &x);

	Matrix operator-(T &x);

	Matrix operator*(T &x);

	Matrix operator/(T &x);

	friend Matrix operator+(Matrix &mat, T x);

	friend Matrix operator-(Matrix &mat, T x);

	friend Matrix operator*(Matrix &mat, T x);

	friend Matrix operator/(Matrix &mat, T x);
};