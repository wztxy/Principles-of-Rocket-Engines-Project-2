/**
 * @file propellant_db.c
 * @brief 推进剂数据库实现 - SLS发动机预设配置
 * @note 使用 NASA 9系数热力数据 (来自 NASA CEA thermo.inp)
 */

#include "propellant_db.h"
#include <string.h>

/* 辅助函数: 设置 NASA 9 系数区间 */
static void set_nasa9_interval(NASA9Interval* interval, 
                               double T_min, double T_max,
                               double a1, double a2, double a3, double a4, 
                               double a5, double a6, double a7, 
                               double b1, double b2)
{
    interval->T_min = T_min;
    interval->T_max = T_max;
    interval->a[0] = a1;
    interval->a[1] = a2;
    interval->a[2] = a3;
    interval->a[3] = a4;
    interval->a[4] = a5;
    interval->a[5] = a6;
    interval->a[6] = a7;
    interval->a[7] = b1;
    interval->a[8] = b2;
}

/* RS-25 发动机配置 - NASA 9 系数 */
static void init_rs25_config_internal(PropellantInput* input)
{
    memset(input, 0, sizeof(PropellantInput));
    
    input->num_propellants = 2;
    input->num_elements = 2;
    input->num_species = 6;
    input->num_condensed = 0;
    
    /* St_aij: H2 (2H,0O), O2 (0H,2O) */
    input->St_aij[0][0] = 2.0; input->St_aij[0][1] = 0.0;
    input->St_aij[1][0] = 0.0; input->St_aij[1][1] = 2.0;
    
    /* O/F = 6.0 */
    input->mass_fraction[0] = 0.14225;
    input->mass_fraction[1] = 0.85775;
    
    input->element_weight[0] = 1.00794;
    input->element_weight[1] = 15.9994;
    
    /* [0] H - Herzberg,1970. Moore,1972. Gordon,1999. */
    input->nasa9[0].num_intervals = 3;
    set_nasa9_interval(&input->nasa9[0].intervals[0], 200.0, 1000.0,
        0.0, 0.0, 2.5, 0.0, 0.0, 0.0, 0.0, 2.547370801E+04, -4.466828530E-01);
    set_nasa9_interval(&input->nasa9[0].intervals[1], 1000.0, 6000.0,
        6.078774250E+01, -1.819354417E-01, 2.500211817E+00, -1.226512864E-07,
        3.732876330E-11, -5.687744560E-15, 3.410210197E-19,
        2.547486398E+04, -4.481917770E-01);
    set_nasa9_interval(&input->nasa9[0].intervals[2], 6000.0, 20000.0,
        2.173757694E+08, -1.312035403E+05, 3.399174200E+01, -3.813999680E-03,
        2.432854837E-07, -7.694275540E-12, 9.644105630E-17,
        1.067638086E+06, -2.742301051E+02);
    
    /* [1] H2 - Gurvich,1978 */
    input->nasa9[1].num_intervals = 3;
    set_nasa9_interval(&input->nasa9[1].intervals[0], 200.0, 1000.0,
        4.078323210E+04, -8.009186040E+02, 8.214702010E+00, -1.269714457E-02,
        1.753605076E-05, -1.202860270E-08, 3.368093490E-12,
        2.682484665E+03, -3.043788844E+01);
    set_nasa9_interval(&input->nasa9[1].intervals[1], 1000.0, 6000.0,
        5.608128010E+05, -8.371504740E+02, 2.975364532E+00, 1.252249124E-03,
        -3.740716190E-07, 5.936625200E-11, -3.606994100E-15,
        5.339824410E+03, -2.202774769E+00);
    set_nasa9_interval(&input->nasa9[1].intervals[2], 6000.0, 20000.0,
        4.966884120E+08, -3.147547149E+05, 7.984121880E+01, -8.414789210E-03,
        4.753248350E-07, -1.371873492E-11, 1.605461756E-16,
        2.488433516E+06, -6.695728110E+02);
    
    /* [2] H2O - Cox,1989. Woolley,1987. */
    input->nasa9[2].num_intervals = 2;
    set_nasa9_interval(&input->nasa9[2].intervals[0], 200.0, 1000.0,
        -3.947960830E+04, 5.755731020E+02, 9.317826530E-01, 7.222712860E-03,
        -7.342557370E-06, 4.955043490E-09, -1.336933246E-12,
        -3.303974310E+04, 1.724205775E+01);
    set_nasa9_interval(&input->nasa9[2].intervals[1], 1000.0, 6000.0,
        1.034972096E+06, -2.412698562E+03, 4.646110780E+00, 2.291998307E-03,
        -6.836830480E-07, 9.426468930E-11, -4.822380530E-15,
        -1.384286509E+04, -7.978148510E+00);
    
    /* [3] O - Brix,1954. Moore,1976. Gordon,1999. */
    input->nasa9[3].num_intervals = 3;
    set_nasa9_interval(&input->nasa9[3].intervals[0], 200.0, 1000.0,
        -7.953611300E+03, 1.607177787E+02, 1.966226438E+00, 1.013670310E-03,
        -1.110415423E-06, 6.517507500E-10, -1.584779251E-13,
        2.840362437E+04, 8.404241820E+00);
    set_nasa9_interval(&input->nasa9[3].intervals[1], 1000.0, 6000.0,
        2.619020262E+05, -7.298722030E+02, 3.317177270E+00, -4.281334360E-04,
        1.036104594E-07, -9.438304330E-12, 2.725038297E-16,
        3.392428060E+04, -6.679585350E-01);
    set_nasa9_interval(&input->nasa9[3].intervals[2], 6000.0, 20000.0,
        1.779004264E+08, -1.082328257E+05, 2.810778365E+01, -2.975232262E-03,
        1.854997534E-07, -5.796231540E-12, 7.191720164E-17,
        8.890942630E+05, -2.181728151E+02);
    
    /* [4] O2 - Gurvich,1989 */
    input->nasa9[4].num_intervals = 3;
    set_nasa9_interval(&input->nasa9[4].intervals[0], 200.0, 1000.0,
        -3.425563420E+04, 4.847000970E+02, 1.119010961E+00, 4.293889240E-03,
        -6.836300520E-07, -2.023372700E-09, 1.039040018E-12,
        -3.391454870E+03, 1.849699470E+01);
    set_nasa9_interval(&input->nasa9[4].intervals[1], 1000.0, 6000.0,
        -1.037939022E+06, 2.344830282E+03, 1.819732036E+00, 1.267847582E-03,
        -2.188067988E-07, 2.053719572E-11, -8.193467050E-16,
        -1.689010929E+04, 1.738716506E+01);
    set_nasa9_interval(&input->nasa9[4].intervals[2], 6000.0, 20000.0,
        4.975294300E+08, -2.866106874E+05, 6.690352250E+01, -6.169959020E-03,
        3.016396027E-07, -7.421416600E-12, 7.278175770E-17,
        2.293554027E+06, -5.530621610E+02);
    
    /* [5] OH - Ruscic,2002. Gurvich,1978 */
    input->nasa9[5].num_intervals = 3;
    set_nasa9_interval(&input->nasa9[5].intervals[0], 200.0, 1000.0,
        -1.998858990E+03, 9.300136160E+01, 3.050854229E+00, 1.529529288E-03,
        -3.157890998E-06, 3.315446180E-09, -1.138762683E-12,
        2.991214235E+03, 4.674110790E+00);
    set_nasa9_interval(&input->nasa9[5].intervals[1], 1000.0, 6000.0,
        1.017393379E+06, -2.509957276E+03, 5.116547860E+00, 1.305299930E-04,
        -8.284322260E-08, 2.006475941E-11, -1.556993656E-15,
        2.019640206E+04, -1.101282337E+01);
    set_nasa9_interval(&input->nasa9[5].intervals[2], 6000.0, 20000.0,
        2.847234193E+08, -1.859532612E+05, 5.008240900E+01, -5.142374980E-03,
        2.875536589E-07, -8.228817960E-12, 9.567229020E-17,
        1.468393908E+06, -4.023555580E+02);
    
    /* Aij matrix */
    input->Aij[0][0] = 1.0; input->Aij[1][0] = 0.0;
    input->Aij[0][1] = 2.0; input->Aij[1][1] = 0.0;
    input->Aij[0][2] = 2.0; input->Aij[1][2] = 1.0;
    input->Aij[0][3] = 0.0; input->Aij[1][3] = 1.0;
    input->Aij[0][4] = 0.0; input->Aij[1][4] = 2.0;
    input->Aij[0][5] = 1.0; input->Aij[1][5] = 1.0;
    
    input->chamber_pressure = 202.0;
    input->initial_enthalpy = -1685000.0;  /* J/kg */
    input->initial_temperature = 298.15;
}

void init_rs25_config(PropellantInput* input) {
    init_rs25_config_internal(input);
}

void init_rl10_config(PropellantInput* input) {
    init_rs25_config_internal(input);
    input->chamber_pressure = 43.4;
    input->initial_enthalpy = -987000.0;  /* J/kg, RL-10 uses -987 kJ/kg */
    input->mass_fraction[0] = 0.1453;
    input->mass_fraction[1] = 0.8547;
}

/* LOX/CH4 推进剂配置 - 8种产物, 3种元素 */
void init_lox_ch4_config(PropellantInput* input)
{
    memset(input, 0, sizeof(PropellantInput));
    
    input->num_propellants = 2;
    input->num_elements = 3;      /* H, C, O */
    input->num_species = 8;       /* H2O, H2, OH, H, CO2, CO, O2, O */
    input->num_condensed = 0;
    
    /* St_aij: CH4 (4H,1C,0O), O2 (0H,0C,2O) */
    input->St_aij[0][0] = 4.0; input->St_aij[0][1] = 1.0; input->St_aij[0][2] = 0.0;
    input->St_aij[1][0] = 0.0; input->St_aij[1][1] = 0.0; input->St_aij[1][2] = 2.0;
    
    /* O/F = 3.5 (typical for CH4/LOX) */
    input->mass_fraction[0] = 1.0 / (1.0 + 3.5);  /* CH4: 0.222 */
    input->mass_fraction[1] = 3.5 / (1.0 + 3.5);  /* O2: 0.778 */
    
    input->element_weight[0] = 1.00794;   /* H */
    input->element_weight[1] = 12.0107;   /* C */
    input->element_weight[2] = 15.9994;   /* O */
    
    /* [0] H2O - Cox,1989. Woolley,1987. */
    input->nasa9[0].num_intervals = 2;
    set_nasa9_interval(&input->nasa9[0].intervals[0], 200.0, 1000.0,
        -3.947960830E+04, 5.755731020E+02, 9.317826530E-01, 7.222712860E-03,
        -7.342557370E-06, 4.955043490E-09, -1.336933246E-12,
        -3.303974310E+04, 1.724205775E+01);
    set_nasa9_interval(&input->nasa9[0].intervals[1], 1000.0, 6000.0,
        1.034972096E+06, -2.412698562E+03, 4.646110780E+00, 2.291998307E-03,
        -6.836830480E-07, 9.426468930E-11, -4.822380530E-15,
        -1.384286509E+04, -7.978148510E+00);
    
    /* [1] H2 - Gurvich,1978 */
    input->nasa9[1].num_intervals = 3;
    set_nasa9_interval(&input->nasa9[1].intervals[0], 200.0, 1000.0,
        4.078323210E+04, -8.009186040E+02, 8.214702010E+00, -1.269714457E-02,
        1.753605076E-05, -1.202860270E-08, 3.368093490E-12,
        2.682484665E+03, -3.043788844E+01);
    set_nasa9_interval(&input->nasa9[1].intervals[1], 1000.0, 6000.0,
        5.608128010E+05, -8.371504740E+02, 2.975364532E+00, 1.252249124E-03,
        -3.740716190E-07, 5.936625200E-11, -3.606994100E-15,
        5.339824410E+03, -2.202774769E+00);
    set_nasa9_interval(&input->nasa9[1].intervals[2], 6000.0, 20000.0,
        4.966884120E+08, -3.147547149E+05, 7.984121880E+01, -8.414789210E-03,
        4.753248350E-07, -1.371873492E-11, 1.605461756E-16,
        2.488433516E+06, -6.695728110E+02);
    
    /* [2] OH - Ruscic,2002. Gurvich,1978 */
    input->nasa9[2].num_intervals = 3;
    set_nasa9_interval(&input->nasa9[2].intervals[0], 200.0, 1000.0,
        -1.998858990E+03, 9.300136160E+01, 3.050854229E+00, 1.529529288E-03,
        -3.157890998E-06, 3.315446180E-09, -1.138762683E-12,
        2.991214235E+03, 4.674110790E+00);
    set_nasa9_interval(&input->nasa9[2].intervals[1], 1000.0, 6000.0,
        1.017393379E+06, -2.509957276E+03, 5.116547860E+00, 1.305299930E-04,
        -8.284322260E-08, 2.006475941E-11, -1.556993656E-15,
        2.019640206E+04, -1.101282337E+01);
    set_nasa9_interval(&input->nasa9[2].intervals[2], 6000.0, 20000.0,
        2.847234193E+08, -1.859532612E+05, 5.008240900E+01, -5.142374980E-03,
        2.875536589E-07, -8.228817960E-12, 9.567229020E-17,
        1.468393908E+06, -4.023555580E+02);
    
    /* [3] H - Herzberg,1970. Moore,1972. Gordon,1999. */
    input->nasa9[3].num_intervals = 3;
    set_nasa9_interval(&input->nasa9[3].intervals[0], 200.0, 1000.0,
        0.0, 0.0, 2.5, 0.0, 0.0, 0.0, 0.0, 2.547370801E+04, -4.466828530E-01);
    set_nasa9_interval(&input->nasa9[3].intervals[1], 1000.0, 6000.0,
        6.078774250E+01, -1.819354417E-01, 2.500211817E+00, -1.226512864E-07,
        3.732876330E-11, -5.687744560E-15, 3.410210197E-19,
        2.547486398E+04, -4.481917770E-01);
    set_nasa9_interval(&input->nasa9[3].intervals[2], 6000.0, 20000.0,
        2.173757694E+08, -1.312035403E+05, 3.399174200E+01, -3.813999680E-03,
        2.432854837E-07, -7.694275540E-12, 9.644105630E-17,
        1.067638086E+06, -2.742301051E+02);
    
    /* [4] CO2 - Gurvich,1991 */
    input->nasa9[4].num_intervals = 3;
    set_nasa9_interval(&input->nasa9[4].intervals[0], 200.0, 1000.0,
        4.943650540E+04, -6.264116010E+02, 5.301725240E+00, 2.503813816E-03,
        -2.127308728E-07, -7.689988780E-10, 2.849677801E-13,
        -4.528198460E+04, -7.048279440E+00);
    set_nasa9_interval(&input->nasa9[4].intervals[1], 1000.0, 6000.0,
        1.176962419E+05, -1.788791477E+03, 8.291523190E+00, -9.223156780E-05,
        4.863676880E-09, -1.891053312E-12, 6.330036590E-16,
        -3.908350590E+04, -2.652669281E+01);
    set_nasa9_interval(&input->nasa9[4].intervals[2], 6000.0, 20000.0,
        -1.544423287E+09, 1.016847056E+06, -2.561405230E+02, 3.369401080E-02,
        -2.181184337E-06, 6.991420840E-11, -8.842351500E-16,
        -8.043214510E+06, 2.254177493E+03);
    
    /* [5] CO - Gurvich,1979 */
    input->nasa9[5].num_intervals = 3;
    set_nasa9_interval(&input->nasa9[5].intervals[0], 200.0, 1000.0,
        1.489045326E+04, -2.922285939E+02, 5.724527170E+00, -8.176235030E-03,
        1.456903469E-05, -1.087746302E-08, 3.027941827E-12,
        -1.303131878E+04, -7.859241350E+00);
    set_nasa9_interval(&input->nasa9[5].intervals[1], 1000.0, 6000.0,
        4.619197250E+05, -1.944704863E+03, 5.916714180E+00, -5.664282830E-04,
        1.398814540E-07, -1.787680361E-11, 9.620935570E-16,
        -2.466261084E+03, -1.387413108E+01);
    set_nasa9_interval(&input->nasa9[5].intervals[2], 6000.0, 20000.0,
        8.868662960E+08, -7.500377840E+05, 2.495474979E+02, -3.956351100E-02,
        3.297772080E-06, -1.318409933E-10, 1.998937948E-15,
        5.701421130E+06, -2.060704786E+03);
    
    /* [6] O2 - Gurvich,1989 */
    input->nasa9[6].num_intervals = 3;
    set_nasa9_interval(&input->nasa9[6].intervals[0], 200.0, 1000.0,
        -3.425563420E+04, 4.847000970E+02, 1.119010961E+00, 4.293889240E-03,
        -6.836300520E-07, -2.023372700E-09, 1.039040018E-12,
        -3.391454870E+03, 1.849699470E+01);
    set_nasa9_interval(&input->nasa9[6].intervals[1], 1000.0, 6000.0,
        -1.037939022E+06, 2.344830282E+03, 1.819732036E+00, 1.267847582E-03,
        -2.188067988E-07, 2.053719572E-11, -8.193467050E-16,
        -1.689010929E+04, 1.738716506E+01);
    set_nasa9_interval(&input->nasa9[6].intervals[2], 6000.0, 20000.0,
        4.975294300E+08, -2.866106874E+05, 6.690352250E+01, -6.169959020E-03,
        3.016396027E-07, -7.421416600E-12, 7.278175770E-17,
        2.293554027E+06, -5.530621610E+02);
    
    /* [7] O - Brix,1954. Moore,1976. Gordon,1999. */
    input->nasa9[7].num_intervals = 3;
    set_nasa9_interval(&input->nasa9[7].intervals[0], 200.0, 1000.0,
        -7.953611300E+03, 1.607177787E+02, 1.966226438E+00, 1.013670310E-03,
        -1.110415423E-06, 6.517507500E-10, -1.584779251E-13,
        2.840362437E+04, 8.404241820E+00);
    set_nasa9_interval(&input->nasa9[7].intervals[1], 1000.0, 6000.0,
        2.619020262E+05, -7.298722030E+02, 3.317177270E+00, -4.281334360E-04,
        1.036104594E-07, -9.438304330E-12, 2.725038297E-16,
        3.392428060E+04, -6.679585350E-01);
    set_nasa9_interval(&input->nasa9[7].intervals[2], 6000.0, 20000.0,
        1.779004264E+08, -1.082328257E+05, 2.810778365E+01, -2.975232262E-03,
        1.854997534E-07, -5.796231540E-12, 7.191720164E-17,
        8.890942630E+05, -2.181728151E+02);
    
    /* Aij matrix: 元素(H,C,O) × 产物(H2O,H2,OH,H,CO2,CO,O2,O)
     * 来自程序/test.cpp:
     * Aij[0] = {2,2,1,1,0,0,0,0}  H含量
     * Aij[1] = {0,0,0,0,1,1,0,0}  C含量
     * Aij[2] = {1,0,1,0,2,1,2,1}  O含量
     */
    /* H含量 */
    input->Aij[0][0] = 2.0;  /* H2O */
    input->Aij[0][1] = 2.0;  /* H2 */
    input->Aij[0][2] = 1.0;  /* OH */
    input->Aij[0][3] = 1.0;  /* H */
    input->Aij[0][4] = 0.0;  /* CO2 */
    input->Aij[0][5] = 0.0;  /* CO */
    input->Aij[0][6] = 0.0;  /* O2 */
    input->Aij[0][7] = 0.0;  /* O */
    
    /* C含量 */
    input->Aij[1][0] = 0.0;  /* H2O */
    input->Aij[1][1] = 0.0;  /* H2 */
    input->Aij[1][2] = 0.0;  /* OH */
    input->Aij[1][3] = 0.0;  /* H */
    input->Aij[1][4] = 1.0;  /* CO2 */
    input->Aij[1][5] = 1.0;  /* CO */
    input->Aij[1][6] = 0.0;  /* O2 */
    input->Aij[1][7] = 0.0;  /* O */
    
    /* O含量 */
    input->Aij[2][0] = 1.0;  /* H2O */
    input->Aij[2][1] = 0.0;  /* H2 */
    input->Aij[2][2] = 1.0;  /* OH */
    input->Aij[2][3] = 0.0;  /* H */
    input->Aij[2][4] = 2.0;  /* CO2 */
    input->Aij[2][5] = 1.0;  /* CO */
    input->Aij[2][6] = 2.0;  /* O2 */
    input->Aij[2][7] = 1.0;  /* O */
    
    input->chamber_pressure = 300.0;  /* atm, typical for Raptor */
    input->initial_enthalpy = -2875720.0;  /* J/kg, from 程序/test.cpp */
    input->initial_temperature = 298.15;
}

/* Raptor 发动机配置 (SpaceX) */
void init_raptor_config(PropellantInput* input)
{
    init_lox_ch4_config(input);
    input->chamber_pressure = 300.0;  /* atm */
    /* O/F = 3.6 */
    input->mass_fraction[0] = 1.0 / (1.0 + 3.6);  /* CH4 */
    input->mass_fraction[1] = 3.6 / (1.0 + 3.6);  /* O2 */
}

void calc_mass_fractions(double mixture_ratio, double* oxidizer_fraction, double* fuel_fraction) {
    *oxidizer_fraction = mixture_ratio / (1.0 + mixture_ratio);
    *fuel_fraction = 1.0 / (1.0 + mixture_ratio);
}

void get_initial_guess(EngineType engine_type, double c_init[MAX_SPECIES], int num_species) {
    int i;
    for (i = 0; i < MAX_SPECIES; i++) c_init[i] = 1e-7;
    
    if ((engine_type == ENGINE_RS25 || engine_type == ENGINE_RL10) && num_species >= 6) {
        /* LOX/LH2: H, H2, H2O, O, O2, OH */
        c_init[0] = 1e-3; c_init[1] = 10.0; c_init[2] = 30.0;
        c_init[3] = 1e-4; c_init[4] = 1e-3; c_init[5] = 1.0;
    } else if (engine_type == ENGINE_RAPTOR && num_species >= 8) {
        /* LOX/CH4: H2O, H2, OH, H, CO2, CO, O2, O (来自程序/test.cpp) */
        c_init[0] = 35.6; c_init[1] = 1e-1; c_init[2] = 1e-2;
        c_init[3] = 1e-5; c_init[4] = 5.6;  c_init[5] = 6.0;
        c_init[6] = 1e-5; c_init[7] = 1e-6;
    } else if (num_species >= 2) {
        c_init[0] = 10.0; c_init[1] = 5.0;
    }
}

static EnginePreset g_presets[4];
static int g_presets_initialized = 0;

static void init_presets(void) {
    if (g_presets_initialized) return;
    
    g_presets[0].name = "RS-25 (SSME)";
    g_presets[0].description = "SLS Core Stage Engine, LH2/LOX";
    g_presets[0].chamber_pressure = 20.47;
    g_presets[0].mixture_ratio = 6.0;
    g_presets[0].thrust = 2279.0;
    g_presets[0].specific_impulse_vac = 452.0;
    init_rs25_config(&g_presets[0].config);
    
    g_presets[1].name = "RL-10B2";
    g_presets[1].description = "Upper Stage Engine, LH2/LOX";
    g_presets[1].chamber_pressure = 4.4;
    g_presets[1].mixture_ratio = 5.88;
    g_presets[1].thrust = 110.0;
    g_presets[1].specific_impulse_vac = 465.5;
    init_rl10_config(&g_presets[1].config);
    
    g_presets[2].name = "Raptor";
    g_presets[2].description = "SpaceX Full-Flow Staged Combustion, CH4/LOX";
    g_presets[2].chamber_pressure = 30.0;
    g_presets[2].mixture_ratio = 3.6;
    g_presets[2].thrust = 2260.0;
    g_presets[2].specific_impulse_vac = 363.0;
    init_raptor_config(&g_presets[2].config);
    
    g_presets[3].name = "Custom";
    g_presets[3].description = "User Defined Parameters";
    g_presets[3].chamber_pressure = 10.0;
    g_presets[3].mixture_ratio = 6.0;
    g_presets[3].thrust = 0.0;
    g_presets[3].specific_impulse_vac = 0.0;
    init_rs25_config(&g_presets[3].config);
    
    g_presets_initialized = 1;
}

const EnginePreset* get_engine_preset(EngineType type) {
    init_presets();
    if (type < 0 || type > ENGINE_CUSTOM) return NULL;
    return &g_presets[type];
}

const EnginePreset* get_all_presets(int* count) {
    init_presets();
    *count = 4;
    return g_presets;
}
