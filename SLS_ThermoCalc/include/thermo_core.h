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
    double mean_molecular_weight;               /* 平均分子量 M_wm (g/mol) */
    double gas_molecular_weight;                /* 气相平均分子量 M_wg (g/mol) */
    double density;                             /* 密度 (kg/m³) */
    double cp;                                  /* 平衡定压比热 c_p (J/(kg·K)) */
    double cv;                                  /* 平衡定容比热 c_v (J/(kg·K)) */
    double cp_frozen;                           /* 冻结定压比热 c_pf (J/(kg·K)) */
    double cv_frozen;                           /* 冻结定容比热 c_vf (J/(kg·K)) */
    double gamma;                               /* 平衡比热比 γ */
    double gamma_frozen;                        /* 冻结比热比 γ_f */
    double gamma_s;                             /* 等熵指数 γ_s */
    double sound_speed;                         /* 平衡声速 a_s (m/s) */
    double sound_speed_frozen;                  /* 冻结声速 a_sf (m/s) */
    double char_velocity;                       /* 特征速度 c* (m/s) */
    double R_specific;                          /* 等价气体常数 R_m (J/(kg·K)) */
    double R_gas;                               /* 气相气体常数 R_g (J/(kg·K)) */
    double viscosity;                           /* 粘性系数 μ (Pa·s) */
    double conductivity;                        /* 导热系数 λ (W/(m·K)) */
    double prandtl;                             /* 普朗特数 Pr */
    double condensed_mass_frac;                 /* 凝相质量分数 Y_c */
    double gas_mass_frac;                       /* 气相质量分数 Y_g */
    double dlnv_dlnT_p;                         /* (∂ln v/∂ln T)_p */
    double dlnv_dlnp_T;                         /* (∂ln v/∂ln p)_T */
    int converged;                              /* 是否收敛 */
} ChamberResult;

/**
 * @brief 喷管计算结果结构体
 */
typedef struct {
    double exit_temperature;                    /* 出口温度 (K) */
    double exit_pressure;                       /* 出口压强 (atm) */
    double mole_fractions[MAX_SPECIES];         /* 出口组分 */
    double exit_velocity;                       /* 出口速度 u_e (m/s) */
    double specific_impulse;                    /* 比冲 I_sp (s) */
    double specific_impulse_vac;                /* 真空比冲 I_sp,vac (s) */
    double thrust_coefficient;                  /* 推力系数 C_F */
    double mass_flow_coefficient;               /* 质量流量系数 Γ */
    double area_ratio;                          /* 面积比 ε */
    double mach_number;                         /* 马赫数 Ma */
    double mean_gamma;                          /* 平均等熵指数 γ̄ (公式69) */
    double exit_density;                        /* 出口密度 ρ_e (kg/m³) */
    double exit_sound_speed;                    /* 出口声速 a_e (m/s) */
    double throat_temperature;                  /* 喉部温度 T_t (K) */
    double throat_pressure;                     /* 喉部压强 p_t (atm) */
    double throat_velocity;                     /* 喉部速度 u_t (m/s) */
    double pressure_ratio;                      /* 压强比 p_e/p_c */
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
