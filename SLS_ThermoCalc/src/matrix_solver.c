/**
 * @file matrix_solver.c
 * @brief 矩阵求解器实现 - 高斯消元法（列主元）
 */

#include "matrix_solver.h"
#include <math.h>
#include <string.h>

/**
 * @brief 高斯消元法求解线性方程组 Ax = b
 */
int solve_gaussian(int n, double A[MAX_SPECIES][MAX_SPECIES], 
                   double b[MAX_SPECIES], double x[MAX_SPECIES])
{
    double aug[MAX_SPECIES][MAX_SPECIES + 1];
    int i, j, k;
    int max_row;
    double max_val, factor, temp;
    
    /* 构造增广矩阵 [A|b] */
    for (i = 0; i < n; i++) {
        for (j = 0; j < n; j++) {
            aug[i][j] = A[i][j];
        }
        aug[i][n] = b[i];
    }
    
    /* 前向消元 */
    for (i = 0; i < n; i++) {
        /* 列主元选择 */
        max_row = i;
        max_val = fabs(aug[i][i]);
        for (k = i + 1; k < n; k++) {
            if (fabs(aug[k][i]) > max_val) {
                max_val = fabs(aug[k][i]);
                max_row = k;
            }
        }
        
        /* 交换行 */
        if (max_row != i) {
            for (j = i; j <= n; j++) {
                temp = aug[i][j];
                aug[i][j] = aug[max_row][j];
                aug[max_row][j] = temp;
            }
        }
        
        /* 检查主元是否为零（矩阵奇异） */
        if (fabs(aug[i][i]) < 1e-14) {
            return -1;  /* 矩阵奇异 */
        }
        
        /* 消元 */
        for (k = i + 1; k < n; k++) {
            factor = aug[k][i] / aug[i][i];
            for (j = i; j <= n; j++) {
                aug[k][j] -= factor * aug[i][j];
            }
        }
    }
    
    /* 回代求解 */
    for (i = n - 1; i >= 0; i--) {
        x[i] = aug[i][n];
        for (j = i + 1; j < n; j++) {
            x[i] -= aug[i][j] * x[j];
        }
        x[i] /= aug[i][i];
    }
    
    return 0;
}

/**
 * @brief 矩阵向量乘法 y = A * x
 */
void matrix_vector_mult(int m, int n, 
                        double A[MAX_SPECIES][MAX_SPECIES],
                        double x[MAX_SPECIES], 
                        double y[MAX_SPECIES])
{
    int i, j;
    for (i = 0; i < m; i++) {
        y[i] = 0.0;
        for (j = 0; j < n; j++) {
            y[i] += A[i][j] * x[j];
        }
    }
}
