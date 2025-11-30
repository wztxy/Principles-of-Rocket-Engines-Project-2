/**
 * @file thermo_core.c
 * @brief 热力计算核心模块实现 - 最小吉布斯自由能法
 * @note 基于公式、报告/热力计算流程及公式.tex
 *       使用 NASA 9系数多项式格式 (NASA CEA thermo.inp)
 */

#include "thermo_core.h"
#include "matrix_solver.h"
#include <math.h>
#include <string.h>
#include <stdio.h>

/* ============ 假定化学式计算 ============ */

void calc_assumed_formula(const PropellantInput* input, double N_k[MAX_ELEMENTS])
{
    int i, j, k;
    double M_i[MAX_PROPELLANTS];  /* 各组元分子量 */
    double B_ij[MAX_PROPELLANTS][MAX_ELEMENTS];  /* 假定化学式系数 */
    
    /* 计算各组元分子量: M_i = Σ(A_wk * St_ik) */
    for (i = 0; i < input->num_propellants; i++) {
        M_i[i] = 0.0;
        for (k = 0; k < input->num_elements; k++) {
            M_i[i] += input->element_weight[k] * input->St_aij[i][k];
        }
        if (M_i[i] < 1e-12) M_i[i] = 1e-12;
    }
    
    /* 计算假定化学式系数: B_ij = St_ij * 1000 / M_i */
    for (i = 0; i < input->num_propellants; i++) {
        for (j = 0; j < input->num_elements; j++) {
            B_ij[i][j] = input->St_aij[i][j] * 1000.0 / M_i[i];
        }
    }
    
    /* 计算推进剂假定化学式: N_k = Σ(g_i * B_ik) */
    for (k = 0; k < input->num_elements; k++) {
        N_k[k] = 0.0;
        for (i = 0; i < input->num_propellants; i++) {
            N_k[k] += input->mass_fraction[i] * B_ij[i][k];
        }
    }
}

/* ============ NASA 9系数多项式热力参数 ============ */
/* 
 * NASA 9系数格式 (来自 NASA CEA thermo.inp):
 * Cp/R = a1*T^(-2) + a2*T^(-1) + a3 + a4*T + a5*T^2 + a6*T^3 + a7*T^4
 * H/RT = -a1*T^(-2) + a2*ln(T)/T + a3 + a4*T/2 + a5*T^2/3 + a6*T^3/4 + a7*T^4/5 + b1/T
 * S/R = -a1*T^(-2)/2 - a2*T^(-1) + a3*ln(T) + a4*T + a5*T^2/2 + a6*T^3/3 + a7*T^4/4 + b2
 * 
 * 系数索引: a[0]=a1, a[1]=a2, ..., a[6]=a7, a[7]=b1, a[8]=b2
 */

const NASA9Interval* get_nasa9_interval(const NASA9Coefficients* coeff, double T)
{
    int i;
    for (i = 0; i < coeff->num_intervals; i++) {
        if (T >= coeff->intervals[i].T_min && T <= coeff->intervals[i].T_max) {
            return &coeff->intervals[i];
        }
    }
    /* 温度超出范围，返回最接近的区间 */
    if (T < coeff->intervals[0].T_min) {
        return &coeff->intervals[0];
    }
    return &coeff->intervals[coeff->num_intervals - 1];
}

double calc_enthalpy(const NASA9Coefficients* coeff, double T)
{
    const NASA9Interval* interval = get_nasa9_interval(coeff, T);
    const double* a = interval->a;
    
    double T2 = T * T;
    double T3 = T2 * T;
    double T4 = T3 * T;
    double lnT = log(T);
    
    /* H/RT = -a1*T^(-2) + a2*ln(T)/T + a3 + a4*T/2 + a5*T²/3 + a6*T³/4 + a7*T⁴/5 + b1/T */
    double H_RT = -a[0] / T2 
                + a[1] * lnT / T 
                + a[2] 
                + a[3] * T / 2.0 
                + a[4] * T2 / 3.0 
                + a[5] * T3 / 4.0 
                + a[6] * T4 / 5.0 
                + a[7] / T;
    
    return H_RT * R_UNIVERSAL * T;  /* H (J/mol) */
}

double calc_entropy(const NASA9Coefficients* coeff, double T)
{
    const NASA9Interval* interval = get_nasa9_interval(coeff, T);
    const double* a = interval->a;
    
    double T2 = T * T;
    double T3 = T2 * T;
    double T4 = T3 * T;
    double lnT = log(T);
    
    /* S/R = -a1*T^(-2)/2 - a2*T^(-1) + a3*ln(T) + a4*T + a5*T²/2 + a6*T³/3 + a7*T⁴/4 + b2 */
    double S_R = -a[0] / (2.0 * T2) 
               - a[1] / T 
               + a[2] * lnT 
               + a[3] * T 
               + a[4] * T2 / 2.0 
               + a[5] * T3 / 3.0 
               + a[6] * T4 / 4.0 
               + a[8];
    
    return S_R * R_UNIVERSAL;  /* S (J/(mol·K)) */
}

double calc_cp(const NASA9Coefficients* coeff, double T)
{
    const NASA9Interval* interval = get_nasa9_interval(coeff, T);
    const double* a = interval->a;
    
    double T2 = T * T;
    double T3 = T2 * T;
    double T4 = T3 * T;
    
    /* Cp/R = a1*T^(-2) + a2*T^(-1) + a3 + a4*T + a5*T² + a6*T³ + a7*T⁴ */
    double Cp_R = a[0] / T2 
                + a[1] / T 
                + a[2] 
                + a[3] * T 
                + a[4] * T2 
                + a[5] * T3 
                + a[6] * T4;
    
    return Cp_R * R_UNIVERSAL;  /* Cp (J/(mol·K)) */
}

double calc_chemical_potential(const NASA9Coefficients* coeff, 
                               double T, double p, int is_condensed)
{
    double H = calc_enthalpy(coeff, T);
    double S = calc_entropy(coeff, T);
    double mu = H - T * S;
    
    /* 气相组分需要加上压力项 */
    if (!is_condensed) {
        mu += R_UNIVERSAL * T * log(p);
    }
    
    return mu;
}

/* ============ 最小吉布斯自由能法 ============ */

int min_gibbs_solve(const PropellantInput* input, double T, double p,
                    const double N_k[MAX_ELEMENTS],
                    const double c_init[MAX_SPECIES],
                    double c_result[MAX_SPECIES])
{
    int N = input->num_species;
    int M = input->num_elements;
    int L = input->num_condensed;
    int iter, i, j, k;
    int eq_size = M + 1 + L;
    
    double c[MAX_SPECIES];
    double y[MAX_SPECIES];
    double Phi_c[MAX_SPECIES];
    double R_mat[MAX_ELEMENTS][MAX_ELEMENTS];
    double alpha[MAX_ELEMENTS];
    double A_left[MAX_SPECIES][MAX_SPECIES];
    double B_right[MAX_SPECIES];
    double X_sol[MAX_SPECIES];
    double X_new[MAX_SPECIES];
    double cg, mu;
    
    /* 初始化组分 */
    for (i = 0; i < N; i++) {
        c[i] = c_init[i];
        if (c[i] < MIN_MOLE_FRAC) c[i] = MIN_MOLE_FRAC;
    }
    
    /* 迭代求解 */
    for (iter = 0; iter < MAX_ITERATIONS; iter++) {
        /* 计算气相总摩尔数 */
        cg = 0.0;
        for (i = L; i < N; i++) {
            cg += c[i];
        }
        if (cg < MIN_MOLE_FRAC) cg = MIN_MOLE_FRAC;
        
        /* 计算 y_i = -μ_i / (R*T) */
        for (i = 0; i < N; i++) {
            mu = calc_chemical_potential(&input->nasa9[i], T, p, (i < L));
            y[i] = -mu / (R_UNIVERSAL * T);
        }
        
        /* 计算 Φ_c[i] = (-y_i + ln(c_i) + ln(p) - ln(cg)) * c_i */
        for (i = 0; i < N; i++) {
            if (i < L) {
                /* 凝相 */
                Phi_c[i] = 0.0;
            } else {
                /* 气相 */
                Phi_c[i] = (-y[i] + log(c[i]) + log(p) - log(cg)) * c[i];
            }
        }
        
        /* 计算 R 矩阵: R[k][l] = Σ(A[k][j] * A[l][j] * c[j]) */
        for (k = 0; k < M; k++) {
            for (i = 0; i < M; i++) {
                R_mat[k][i] = 0.0;
                for (j = L; j < N; j++) {
                    R_mat[k][i] += input->Aij[k][j] * input->Aij[i][j] * c[j];
                }
            }
        }
        
        /* 计算 alpha: alpha[k] = Σ(A[k][j] * c[j]) */
        for (k = 0; k < M; k++) {
            alpha[k] = 0.0;
            for (j = L; j < N; j++) {
                alpha[k] += input->Aij[k][j] * c[j];
            }
        }
        
        /* 构造系数矩阵 A_left */
        memset(A_left, 0, sizeof(A_left));
        for (i = 0; i < eq_size; i++) {
            for (j = 0; j < eq_size; j++) {
                if (i < M && j < M) {
                    A_left[i][j] = R_mat[i][j];
                } else if (i < M && j == M) {
                    A_left[i][j] = alpha[i];
                } else if (i < M && j > M) {
                    A_left[i][j] = input->Aij[i][j - M - 1];
                } else if (i == M && j < M) {
                    A_left[i][j] = alpha[j];
                } else if (i > M && j < M) {
                    A_left[i][j] = input->Aij[j][i - M - 1];
                } else {
                    A_left[i][j] = 0.0;
                }
            }
        }
        
        /* 构造右端向量 B_right */
        for (i = 0; i < eq_size; i++) {
            if (i < M) {
                B_right[i] = N_k[i];
                for (j = L; j < N; j++) {
                    B_right[i] += input->Aij[i][j] * Phi_c[j];
                }
            } else if (i == M) {
                B_right[i] = 0.0;
                for (j = L; j < N; j++) {
                    B_right[i] += Phi_c[j];
                }
            } else {
                B_right[i] = -y[i - M - 1];
            }
        }
        
        /* 求解线性方程组 */
        if (solve_gaussian(eq_size, A_left, B_right, X_sol) != 0) {
            return -1;  /* 矩阵奇异 */
        }
        
        /* 计算新的组分值 */
        for (i = 0; i < N; i++) {
            if (i < L) {
                /* 凝相 */
                X_new[i] = X_sol[i + M + 1];
            } else {
                /* 气相 */
                X_new[i] = -Phi_c[i] + c[i] * X_sol[M];
                for (k = 0; k < M; k++) {
                    X_new[i] += input->Aij[k][i] * X_sol[k] * c[i];
                }
            }
        }
        
        /* 更新组分，检查收敛 */
        double max_change = 0.0;
        for (i = 0; i < N; i++) {
            if (X_new[i] > MIN_MOLE_FRAC) {
                double change = fabs(X_new[i] - c[i]) / (fabs(c[i]) + MIN_MOLE_FRAC);
                if (change > max_change) max_change = change;
                c[i] = X_new[i];
            } else {
                c[i] = MIN_MOLE_FRAC;
            }
        }
        
        /* 检查收敛 */
        if (max_change < CONVERGENCE_TOL && iter > 5) {
            break;
        }
    }
    
    /* 输出结果 */
    for (i = 0; i < N; i++) {
        c_result[i] = c[i];
    }
    
    return iter;
}

/* ============ 燃烧室计算 ============ */

int combustion_chamber_calc(const PropellantInput* input,
                            double T_min, double T_max, double T_step,
                            const double c_init[MAX_SPECIES],
                            ChamberResult* result)
{
    int N = input->num_species;
    int i, num_points;
    double T, H_total;
    double N_k[MAX_ELEMENTS];
    double c[MAX_SPECIES];
    double c_current[MAX_SPECIES];
    
    /* 存储温度扫描结果 */
    typedef struct {
        double T;
        double H;
        double moles[MAX_SPECIES];
    } ScanPoint;
    ScanPoint points[200];
    
    /* 计算假定化学式 */
    calc_assumed_formula(input, N_k);
    
    /* 初始化组分 */
    for (i = 0; i < N; i++) {
        c[i] = c_init[i];
    }
    
    /* 温度扫描 */
    num_points = 0;
    for (T = T_min; T <= T_max && num_points < 200; T += T_step) {
        /* 求解平衡组分 */
        int iter = min_gibbs_solve(input, T, input->chamber_pressure, 
                                   N_k, c, c_current);
        if (iter < 0) {
            continue;  /* 跳过不收敛的点 */
        }
        
        /* 计算总焓 */
        H_total = 0.0;
        for (i = 0; i < N; i++) {
            H_total += c_current[i] * calc_enthalpy(&input->nasa9[i], T);
        }
        
        /* 保存结果 */
        points[num_points].T = T;
        points[num_points].H = H_total;
        for (i = 0; i < N; i++) {
            points[num_points].moles[i] = c_current[i];
            c[i] = c_current[i];  /* 使用上一点结果作为下一点初值 */
        }
        num_points++;
    }
    
    if (num_points < 2) {
        result->converged = 0;
        return -1;
    }
    
    /* 通过焓守恒插值确定燃烧温度 */
    double H_pre = input->initial_enthalpy;
    int found = 0;
    
    for (i = 1; i < num_points; i++) {
        if ((H_pre >= points[i-1].H && H_pre <= points[i].H) ||
            (H_pre <= points[i-1].H && H_pre >= points[i].H)) {
            /* 线性插值 */
            double ratio = (H_pre - points[i-1].H) / (points[i].H - points[i-1].H);
            result->temperature = points[i-1].T + ratio * (points[i].T - points[i-1].T);
            
            for (int j = 0; j < N; j++) {
                result->mole_fractions[j] = points[i-1].moles[j] + 
                                            ratio * (points[i].moles[j] - points[i-1].moles[j]);
            }
            found = 1;
            break;
        }
    }
    
    if (!found) {
        result->converged = 0;
        return -1;
    }
    
    /* 计算其他热力参数 */
    calc_thermo_properties(input, result->temperature, input->chamber_pressure,
                           result->mole_fractions, result);
    
    result->converged = 1;
    return 0;
}

/* ============ 热力参数计算 ============ */

void calc_thermo_properties(const PropellantInput* input,
                            double T, double p,
                            const double moles[MAX_SPECIES],
                            ChamberResult* result)
{
    int N = input->num_species;
    int M = input->num_elements;
    int L = input->num_condensed;
    int i;
    double ng = 0.0;
    double mass_total = 0.0;
    double mass_gas = 0.0;
    
    /* 计算气相总摩尔数 */
    for (i = L; i < N; i++) {
        ng += moles[i];
    }
    
    /* 计算各组分分子量并求总质量 */
    double M_i[MAX_SPECIES];
    for (i = 0; i < N; i++) {
        M_i[i] = 0.0;
        for (int k = 0; k < M; k++) {
            M_i[i] += input->element_weight[k] * input->Aij[k][i];
        }
        mass_total += moles[i] * M_i[i];
        if (i >= L) {
            mass_gas += moles[i] * M_i[i];
        }
    }
    
    /* 平均分子量 (g/mol) */
    result->mean_molecular_weight = mass_total / (ng > 0 ? ng : 1);
    
    /* 密度 (kg/m³) */
    result->density = (p * ATM_TO_PA) / (ng * R_UNIVERSAL * T);
    
    /* 总焓 */
    result->total_enthalpy = 0.0;
    for (i = 0; i < N; i++) {
        result->total_enthalpy += moles[i] * calc_enthalpy(&input->nasa9[i], T);
    }
    
    /* 总熵 */
    result->total_entropy = 0.0;
    for (i = 0; i < N; i++) {
        result->total_entropy += moles[i] * calc_entropy(&input->nasa9[i], T);
    }
    /* 气相混合熵修正 */
    for (i = L; i < N; i++) {
        if (moles[i] > MIN_MOLE_FRAC && ng > MIN_MOLE_FRAC) {
            result->total_entropy -= R_UNIVERSAL * moles[i] * log(moles[i] / ng);
        }
    }
    result->total_entropy -= R_UNIVERSAL * ng * log(p);
    
    /* 定压比热 (冻结) */
    double cp_frozen = 0.0;
    for (i = 0; i < N; i++) {
        cp_frozen += moles[i] * calc_cp(&input->nasa9[i], T);
    }
    
    /* 计算偏微分用于平衡比热 */
    double dlnn_dlnT[MAX_SPECIES], dlnng_dlnT;
    double dlnn_dlnp[MAX_SPECIES], dlnng_dlnp;
    calc_partial_derivatives_T(input, T, p, moles, dlnn_dlnT, &dlnng_dlnT);
    calc_partial_derivatives_p(input, T, p, moles, dlnn_dlnp, &dlnng_dlnp);
    
    /* 平衡定压比热 */
    double cp_eq = cp_frozen;
    for (i = 0; i < N; i++) {
        double H_i = calc_enthalpy(&input->nasa9[i], T);
        cp_eq += moles[i] * H_i * dlnn_dlnT[i] / T;
    }
    result->cp = cp_eq;
    
    /* 偏导数 */
    double dlnv_dlnT = 1.0 + dlnng_dlnT;
    double dlnv_dlnp = -1.0 + dlnng_dlnp;
    
    /* 定容比热 */
    result->cv = result->cp + ng * R_UNIVERSAL * dlnv_dlnT * dlnv_dlnT / dlnv_dlnp;
    
    /* 比热比 */
    result->gamma = result->cp / result->cv;
    
    /* 等熵指数 */
    result->gamma_s = -result->gamma / dlnv_dlnp;
    
    /* 声速 */
    result->sound_speed = sqrt(result->gamma_s * ng * R_UNIVERSAL * T);
    
    /* 特征速度 */
    double Gamma = sqrt(result->gamma_s * pow(2.0 / (result->gamma_s + 1.0), 
                        (result->gamma_s + 1.0) / (result->gamma_s - 1.0)));
    result->char_velocity = sqrt(T * R_UNIVERSAL / result->mean_molecular_weight * 1000.0) / Gamma;
}

/* ============ 偏微分计算 ============ */

void calc_partial_derivatives_T(const PropellantInput* input,
                                double T, double p,
                                const double moles[MAX_SPECIES],
                                double dlnn_dlnT[MAX_SPECIES],
                                double* dlnng_dlnT)
{
    int N = input->num_species;
    int M = input->num_elements;
    int L = input->num_condensed;
    int eq_size = N + M + 1;
    int i, j;
    
    double ng = 0.0;
    for (i = L; i < N; i++) {
        ng += moles[i];
    }
    
    double A_mat[MAX_SPECIES][MAX_SPECIES];
    double B_vec[MAX_SPECIES];
    double X_sol[MAX_SPECIES];
    
    memset(A_mat, 0, sizeof(A_mat));
    memset(B_vec, 0, sizeof(B_vec));
    
    /* 构造方程组（参考公式28） */
    /* 凝相方程 */
    for (i = 0; i < L; i++) {
        for (j = N; j < N + M; j++) {
            A_mat[i][j] = -input->Aij[j - N][i];
        }
        B_vec[i] = calc_enthalpy(&input->nasa9[i], T_REF) / (R_UNIVERSAL * T);
    }
    
    /* 气相方程 */
    for (i = L; i < N; i++) {
        A_mat[i][i] = 1.0;
        for (j = N; j < N + M; j++) {
            A_mat[i][j] = -input->Aij[j - N][i];
        }
        A_mat[i][N + M] = -1.0;
        B_vec[i] = calc_enthalpy(&input->nasa9[i], T_REF) / (R_UNIVERSAL * T);
    }
    
    /* 元素守恒方程 */
    for (i = N; i < N + M; i++) {
        for (j = 0; j < N; j++) {
            A_mat[i][j] = input->Aij[i - N][j] * moles[j];
        }
    }
    
    /* 气相摩尔数方程 */
    for (j = L; j < N; j++) {
        A_mat[N + M][j] = moles[j];
    }
    A_mat[N + M][N + M] = -ng;
    
    /* 求解 */
    if (solve_gaussian(eq_size, A_mat, B_vec, X_sol) == 0) {
        for (i = 0; i < N; i++) {
            dlnn_dlnT[i] = X_sol[i];
        }
        *dlnng_dlnT = X_sol[N + M];
    } else {
        for (i = 0; i < N; i++) {
            dlnn_dlnT[i] = 0.0;
        }
        *dlnng_dlnT = 0.0;
    }
}

void calc_partial_derivatives_p(const PropellantInput* input,
                                double T, double p,
                                const double moles[MAX_SPECIES],
                                double dlnn_dlnp[MAX_SPECIES],
                                double* dlnng_dlnp)
{
    int N = input->num_species;
    int M = input->num_elements;
    int L = input->num_condensed;
    int eq_size = N + M + 1;
    int i, j;
    
    double ng = 0.0;
    for (i = L; i < N; i++) {
        ng += moles[i];
    }
    
    double A_mat[MAX_SPECIES][MAX_SPECIES];
    double B_vec[MAX_SPECIES];
    double X_sol[MAX_SPECIES];
    
    memset(A_mat, 0, sizeof(A_mat));
    memset(B_vec, 0, sizeof(B_vec));
    
    /* 与温度偏导相同的矩阵结构，但右端向量不同 */
    for (i = 0; i < L; i++) {
        for (j = N; j < N + M; j++) {
            A_mat[i][j] = -input->Aij[j - N][i];
        }
        B_vec[i] = 0.0;
    }
    
    for (i = L; i < N; i++) {
        A_mat[i][i] = 1.0;
        for (j = N; j < N + M; j++) {
            A_mat[i][j] = -input->Aij[j - N][i];
        }
        A_mat[i][N + M] = -1.0;
        B_vec[i] = -1.0;  /* 关键区别 */
    }
    
    for (i = N; i < N + M; i++) {
        for (j = 0; j < N; j++) {
            A_mat[i][j] = input->Aij[i - N][j] * moles[j];
        }
    }
    
    for (j = L; j < N; j++) {
        A_mat[N + M][j] = moles[j];
    }
    A_mat[N + M][N + M] = -ng;
    
    if (solve_gaussian(eq_size, A_mat, B_vec, X_sol) == 0) {
        for (i = 0; i < N; i++) {
            dlnn_dlnp[i] = X_sol[i];
        }
        *dlnng_dlnp = X_sol[N + M];
    } else {
        for (i = 0; i < N; i++) {
            dlnn_dlnp[i] = 0.0;
        }
        *dlnng_dlnp = 0.0;
    }
}

/* ============ 喷管计算 ============ */

int nozzle_calc_from_pressure(const PropellantInput* input,
                              const ChamberResult* chamber,
                              double exit_pressure,
                              const double c_init[MAX_SPECIES],
                              NozzleResult* result)
{
    int N = input->num_species;
    int L = input->num_condensed;
    int i;
    double N_k[MAX_ELEMENTS];
    
    calc_assumed_formula(input, N_k);
    
    /* 估算出口温度初值 - 使用等熵关系 */
    double Te_guess = chamber->temperature * 
                      pow(exit_pressure / input->chamber_pressure,
                          (chamber->gamma_s - 1.0) / chamber->gamma_s);
    
    /* 扩大温度扫描范围 */
    double T_min = Te_guess * 0.5;  /* 下限为估算值的50% */
    double T_max = chamber->temperature * 0.98;  /* 上限接近燃烧室温度 */
    double T_step = (T_max - T_min) / 50.0;  /* 50个点 */
    
    if (T_min < 300.0) T_min = 300.0;
    if (T_step < 10.0) T_step = 10.0;
    
    /* 温度扫描求等熵条件 */
    typedef struct {
        double T;
        double S;
        double moles[MAX_SPECIES];
    } ScanPoint;
    ScanPoint points[200];
    int num_points = 0;
    
    /* 使用燃烧室组分作为初始猜测 */
    double c[MAX_SPECIES];
    for (i = 0; i < N; i++) {
        c[i] = chamber->mole_fractions[i];
        if (c[i] < MIN_MOLE_FRAC) c[i] = MIN_MOLE_FRAC;
    }
    
    /* 从高温向低温扫描 */
    for (double T = T_max; T >= T_min && num_points < 200; T -= T_step) {
        double c_result[MAX_SPECIES];
        int iter = min_gibbs_solve(input, T, exit_pressure, N_k, c, c_result);
        if (iter < 0) continue;
        
        /* 计算熵 */
        double ng = 0.0;
        for (i = L; i < N; i++) ng += c_result[i];
        if (ng < MIN_MOLE_FRAC) ng = MIN_MOLE_FRAC;
        
        double S = 0.0;
        for (i = 0; i < N; i++) {
            S += c_result[i] * calc_entropy(&input->nasa9[i], T);
        }
        /* 混合熵修正 */
        for (i = L; i < N; i++) {
            if (c_result[i] > MIN_MOLE_FRAC && ng > MIN_MOLE_FRAC) {
                S -= R_UNIVERSAL * c_result[i] * log(c_result[i] / ng);
            }
        }
        /* 压力修正 */
        S -= R_UNIVERSAL * ng * log(exit_pressure);
        
        points[num_points].T = T;
        points[num_points].S = S;
        for (i = 0; i < N; i++) {
            points[num_points].moles[i] = c_result[i];
            c[i] = c_result[i];
        }
        num_points++;
    }
    
    if (num_points < 2) {
        result->converged = 0;
        return -1;
    }
    
    /* 查找等熵点 - 燃烧室熵 */
    double S_chamber = chamber->total_entropy;
    int found = 0;
    
    for (i = 1; i < num_points; i++) {
        double S1 = points[i-1].S;
        double S2 = points[i].S;
        
        /* 检查 S_chamber 是否在 S1 和 S2 之间 */
        if ((S_chamber >= S1 && S_chamber <= S2) ||
            (S_chamber <= S1 && S_chamber >= S2)) {
            double dS = S2 - S1;
            if (fabs(dS) < 1e-10) continue;
            
            double ratio = (S_chamber - S1) / dS;
            result->exit_temperature = points[i-1].T + 
                                       ratio * (points[i].T - points[i-1].T);
            
            for (int j = 0; j < N; j++) {
                result->mole_fractions[j] = points[i-1].moles[j] + 
                                            ratio * (points[i].moles[j] - points[i-1].moles[j]);
            }
            found = 1;
            break;
        }
    }
    
    if (!found) {
        /* 如果找不到精确等熵点，使用最接近的点 */
        double min_diff = 1e30;
        int best_idx = 0;
        for (i = 0; i < num_points; i++) {
            double diff = fabs(points[i].S - S_chamber);
            if (diff < min_diff) {
                min_diff = diff;
                best_idx = i;
            }
        }
        
        /* 如果误差在10%以内，仍然使用该点 */
        if (min_diff < fabs(S_chamber) * 0.1) {
            result->exit_temperature = points[best_idx].T;
            for (int j = 0; j < N; j++) {
                result->mole_fractions[j] = points[best_idx].moles[j];
            }
            found = 1;
        }
    }
    
    if (!found) {
        result->converged = 0;
        return -1;
    }
    
    result->exit_pressure = exit_pressure;
    
    /* 计算出口速度 */
    double H_exit = 0.0;
    for (i = 0; i < N; i++) {
        H_exit += result->mole_fractions[i] * 
                  calc_enthalpy(&input->nasa9[i], result->exit_temperature);
    }
    
    double delta_H = chamber->total_enthalpy - H_exit;
    if (delta_H > 0) {
        result->exit_velocity = sqrt(2.0 * delta_H);
    } else {
        result->exit_velocity = 0.0;
    }
    
    /* 比冲 */
    result->specific_impulse = result->exit_velocity / 9.80665;
    
    /* 马赫数 */
    double ng_exit = 0.0;
    for (i = L; i < N; i++) ng_exit += result->mole_fractions[i];
    double sound_exit = sqrt(chamber->gamma_s * ng_exit * R_UNIVERSAL * result->exit_temperature);
    result->mach_number = result->exit_velocity / sound_exit;
    
    result->converged = 1;
    return 0;
}

int nozzle_calc_from_temperature(const PropellantInput* input,
                                 const ChamberResult* chamber,
                                 double exit_temperature,
                                 NozzleResult* result)
{
    int N = input->num_species;
    int L = input->num_condensed;
    int i;
    
    /* 冻结流动：组分不变 */
    result->exit_temperature = exit_temperature;
    for (i = 0; i < N; i++) {
        result->mole_fractions[i] = chamber->mole_fractions[i];
    }
    
    /* 计算出口压强 */
    result->exit_pressure = input->chamber_pressure * 
                            pow(exit_temperature / chamber->temperature,
                                chamber->gamma_s / (chamber->gamma_s - 1.0));
    
    /* 计算出口速度 */
    double H_exit = 0.0;
    for (i = 0; i < N; i++) {
        H_exit += result->mole_fractions[i] * 
                  calc_enthalpy(&input->nasa9[i], exit_temperature);
    }
    
    double delta_H = chamber->total_enthalpy - H_exit;
    if (delta_H > 0) {
        result->exit_velocity = sqrt(2.0 * delta_H);
    } else {
        result->exit_velocity = 0.0;
    }
    
    result->specific_impulse = result->exit_velocity / 9.80665;
    
    double ng = 0.0;
    for (i = L; i < N; i++) ng += result->mole_fractions[i];
    double sound = sqrt(chamber->gamma_s * ng * R_UNIVERSAL * exit_temperature);
    result->mach_number = result->exit_velocity / sound;
    
    result->converged = 1;
    return 0;
}

/* ============ 综合计算接口 ============ */

int thermo_calculate(const PropellantInput* input,
                     double exit_pressure,
                     ThermoResult* result)
{
    double c_init[MAX_SPECIES];
    int i;
    
    /* 初始化组分猜测值 */
    for (i = 0; i < input->num_species; i++) {
        c_init[i] = 1e-5;
    }
    /* H2/O2燃烧的主要产物初值: H, H2, H2O, O, O2, OH */
    if (input->num_species >= 6) {
        c_init[0] = 1e-3;   /* H */
        c_init[1] = 10.0;   /* H2 */
        c_init[2] = 30.0;   /* H2O - 主产物 */
        c_init[3] = 1e-4;   /* O */
        c_init[4] = 1e-3;   /* O2 */
        c_init[5] = 1.0;    /* OH */
    }
    
    /* 燃烧室计算 */
    int ret = combustion_chamber_calc(input, 
                                      DEFAULT_T_MIN, DEFAULT_T_MAX, DEFAULT_T_STEP,
                                      c_init, &result->chamber);
    
    if (ret != 0) {
        result->success = 0;
        snprintf(result->error_msg, sizeof(result->error_msg), 
                 "燃烧室计算未收敛");
        return -1;
    }
    
    /* 喷管计算 */
    if (exit_pressure > 0) {
        ret = nozzle_calc_from_pressure(input, &result->chamber, 
                                        exit_pressure, c_init, &result->nozzle);
        if (ret != 0) {
            result->success = 0;
            snprintf(result->error_msg, sizeof(result->error_msg), 
                     "喷管计算未收敛");
            return -1;
        }
    }
    
    result->success = 1;
    result->error_msg[0] = '\0';
    return 0;
}
