#ifndef __MATRIX_H__
#define __MATRIX_H__

typedef float entry_t;

typedef struct {
    size_t row, col;
    entry_t *entry;
} Matrix;

// Create an empty matrix
Matrix *createMatrix(const size_t row, const size_t col, const entry_t *const entry);

// Delete a matrix
void deleteMatrix(Matrix *const mat);

// copy data from b to a
void copyMatrix(Matrix *const a, const Matrix *const b);

// Add two matrices
Matrix *addMatrix(const Matrix *const a, const Matrix *const b);

// Substraction of to matrices
Matrix *substractMatrix(const Matrix *const a, const Matrix *const b);

// Multiply two matrices
void multiplyMatrix(const Matrix *const a, const Matrix *const b);

// Add a scalar to a matrix
void addScalar();

// Subtract a scalar from a matrix
void substractScalar();

// Multiply a matrix with a scalar
void multiplyScalar();

// Find the minimal value of a matrix
entry_t minEntry();

// Find the maximal value of a matrix
entry_t maxEntry();

void print(const Matrix *const mat);

#endif // __MATRIX_H__