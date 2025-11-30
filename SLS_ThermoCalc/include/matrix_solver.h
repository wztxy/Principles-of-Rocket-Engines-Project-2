/**
 * @file matrix_solver.h
 * @brief 矩阵求解器 - 高斯消元法
 */

#ifndef MATRIX_SOLVER_H
#define MATRIX_SOLVER_H

#ifdef __cplusplus
extern "C" {
#endif

#include "constants.h"

/**
 * @brief 高斯消元法求解线性方程组 Ax = b
 * @param n 矩阵维度
 * @param A 系数矩阵 (n x n)，会被修改
 * @param b 右端向量 (n)，会被修改
 * @param x 解向量 (n)
 * @return 0 成功，-1 矩阵奇异
 */
int solve_gaussian(int n, double A[MAX_SPECIES][MAX_SPECIES], 
                   double b[MAX_SPECIES], double x[MAX_SPECIES]);

/**
 * @brief 矩阵向量乘法 y = A * x
 */
void matrix_vector_mult(int m, int n, 
                        double A[MAX_SPECIES][MAX_SPECIES],
                        double x[MAX_SPECIES], 
                        double y[MAX_SPECIES]);

#ifdef __cplusplus
}
#endif

#endif /* MATRIX_SOLVER_H */
