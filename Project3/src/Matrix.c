#include "Matrix.h"
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <stdio.h>

// Build a matrix pointer list globally
MatrixPointer MPL_head; // The head of the matrix pointers list

// push a matrix into MPL
void MPL_push_front(const Matrix *const mat) {
    MatrixPointer *mp = (MatrixPointer *)malloc(sizeof(MatrixPointer));
    mp->mat = mat;
    mp->next = MPL_head.next;
    MPL_head.next = mp;
}

// delete a matrix from MPL
void MPL_erase(const Matrix *const mat) {
    MatrixPointer *prev = &MPL_head, *mp;
    while (prev->next->mat != mat)
        prev = prev->next; // find the previous pointer in the list
    mp = prev->next;
    prev->next = mp->next;
    free(mp);
}

/**
 * @brief Check the validity of a Matrix pointer
 * @param mat The pointer needed to be checked
 * @return True for valid, false for invalid
*/
bool check_matrix_pointer(const Matrix *const mat) {
    if (!mat) return false; // empty pointer
    for (MatrixPointer *cur = &MPL_head; cur; cur = cur->next)
        if (cur->mat == mat) return true; // in MPL, valid
    return false; // not in MPL, invalid
}

/**
 * @brief Create a matrix with initialization
 * @param row The number of rows
 * @param col The number of columns
 * @param entry A pointer points to the array of entries
 * @return A pointer point to the created matrix
*/
Matrix *createMatrix(const size_t row, const size_t col, const entry_t *const entry) {
    if (!row || !col) {
        puts("Error in createMatrix(): Invalid dimentions!");
        return NULL;
    }
    if (!entry) {
        puts("Error in createMatrix(): Invalid entry array pointer!");
        return NULL;
    }
    Matrix *mat = (Matrix *)malloc(sizeof(Matrix));
    mat->row = row;
    mat->col = col;
    size_t siz = row*col;
    mat->entry = (entry_t *)malloc(sizeof(entry_t)*siz);
    for (size_t i = 0; i < siz; ++i)
        mat->entry[i] = entry[i];
    MPL_push_front(mat);
    return mat;
}


/**
 * @brief Create an identity matrix
 * @param n The size of the matrix
 * @return A pointer point to the created matrix
*/
Matrix *createIMatrix(const size_t n) {
    size_t siz = n*n;
    entry_t entry[siz];
    memset(entry, 0, sizeof(entry));
    for (size_t i = 0; i < n; ++i)
        entry[i*i] = 1;
    return createMatrix(n, n, entry);
}

/**
 * @brief Delete a matrix
 * @param mat The pointer points the matrix to be deleted
*/
void deleteMatrix(Matrix *const mat) {
    if (!check_matrix_pointer(mat)) {
        puts("Error in deleteMatrix(): Invalid matrix pointer!");
        return;
    }
    MPL_erase(mat);
    free(mat->entry);
    free(mat);
}

/**
 * @brief Copy data from a matrix to another
 * @param a A pointer point to the DST matrix
 * @param b A pointer point to the SRC matrix
*/
void copyMatrix(Matrix *const a, const Matrix *const b) {
    if (!check_matrix_pointer(a) || !check_matrix_pointer(b)) {
        puts("Error in copyMatrix(): Invalid matrix pointers!");
        return;
    }
    a->row = b->row;
    a->col = b->col;
    size_t siz = b->row*b->col;
    a->entry = (entry_t *)realloc(a->entry, siz); // equivalent to free() first and then malloc()
    memcpy(a->entry, b->entry, sizeof(entry_t)*siz);
}

/**
 * @brief Matrix addition
 * @param a A pointer point to the augend matrix
 * @param b A pointer point to the addend matrix
 * @returns A pointer point to the result of the addition
*/
Matrix *addMatrix(const Matrix *const a, const Matrix *const b) {
    if (!check_matrix_pointer(a) || !check_matrix_pointer(b)) {
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
 * @param a A pointer point to the subtrahend matrix
 * @param b A pointer point to the subtractor matrix
 * @returns A pointer point to the result of the substraction
*/
Matrix *substractMatrix(const Matrix *const a, const Matrix *const b) {
    if (!check_matrix_pointer(a) || !check_matrix_pointer(b)) {
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
 * @param a A pointer point to the multiplicand matrix
 * @param b A pointer point to the multiplier matrix
 * @returns A pointer point to the result of the multiplication
*/
Matrix *multiplyMatrix(const Matrix *const a, const Matrix *const b) {
    if (!check_matrix_pointer(a) || !check_matrix_pointer(b)) {
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
 * @param mat A pointer point to the augend matrix
 * @param x The addend scalar
*/
void addScalar(const Matrix *mat, const entry_t x) {
    if (!check_matrix_pointer(mat)) {
        puts("Error in addScalar(): Invalid matrix pointer!");
        return;
    }
    for (size_t i = 0; i < mat->row*mat->col; ++i)
        mat->entry[i] += x;
}

/**
 * @brief Substract a scalar from a matrix
 * @param mat A pointer point to the subtrahend matrix
 * @param x The substractor scalar
*/
void substractScalar(const Matrix *mat, const entry_t x) {
    if (!check_matrix_pointer(mat)) {
        puts("Error in substractScalar(): Invalid matrix pointer!");
        return;
    }
    for (size_t i = 0; i < mat->row*mat->col; ++i)
        mat->entry[i] -= x;
}

/**
 * @brief Multiply a matrix and a scalar
 * @param mat A pointer point to the multiplicand matrix
 * @param x The multiplier scalar
*/
void multiplyScalar(const Matrix *mat, const entry_t x) {
    if (!check_matrix_pointer(mat)) {
        puts("Error in multiplyScalar(): Invalid matrix pointer!");
        return;
    }
    for (size_t i = 0; i < mat->row*mat->col; ++i)
        mat->entry[i] *= x;
}

/**
 * @brief Divide a matrix by a scalar
 * @param mat A pointer point to the dividend matrix
 * @param x The divisor scalar
*/
void divideScalar(const Matrix *mat, const entry_t x) {
    if (!check_matrix_pointer(mat)) {
        puts("Error in multiplyScalar(): Invalid matrix pointer!");
        return;
    }
    for (size_t i = 0; i < mat->row*mat->col; ++i)
        mat->entry[i] /= x;
}

/**
 * @brief Find the minimal entry of a matrix
 * @param mat The target matrix
 * @returns The minimal entry of the matrix
*/
entry_t minEntry(const Matrix *const mat) {
    if (!check_matrix_pointer(mat)) {
        puts("Error in minEntry(): Invalid matrix pointer!");
        return NAN;
    }
    entry_t result = mat->entry[0];
    for (size_t i = 1; i < mat->row*mat->col; ++i)
        result = (result <= mat->entry[i] ? result : mat->entry[i]);
    return result;
}

/**
 * @brief Find the maximum entry of a matrix
 * @param mat The target matrix
 * @returns The maximum entry of the matrix
*/
entry_t maxEntry(const Matrix *const mat) {
    if (!check_matrix_pointer(mat)) {
        puts("Error in maxEntry(): Invalid matrix pointer!");
        return NAN;
    }
    entry_t result = mat->entry[0];
    for (size_t i = 1; i < mat->row*mat->col; ++i)
        result = (result >= mat->entry[i] ? result : mat->entry[i]);
    return result;
}

/**
 * @brief Calculate the trace of a matrix
 * @param mat The target matrix
 * @return Trace of the matrix
*/
entry_t trace(const Matrix *const mat) {
    if (!check_matrix_pointer(mat)) {
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
 * @brief Calculate the determinant of a matrix
 * @param mat The target matrix
 * @return Determinant of the matrix
*/
entry_t det(const Matrix*const mat) {
    if (!check_matrix_pointer(mat)) {
        puts("Error in det(): Invalid matrix pointer!");
        return NAN;
    }
    if (mat->row != mat->col) {
        puts("Error in det(): Invalid matrix dementions!");
        return NAN;
    }
    size_t row = mat->row, col = mat->col, siz = (mat->row-1)*(mat->col-1);
    entry_t *entry = mat->entry, temp[siz];
    switch (row) {
        case 1: return entry[0];
        case 2: return entry[0]*entry[3]-entry[1]*entry[2];
        default: // expand the first column of the matrix
            entry_t ret = 0;
            for (size_t k = 0; k < row; ++k) {
                size_t idx = 0;
                for (size_t i = 0; i < row; ++i) {
                    if (i == k) continue;
                    for (size_t j = 1; j < col; ++j)
                        temp[idx++] = entry[i*col+j];
                }
                Matrix *sub = createMatrix(row-1, col-1, temp);
                ret += (k&1 ? -entry[k*col]*det(sub) : entry[k*col]*det(sub));
                deleteMatrix(sub);
            }
            return ret;
    }
}

/**
 * @brief Print a matrix by set percision
 * @param mat The target matrix
*/
void printMatrix(const Matrix *const mat) {
    if (!check_matrix_pointer(mat)) {
        puts("Error in printMatrix(): Invalid matrix pointer!");
        return;
    }
    printf("row = %zu, col = %zu\n", mat->row, mat->col);
    for (size_t i = 0; i < mat->row; ++i) {
        for (size_t j = 0; j < mat->col; ++j)
            printf(entry_place_holder, mat->entry[i*mat->col+j]);
        putchar('\n');
    }
}
