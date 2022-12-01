#include <immintrin.h>
#include <stdio.h>
#include <pthread.h>
#include <time.h>

#define MATRIX_SIZE 8192
#define NUM_THREAD 4

float matA[MATRIX_SIZE][MATRIX_SIZE];
float matB[MATRIX_SIZE][MATRIX_SIZE];
float matC[MATRIX_SIZE][MATRIX_SIZE];
int step = 0;

void *multiplicationAVX(void *args) {
    __m256 vecA, vecB, vecC;
    int thread = step++;
    for (int i = thread * MATRIX_SIZE / NUM_THREAD;
         i < (thread + 1) * MATRIX_SIZE / NUM_THREAD; i++) {
        for (int j = 0; j < MATRIX_SIZE; j++) {
            matC[i][j] = 0.0f;
        }
        for (int j = 0; j < MATRIX_SIZE; j++) {
            vecA = _mm256_set1_ps(matA[i][j]);
            for (int k = 0; k < MATRIX_SIZE; k += 8) {
                vecB = _mm256_loadu_ps(&matB[j][k]);
                vecC = _mm256_loadu_ps(&matC[i][k]);
                vecC = _mm256_fmadd_ps(vecA, vecB, vecC);
                _mm256_storeu_ps(&matC[i][k], vecC);
            }
        }
    }
    return 0;
}

void *multiplicationNormal(void *args) {
    int thread = step++;
    for (int i = thread * MATRIX_SIZE / NUM_THREAD;
         i < (thread + 1) * MATRIX_SIZE / NUM_THREAD; i++) {
        for (int j = 0; j < MATRIX_SIZE; j++) {
            for (int k = 0; k < MATRIX_SIZE; k++) {
                matC[i][j] += matA[i][k] * matB[k][j];
            }
        }
    }
    return 0;
}

void createMatrix() {
    for (int i = 0; i < MATRIX_SIZE; i++) {
        for (int j = 0; j < MATRIX_SIZE; j++) {
            matA[i][j] = i + j * 2;
            matB[i][j] = i * 2 + j;
            matC[i][j] = 0.0f;
        }
    }
}
void printMatrix() {
    if (MATRIX_SIZE <= 16) {
        printf("Matriz A");
        for (int i = 0; i < MATRIX_SIZE; i++) {
            printf("\n");
            for (int j = 0; j < MATRIX_SIZE; j++) {
                printf("%f ", matA[i][j]);
            }
        }
        printf("\n\n");
        printf("Matriz B");
        for (int i = 0; i < MATRIX_SIZE; i++) {
            printf("\n");
            for (int j = 0; j < MATRIX_SIZE; j++) {
                printf("%f ", matB[i][j]);
            }
        }
        printf("\n\n");
        printf("Multiplying matrix A with B");
        for (int i = 0; i < MATRIX_SIZE; i++) {
            printf("\n");
            for (int j = 0; j < MATRIX_SIZE; j++) {
                printf("%f ", matC[i][j]);
            }
        }
    }
}
int main() {
    pthread_t threads[NUM_THREAD];
    clock_t start, end;
    createMatrix();
    start = clock();
    for (int i = 0; i < NUM_THREAD; i++) {
        // pthread_create(&threads[i], NULL, multiplicationNormal, NULL);
        pthread_create(&threads[i], NULL, multiplicationAVX, NULL);
    }
    for (int i = 0; i < NUM_THREAD; i++) {
        pthread_join(threads[i], NULL);
    }
    end = clock();
    printMatrix();
    printf("\n\n使用的线程数 -> %d\n", NUM_THREAD);
    printf("\n矩阵大小 -> %d\n", MATRIX_SIZE);
    printf("\n程序运行时间（毫秒） -> %f\n\n", (float)(end - start) * 1000 / CLOCKS_PER_SEC);
}