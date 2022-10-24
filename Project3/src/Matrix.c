#include "Matrix.h"
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <stdio.h>

/**
 * @brief Create a matrix with initialization
 * 
 * @param row The number of rows
 * @param col The number of columns
 * @param entry A pointer points to the array of entries
 * 
 * @return A pointer point to the created matrix
*/
inline Matrix *createMatrix(const size_t row, const size_t col, const entry_t *const entry) {
    if (!row || !col) {
        puts("Error in createMatrix(): Invalid dimentions!");
        return NULL;
    }
    if (!entry) {
        puts("Error in createMatrix(): Invalid entry array pointer!");
        return NULL;
    }
    Matrix *mat = (Matrix *)malloc(sizeof(Matrix)); // convert the pointer type ny force in support of C++
    mat->row = row;
    mat->col = col;
    size_t size = row*col;
    mat->entry = (entry_t *)malloc(sizeof(entry_t)*size);
    for (size_t i = 0; i < size; ++i)
        mat->entry[i] = entry[i];
    return mat;
}

/**
 * @brief Delete a matrix
 * 
 * @param mat The pointer points the matrix to be deleted
*/
inline void deleteMatrix(Matrix *const mat) {
    if (!mat) {
        puts("Error in deleteMatrix(): Invalid matrix pointer!");
        return;
    }
    free(mat->entry);
    free(mat);
}

/**
 * @brief Copy data from a matrix to another
 * 
 * @param a A pointer point to the DIR matrix
 * @param b A pointer point to the SRC matrix
*/
inline void copyMatrix(Matrix *const a, const Matrix *const b) {
    if (!a || !b) {
        puts("Error in copyMatrix(): Invalid matrix pointers!");
        return;
    }
    a->row = b->row;
    a->col = b->col;
    size_t size = b->row*b->col;
    a->entry = realloc(a->entry, size); // equivalent to free() first and then malloc()
    memcpy(a->entry, b->entry, sizeof(entry_t)*size);
}

/**
 * @brief Matrix addition
 * 
 * @param a A pointer point to the augend matrix
 * @param b A pointer point to the addend matrix
 * 
 * @returns A pointer point to the result of the addition
*/
inline Matrix *addMatrix(const Matrix *const a, const Matrix *const b) {
    if (!a || !b) {
        puts("Error in addMatrix(): Invalid matrix pointers!");
        return NULL;
    }
    if (a->row != b->row || a->col != b->col) {
        puts("Error in addMatrix(): Inequal dementions!");
        return NULL;
    }
    size_t size = a->row*a->col;
    entry_t entry[size];
    for (size_t i = 0; i < size; ++i)
        entry[i] = a->entry[i]+b->entry[i];
    return createMatrix(a->row, a->col, entry);
}

/**
 * @brief Matrix substraction 
 * 
 * @param a A pointer point to the subtrahend matrix
 * @param b A pointer point to the subtractor matrix
 * 
 * @returns A pointer point to the result of the substraction
*/
inline Matrix *substractMatrix(const Matrix *const a, const Matrix *const b) {
    if (!a || !b) {
        puts("Error in substractMatrix(): Invalid matrix pointers!");
        return NULL;
    }
    if (a->row != b->row || a->col != b->col) {
        puts("Error in substractMatrix(): Inequal dementions!");
        return NULL;
    }
    size_t size = a->row*a->col;
    entry_t entry[size];
    for (size_t i = 0; i < size; ++i)
        entry[i] = a->entry[i]-b->entry[i];
    return createMatrix(a->row, a->col, entry);
}

/**
 * @brief Matrix multiplication 
 * 
 * @param a A pointer point to the multiplicand matrix
 * @param b A pointer point to the multiplier matrix
 * 
 * @returns A pointer point to the result of the multiplication
*/
inline Matrix *multiplyMatrix(const Matrix *const a, const Matrix *const b) {
    if (!a || !b) {
        puts("Error in multiplyMatrix(): Invalid matrix pointers!");
        return NULL;
    }
    if (a->row != b->col || a->col != b->row) {
        puts("Error in multiplyMatrix(): Unpaired dementions!");
        return NULL;
    }
    size_t size = a->row*b->col;
    entry_t entry[size];
    for (size_t i = 0; i < a->row; ++i)
        for (size_t j = 0; j < b->col; ++j) {
            entry[i*b->col+j] = 0.0;
            for (size_t k = 0; k < a->col; ++k)
                entry[i*b->col+j] += a->entry[i*a->col+k]*b->entry[k*b->col+j];
        }
    return createMatrix(a->row, b->col, entry);
}

/**
 * @brief Add a scalar to a matrix 
 * 
 * @param mat A pointer point to the augend matrix
 * @param x The addend scalar
*/
inline void addScalar(const Matrix *mat, const entry_t x) {
    if (!mat) {
        puts("Error in addScalar(): Invalid matrix pointer!");
        return;
    }
    for (size_t i = 0; i < mat->row*mat->col; ++i)
        mat->entry[i] += x;
}

/**
 * @brief Substract a scalar from a matrix 
 * 
 * @param mat A pointer point to the subtrahend matrix
 * @param x The substractor scalar
*/
inline void substractScalar(const Matrix *mat, const entry_t x) {
    if (!mat) {
        puts("Error in substractScalar(): Invalid matrix pointer!");
        return;
    }
    for (size_t i = 0; i < mat->row*mat->col; ++i)
        mat->entry[i] -= x;
}

/**
 * @brief Multiply a matrix and a scalar
 * 
 * @param mat A pointer point to the multiplicand matrix
 * @param x The multiplier scalar
*/
inline void multiplyScalar(const Matrix *mat, const entry_t x) {
    if (!mat) {
        puts("Error in multiplyScalar(): Invalid matrix pointer!");
        return;
    }
    for (size_t i = 0; i < mat->row*mat->col; ++i)
        mat->entry[i] *= x;
}

/**
 * @brief Divide a matrix by a scalar
 * 
 * @param mat A pointer point to the dividend matrix
 * @param x The divisor scalar
*/
inline void divideScalar(const Matrix *mat, const entry_t x) {
    if (!mat) {
        puts("Error in multiplyScalar(): Invalid matrix pointer!");
        return;
    }
    for (size_t i = 0; i < mat->row*mat->col; ++i)
        mat->entry[i] /= x;
}

// A help function to find the minimum of two entry elements
inline entry_t min_entry(entry_t a, entry_t b) {
    return a < b ? a : b;
}

// A help function to find the maximum of two entry elements
inline entry_t max_entry(entry_t a, entry_t b) {
    return a > b ? a : b;
}

/**
 * @brief Find the minimal entry of a matrix
 * 
 * @param mat The target matrix
 * 
 * @returns The minimal entry of the matrix
*/
inline entry_t minEntry(const Matrix *const mat) {
    if (!mat) {
        puts("Error in minEntry(): Invalid matrix pointer!");
        return NAN;
    }
    entry_t result = mat->entry[0];
    for (size_t i = 1; i < mat->row*mat->col; ++i)
        result = min_entry(result, mat->entry[i]);
    return result;
}

/**
 * @brief Find the maximum entry of a matrix
 * 
 * @param mat The target matrix
 * 
 * @returns The maximum entry of the matrix
*/
inline entry_t maxEntry(const Matrix *const mat) {
    if (!mat) {
        puts("Error in maxEntry(): Invalid matrix pointer!");
        return NAN;
    }
    entry_t result = mat->entry[0];
    for (size_t i = 1; i < mat->row*mat->col; ++i)
        result = max_entry(result, mat->entry[i]);
    return result;
}

/**
 * @brief Calculate the trace of a matrix
 * 
 * @param mat The target matrix
 * 
 * @return Trace of the matrix
*/
inline entry_t trace(const Matrix *const mat) {
    if (!mat) {
        puts("Error in trace(): Invalid matrix pointer!");
        return NAN;
    }
    if (mat->row != mat->col) {
        puts("Error in trace(): Invalid matrix dementions!");
        return NAN;
    }
    entry_t result = 0.0;
    for (size_t i = 0; i < mat->row; ++i)
        result += mat->entry[i*mat->col+i];
    return result;
}

/**
 * @brief Calculate the rank of a matrix
 * 
 * @param mat The target matrix
 * 
 * @return Rank of the matrix
*/
inline entry_t rank(const Matrix *const mat) {

}

/**
 * @brief Print a matrix by set percision
 * 
 * @param mat The target matrix
*/
inline void print(const Matrix *const mat) {
    if (!mat) {
        puts("Error in print(): Invalid matrix pointer!");
        return;
    }
    printf("row = %ld, col = %ld\n", mat->row, mat->col);
    for (size_t i = 0; i < mat->row; ++i) {
        for (size_t j = 0; j < mat->col; ++j)
            printf(entry_place_holder, mat->entry[i*mat->col+j]);
        putchar('\n');
    }
}
