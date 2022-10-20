#ifndef __MATRIX_C__
#define __MATRIX_C__

#include "Matrix.h"

Matrix createMatrix(const size_t col, const size_t row);

inline void deleteMatrix();

inline void copyMatrix();

inline void addMatrix();

inline void substractMatrix();

inline void multiplyMatrix();

inline void addScalar();

inline void substractScalar();

inline void multiplyScalar();

inline double minEntry();

inline double maxEntry();

#endif // __MATRIX_C__