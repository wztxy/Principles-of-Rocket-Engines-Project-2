/**
 * @file constants.h
 * @brief 物理常数和全局定义
 * @note SLS火箭发动机热力计算程序
 */

#ifndef CONSTANTS_H
#define CONSTANTS_H

#ifdef __cplusplus
extern "C" {
#endif

/* 物理常数 */
#define R_UNIVERSAL     8.314462618 /* 通用气体常数 J/(mol·K) - CODATA 2018 */
#define ATM_TO_PA       101325.0    /* 1 atm = 101325 Pa */
#define T_REF           298.15      /* 标准参考温度 K */

/* 计算限制 */
#define MAX_ELEMENTS    10          /* 最大元素数 */
#define MAX_SPECIES     50          /* 最大组分数 */
#define MAX_PROPELLANTS 10          /* 最大推进剂组元数 */

/* NASA 9系数多项式格式 (来自 NASA CEA thermo.inp)
 * Cp/R = a1*T^(-2) + a2*T^(-1) + a3 + a4*T + a5*T^2 + a6*T^3 + a7*T^4
 * H/RT = -a1*T^(-2) + a2*ln(T)/T + a3 + a4*T/2 + a5*T^2/3 + a6*T^3/4 + a7*T^4/5 + b1/T
 * S/R  = -a1*T^(-2)/2 - a2*T^(-1) + a3*ln(T) + a4*T + a5*T^2/2 + a6*T^3/3 + a7*T^4/4 + b2
 * 
 * 系数存储: a1-a7 (7个多项式系数) + b1, b2 (积分常数)
 * 共9个系数，每个温度区间独立
 */
#define NASA9_COEFF_NUM     9       /* NASA 9系数个数 (a1-a7 + b1 + b2) */
#define NASA9_TEMP_RANGES   3       /* 温度区间数: 200-1000, 1000-6000, 6000-20000K */

/* 兼容性：保留旧宏名 */
#define NASA_COEFF_NUM  (NASA9_COEFF_NUM * NASA9_TEMP_RANGES)  /* 总系数数 = 27 */

/* 温度区间边界 (K) */
#define T_LOW_MIN       200.0       /* 低温区下限 */
#define T_LOW_MAX       1000.0      /* 低温区上限 / 中温区下限 */
#define T_MID_MAX       6000.0      /* 中温区上限 / 高温区下限 */
#define T_HIGH_MAX      20000.0     /* 高温区上限 */

/* 迭代参数 */
#define MAX_ITERATIONS  100         /* 最大迭代次数 */
#define CONVERGENCE_TOL 1e-8        /* 收敛容差 */
#define MIN_MOLE_FRAC   1e-12       /* 最小摩尔分数 */

/* 温度扫描参数 */
#define DEFAULT_T_MIN   1000.0      /* 默认最低温度 K */
#define DEFAULT_T_MAX   4500.0      /* 默认最高温度 K */
#define DEFAULT_T_STEP  50.0        /* 默认温度步长 K */

#ifdef __cplusplus
}
#endif

#endif /* CONSTANTS_H */
