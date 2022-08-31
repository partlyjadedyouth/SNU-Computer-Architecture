#include <chrono>
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <x86intrin.h>
#include "function.hpp"

using namespace std;

#ifdef __unix__
  #pragma GCC push_options
  #pragma GCC optimize ("O0")
#elif _WIN64
  #pragma optimize ("",off)
#endif

#define UNROLL 4
// SIMD
void SIMDriver(int n, double* A, double* B, double* C) {
    for (uint16_t i = 0; i < n; i += UNROLL * 4) {
        for (uint16_t j = 0; j < n; j++) {
            __m256d c[4];
            for (uint8_t x = 0; x < UNROLL; x++) {
                c[x] = _mm256_load_pd(C + i + x * 4 + j * n);
            }
            for (uint16_t k = 0; k < n; k++){
                __m256d b = _mm256_broadcast_sd(B + k + j * n);
                for(uint8_t x = 0; x < UNROLL; x++){
                    c[x] = _mm256_add_pd(c[x], _mm256_mul_pd(_mm256_load_pd(A + n * k + x * 4 + i), b));
                }
            }
            for(uint8_t x = 0; x < UNROLL; x++){
                _mm256_store_pd(C + i + x * 4 + j * n, c[x]);
            }
        }
    }
}

void MatrixMultiplication64(vector<vector<int>> &A, vector<vector<int>> &B,
                            vector<vector<int>> &C) {
    int n = 64;
    double * newA = new double[n * n];
    double * newB = new double[n * n];
    double * newC = new double[n * n];

    for (int j = 0; j < n; j++){
        for (int i = 0; i < n; i++){
            newA[j * n + i] = (double)A[i][j];
            newB[j * n + i] = (double)B[i][j];
            newC[j * n + i] = 0;
        }
    }

    SIMDriver(n, newA, newB, newC);

    for(int j = 0; j < n; j++){
        for(int i = 0; i < n; i++){
            C[i][j] = newC[j * n + i];
        }
    }
}

void MatrixMultiplication128(vector<vector<int>> &A, vector<vector<int>> &B,
                             vector<vector<int>> &C) {
    int n = 128;
    double * newA = new double[n * n];
    double * newB = new double[n * n];
    double * newC = new double[n * n];

    for (int j = 0; j < n; j++){
        for (int i = 0; i < n; i++){
            newA[j * n + i] = (double)A[i][j];
            newB[j * n + i] = (double)B[i][j];
            newC[j * n + i] = 0;
        }
    }

    SIMDriver(n, newA, newB, newC);

    for(int j = 0; j < n; j++){
        for(int i = 0; i < n; i++){
            C[i][j] = newC[j * n + i];
        }
    }
}

void MatrixMultiplication1024(vector<vector<int>> &A,
                              vector<vector<int>> &B,
                              vector<vector<int>> &C) {
    int n = 1024;
    double * newA = new double[n * n];
    double * newB = new double[n * n];
    double * newC = new double[n * n];

    for (int j = 0; j < n; j++){
        for (int i = 0; i < n; i++){
            newA[j * n + i] = (double)A[i][j];
            newB[j * n + i] = (double)B[i][j];
            newC[j * n + i] = 0;
        }
    }

    SIMDriver(n, newA, newB, newC);

    for(int j = 0; j < n; j++){
        for(int i = 0; i < n; i++){
            C[i][j] = newC[j * n + i];
        }
    }
}

void MatrixMultiplication2048(vector<vector<int>> &A,
                              vector<vector<int>> &B,
                              vector<vector<int>> &C) {
    int n = 2048;
    double * newA = new double[n * n];
    double * newB = new double[n * n];
    double * newC = new double[n * n];

    for (int j = 0; j < n; j++){
        for (int i = 0; i < n; i++){
            newA[j * n + i] = (double)A[i][j];
            newB[j * n + i] = (double)B[i][j];
            newC[j * n + i] = 0;
        }
    }

    SIMDriver(n, newA, newB, newC);

    for(int j = 0; j < n; j++){
        for(int i = 0; i < n; i++){
            C[i][j] = newC[j * n + i];
        }
    }
}

#ifdef __unix__
  #pragma GCC pop_options
#elif _WIN64
  #pragma optimize ("",on)
#endif
