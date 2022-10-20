#ifndef __MATRIX_C__
#define __MATRIX_C__

#include "Matrix.h"

Matrix createMatrix(const size_t col, const size_t row);

void deleteMatrix();

void copyMatrix();

void addMatrix();

void substractMatrix();

void multiplyMatrix();

void addScalar();

void substractScalar();

void multiplyScalar();

double minEntry();

double maxEntry();

#endif // __MATRIX_C__