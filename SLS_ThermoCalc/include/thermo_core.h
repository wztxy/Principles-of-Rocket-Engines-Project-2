/**
 * @file thermo_core.h
 * @brief 热力计算核心模块 - 最小吉布斯自由能法
 * @note 基于热力计算流程及公式.tex中的公式实现
 *       使用 NASA 9系数多项式格式 (NASA CEA thermo.inp)
 */

#ifndef THERMO_CORE_H
#define THERMO_CORE_H

#ifdef __cplusplus
extern "C" {
#endif

#include "constants.h"

/**
 * @brief NASA 9系数结构体 - 单个温度区间
 * 系数顺序: a1, a2, a3, a4, a5, a6, a7, b1, b2
 */
typedef struct {
    double T_min;                   /* 温度区间下限 (K) */
    double T_max;                   /* 温度区间上限 (K) */
    double a[NASA9_COEFF_NUM];      /* NASA 9系数: a1-a7, b1, b2 */
} NASA9Interval;

/**
 * @brief NASA 9系数完整数据结构 - 所有温度区间
 */
typedef struct {
    int num_intervals;                          /* 温度区间数 (通常为2或3) */
    NASA9Interval intervals[NASA9_TEMP_RANGES]; /* 温度区间数据 */
} NASA9Coefficients;

/**
 * @brief 推进剂输入参数结构体
 */
typedef struct {
    int num_propellants;                        /* 推进剂组元数 P */
    int num_elements;                           /* 元素数 M */
    int num_species;                            /* 平衡组分数 N */
    int num_condensed;                          /* 凝相组分数 L */
    
    double St_aij[MAX_PROPELLANTS][MAX_ELEMENTS]; /* 组元-元素原子数矩阵 */
    double mass_fraction[MAX_PROPELLANTS];        /* 组元质量分数 */
    double element_weight[MAX_ELEMENTS];          /* 元素原子量 */
    
    /* NASA 9系数热力数据 */
    NASA9Coefficients nasa9[MAX_SPECIES];         /* NASA 9系数数组 */
    
    double Aij[MAX_ELEMENTS][MAX_SPECIES];        /* 产物-元素原子数矩阵 */
    
    double chamber_pressure;                    /* 燃烧室压强 (atm) */
    double initial_enthalpy;                    /* 推进剂初始焓 (J/kg) */
    double initial_temperature;                 /* 推进剂初温 (K) */
} PropellantInput;

/**
 * @brief 燃烧室计算结果结构体
 */
typedef struct {
    double temperature;                         /* 燃烧温度 (K) */
    double mole_fractions[MAX_SPECIES];         /* 平衡组分摩尔数 */
    double total_enthalpy;                      /* 总焓 (J/kg) */
    double total_entropy;                       /* 总熵 (J/(kg·K)) */
    double mean_molecular_weight;               /* 平均分子量 (g/mol) */
    double density;                             /* 密度 (kg/m³) */
    double cp;                                  /* 定压比热 (J/(kg·K)) */
    double cv;                                  /* 定容比热 (J/(kg·K)) */
    double gamma;                               /* 比热比 */
    double gamma_s;                             /* 等熵指数 */
    double sound_speed;                         /* 声速 (m/s) */
    double char_velocity;                       /* 特征速度 (m/s) */
    int converged;                              /* 是否收敛 */
} ChamberResult;

/**
 * @brief 喷管计算结果结构体
 */
typedef struct {
    double exit_temperature;                    /* 出口温度 (K) */
    double exit_pressure;                       /* 出口压强 (atm) */
    double mole_fractions[MAX_SPECIES];         /* 出口组分 */
    double exit_velocity;                       /* 出口速度 (m/s) */
    double specific_impulse;                    /* 比冲 (s) */
    double thrust_coefficient;                  /* 推力系数 */
    double mass_flow_rate;                      /* 质量流量系数 */
    double area_ratio;                          /* 面积比 */
    double mach_number;                         /* 马赫数 */
    int converged;                              /* 是否收敛 */
} NozzleResult;

/**
 * @brief 完整计算结果结构体
 */
typedef struct {
    ChamberResult chamber;
    NozzleResult nozzle;
    char error_msg[256];
    int success;
} ThermoResult;

/* ============ 假定化学式计算 ============ */

/**
 * @brief 计算推进剂假定化学式 N_k
 * @param input 推进剂输入参数
 * @param N_k 输出元素摩尔数数组
 */
void calc_assumed_formula(const PropellantInput* input, double N_k[MAX_ELEMENTS]);

/* ============ NASA 9系数多项式热力参数 ============ */

/**
 * @brief 获取指定温度对应的 NASA 9 系数区间
 * @param coeff NASA 9 系数结构体
 * @param T 温度 (K)
 * @return 对应区间指针，NULL表示温度超出范围
 */
const NASA9Interval* get_nasa9_interval(const NASA9Coefficients* coeff, double T);

/**
 * @brief 计算标准摩尔焓 H°(T) (J/mol) - NASA 9格式
 * H/RT = -a1*T^(-2) + a2*ln(T)/T + a3 + a4*T/2 + a5*T²/3 + a6*T³/4 + a7*T⁴/5 + b1/T
 */
double calc_enthalpy(const NASA9Coefficients* coeff, double T);

/**
 * @brief 计算标准摩尔熵 S°(T) (J/(mol·K)) - NASA 9格式
 * S/R = -a1*T^(-2)/2 - a2*T^(-1) + a3*ln(T) + a4*T + a5*T²/2 + a6*T³/3 + a7*T⁴/4 + b2
 */
double calc_entropy(const NASA9Coefficients* coeff, double T);

/**
 * @brief 计算摩尔定压比热 Cp(T) (J/(mol·K)) - NASA 9格式
 * Cp/R = a1*T^(-2) + a2*T^(-1) + a3 + a4*T + a5*T² + a6*T³ + a7*T⁴
 */
double calc_cp(const NASA9Coefficients* coeff, double T);

/**
 * @brief 计算化学势 μ_i (J/mol)
 */
double calc_chemical_potential(const NASA9Coefficients* coeff, 
                               double T, double p, int is_condensed);

/* ============ 最小吉布斯自由能法 ============ */

/**
 * @brief 最小吉布斯自由能法求解平衡组分
 * @param input 推进剂输入参数
 * @param T 温度 (K)
 * @param p 压强 (atm)
 * @param N_k 元素摩尔数
 * @param c_init 组分初值
 * @param c_result 输出平衡组分
 * @return 迭代次数，-1表示不收敛
 */
int min_gibbs_solve(const PropellantInput* input, double T, double p,
                    const double N_k[MAX_ELEMENTS],
                    const double c_init[MAX_SPECIES],
                    double c_result[MAX_SPECIES]);

/* ============ 燃烧室计算 ============ */

/**
 * @brief 燃烧室热力计算主函数
 * @param input 推进剂输入参数
 * @param T_min 最低试算温度
 * @param T_max 最高试算温度
 * @param T_step 温度步长
 * @param c_init 组分初值
 * @param result 输出计算结果
 * @return 0 成功，-1 失败
 */
int combustion_chamber_calc(const PropellantInput* input,
                            double T_min, double T_max, double T_step,
                            const double c_init[MAX_SPECIES],
                            ChamberResult* result);

/* ============ 喷管计算 ============ */

/**
 * @brief 给定出口压强的喷管计算（平衡流动）
 */
int nozzle_calc_from_pressure(const PropellantInput* input,
                              const ChamberResult* chamber,
                              double exit_pressure,
                              const double c_init[MAX_SPECIES],
                              NozzleResult* result);

/**
 * @brief 给定出口温度的喷管计算（冻结流动）
 */
int nozzle_calc_from_temperature(const PropellantInput* input,
                                 const ChamberResult* chamber,
                                 double exit_temperature,
                                 NozzleResult* result);

/**
 * @brief 给定面积比的喷管计算
 */
int nozzle_calc_from_area_ratio(const PropellantInput* input,
                                const ChamberResult* chamber,
                                double area_ratio,
                                const double c_init[MAX_SPECIES],
                                NozzleResult* result);

/* ============ 热力参数计算 ============ */

/**
 * @brief 计算燃烧产物热力参数
 */
void calc_thermo_properties(const PropellantInput* input,
                            double T, double p,
                            const double moles[MAX_SPECIES],
                            ChamberResult* result);

/**
 * @brief 计算偏微分 (∂ln n_j/∂ln T)_p 和 (∂ln n_g/∂ln T)_p
 */
void calc_partial_derivatives_T(const PropellantInput* input,
                                double T, double p,
                                const double moles[MAX_SPECIES],
                                double dlnn_dlnT[MAX_SPECIES],
                                double* dlnng_dlnT);

/**
 * @brief 计算偏微分 (∂ln n_j/∂ln p)_T 和 (∂ln n_g/∂ln p)_T
 */
void calc_partial_derivatives_p(const PropellantInput* input,
                                double T, double p,
                                const double moles[MAX_SPECIES],
                                double dlnn_dlnp[MAX_SPECIES],
                                double* dlnng_dlnp);

/* ============ 综合计算接口 ============ */

/**
 * @brief 完整热力计算主函数
 * @param input 推进剂输入参数
 * @param exit_pressure 喷管出口压强 (atm)，若<=0则不计算喷管
 * @param result 输出完整结果
 * @return 0 成功，-1 失败
 */
int thermo_calculate(const PropellantInput* input,
                     double exit_pressure,
                     ThermoResult* result);

#ifdef __cplusplus
}
#endif

#endif /* THERMO_CORE_H */
