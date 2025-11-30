/**
 * @file nasa_cea_data.h
 * @brief NASA CEA 热力数据库 - 来自 RPA thermo.inp (Gurvich/JANAF)
 * @note 数据源: NASA Glenn Research Center CEA, Gurvich 1978-1996, NIST-JANAF
 * 
 * NASA 9 系数多项式格式:
 * Cp/R = a1*T^(-2) + a2*T^(-1) + a3 + a4*T + a5*T^2 + a6*T^3 + a7*T^4
 * H/RT = -a1*T^(-2) + a2*ln(T)/T + a3 + a4*T/2 + a5*T^2/3 + a6*T^3/4 + a7*T^4/5 + b1/T
 * S/R = -a1*T^(-2)/2 - a2*T^(-1) + a3*ln(T) + a4*T + a5*T^2/2 + a6*T^3/3 + a7*T^4/4 + b2
 */

#ifndef NASA_CEA_DATA_H
#define NASA_CEA_DATA_H

#define NASA9_COEFF_NUM 9  /* 7个多项式系数 + 2个积分常数 b1, b2 */

/**
 * NASA 9系数热力数据结构
 * 每种组分有两个温度区间: 200-1000K 和 1000-6000K
 */
typedef struct {
    const char* name;           /* 组分名称 */
    const char* formula;        /* 化学式 */
    double molecular_weight;    /* 分子量 g/mol */
    double Hf_298;             /* 298.15K 时的生成焓 J/mol */
    
    /* 低温区系数 (200-1000K) */
    double a_low[NASA9_COEFF_NUM];
    double T_low_min, T_low_max;
    
    /* 高温区系数 (1000-6000K) */
    double a_high[NASA9_COEFF_NUM];
    double T_high_min, T_high_max;
    
    /* 元素组成: [H, O] */
    int atoms[2];
} NASA_Species;

/**
 * H2/O2 燃烧系统的产物组分数据
 * 数据来源: RPA-4.0.7/resources/thermo.inp
 * 原始来源: NASA Glenn CEA, Gurvich IVTANTHERMO
 */

/* H - 原子氢 (Hf = 217998.828 J/mol) */
/* 来源: D0(H2):Herzberg,1970. Moore,1972. Gordon,1999. */
static const NASA_Species SPECIES_H = {
    .name = "H",
    .formula = "H",
    .molecular_weight = 1.00794,
    .Hf_298 = 217998.828,
    /* 200-1000K */
    .a_low = {
        0.000000000E+00,  /* a1 */
        0.000000000E+00,  /* a2 */
        2.500000000E+00,  /* a3 */
        0.000000000E+00,  /* a4 */
        0.000000000E+00,  /* a5 */
        0.000000000E+00,  /* a6 */
        0.000000000E+00,  /* a7 */
        2.547370801E+04,  /* b1 (H/R integration constant) */
       -4.466828530E-01   /* b2 (S/R integration constant) */
    },
    .T_low_min = 200.0, .T_low_max = 1000.0,
    /* 1000-6000K */
    .a_high = {
        6.078774250E+01,
       -1.819354417E-01,
        2.500211817E+00,
       -1.226512864E-07,
        3.732876330E-11,
       -5.687744560E-15,
        3.410210197E-19,
        2.547486398E+04,
       -4.481917770E-01
    },
    .T_high_min = 1000.0, .T_high_max = 6000.0,
    .atoms = {1, 0}  /* 1H, 0O */
};

/* H2 - 分子氢 (Hf = 0 参考态) */
/* 来源: Ref-Elm. Gurvich,1978 pt1 p103 pt2 p31. */
static const NASA_Species SPECIES_H2 = {
    .name = "H2",
    .formula = "H2",
    .molecular_weight = 2.01588,
    .Hf_298 = 0.0,
    /* 200-1000K */
    .a_low = {
        4.078323210E+04,
       -8.009186040E+02,
        8.214702010E+00,
       -1.269714457E-02,
        1.753605076E-05,
       -1.202860270E-08,
        3.368093490E-12,
        2.682484665E+03,
       -3.043788844E+01
    },
    .T_low_min = 200.0, .T_low_max = 1000.0,
    /* 1000-6000K */
    .a_high = {
        5.608128010E+05,
       -8.371504740E+02,
        2.975364532E+00,
        1.252249124E-03,
       -3.740716190E-07,
        5.936625200E-11,
       -3.606994100E-15,
        5.339824410E+03,
       -2.202774769E+00
    },
    .T_high_min = 1000.0, .T_high_max = 6000.0,
    .atoms = {2, 0}  /* 2H, 0O */
};

/* H2O - 水蒸气 (Hf = -241826 J/mol) */
/* 来源: Hf:Cox,1989. Woolley,1987. TRC(10/88) tuv25. */
static const NASA_Species SPECIES_H2O = {
    .name = "H2O",
    .formula = "H2O",
    .molecular_weight = 18.01528,
    .Hf_298 = -241826.0,
    /* 200-1000K */
    .a_low = {
       -3.947960830E+04,
        5.755731020E+02,
        9.317826530E-01,
        7.222712860E-03,
       -7.342557370E-06,
        4.955043490E-09,
       -1.336933246E-12,
       -3.303974310E+04,
        1.724205775E+01
    },
    .T_low_min = 200.0, .T_low_max = 1000.0,
    /* 1000-6000K */
    .a_high = {
        1.034972096E+06,
       -2.412698562E+03,
        4.646110780E+00,
        2.291998307E-03,
       -6.836830480E-07,
        9.426468930E-11,
       -4.822380530E-15,
       -1.384286509E+04,
       -7.978148510E+00
    },
    .T_high_min = 1000.0, .T_high_max = 6000.0,
    .atoms = {2, 1}  /* 2H, 1O */
};

/* O - 原子氧 (Hf = 249175.003 J/mol) */
/* 来源: D0(O2):Brix,1954. Moore,1976. Gordon,1999. */
static const NASA_Species SPECIES_O = {
    .name = "O",
    .formula = "O",
    .molecular_weight = 15.9994,
    .Hf_298 = 249175.003,
    /* 200-1000K */
    .a_low = {
       -7.953611300E+03,
        1.607177787E+02,
        1.966226438E+00,
        1.013670310E-03,
       -1.110415423E-06,
        6.517507500E-10,
       -1.584779251E-13,
        2.840362437E+04,
        8.404241820E+00
    },
    .T_low_min = 200.0, .T_low_max = 1000.0,
    /* 1000-6000K */
    .a_high = {
        2.619020262E+05,
       -7.298722030E+02,
        3.317177270E+00,
       -4.281334360E-04,
        1.036104594E-07,
       -9.438304330E-12,
        2.725038297E-16,
        3.392428060E+04,
       -6.679585350E-01
    },
    .T_high_min = 1000.0, .T_high_max = 6000.0,
    .atoms = {0, 1}  /* 0H, 1O */
};

/* O2 - 分子氧 (Hf = 0 参考态) */
/* 来源: Ref-Elm. Gurvich,1989 pt1 p94 pt2 p9. */
static const NASA_Species SPECIES_O2 = {
    .name = "O2",
    .formula = "O2",
    .molecular_weight = 31.9988,
    .Hf_298 = 0.0,
    /* 200-1000K */
    .a_low = {
       -3.425563420E+04,
        4.847000970E+02,
        1.119010961E+00,
        4.293889240E-03,
       -6.836300520E-07,
       -2.023372700E-09,
        1.039040018E-12,
       -3.391454870E+03,
        1.849699470E+01
    },
    .T_low_min = 200.0, .T_low_max = 1000.0,
    /* 1000-6000K */
    .a_high = {
       -1.037939022E+06,
        2.344830282E+03,
        1.819732036E+00,
        1.267847582E-03,
       -2.188067988E-07,
        2.053719572E-11,
       -8.193467050E-16,
       -1.689010929E+04,
        1.738716506E+01
    },
    .T_high_min = 1000.0, .T_high_max = 6000.0,
    .atoms = {0, 2}  /* 0H, 2O */
};

/* OH - 羟基 (Hf = 37278.206 J/mol) */
/* 来源: D0(H-OH): Ruscic,2002. Gurvich,1978 pt1 p110 pt2 p37. */
static const NASA_Species SPECIES_OH = {
    .name = "OH",
    .formula = "OH",
    .molecular_weight = 17.00734,
    .Hf_298 = 37278.206,
    /* 200-1000K */
    .a_low = {
       -1.998858990E+03,
        9.300136160E+01,
        3.050854229E+00,
        1.529529288E-03,
       -3.157890998E-06,
        3.315446180E-09,
       -1.138762683E-12,
        2.991214235E+03,
        4.674110790E+00
    },
    .T_low_min = 200.0, .T_low_max = 1000.0,
    /* 1000-6000K */
    .a_high = {
        1.017393379E+06,
       -2.509957276E+03,
        5.116547860E+00,
        1.305299930E-04,
       -8.284322260E-08,
        2.006475941E-11,
       -1.556993656E-15,
        2.019640206E+04,
       -1.101282337E+01
    },
    .T_high_min = 1000.0, .T_high_max = 6000.0,
    .atoms = {1, 1}  /* 1H, 1O */
};

/* 组分数组 - 按顺序: H, H2, H2O, O, O2, OH */
static const NASA_Species* ALL_SPECIES[6] = {
    &SPECIES_H,
    &SPECIES_H2,
    &SPECIES_H2O,
    &SPECIES_O,
    &SPECIES_O2,
    &SPECIES_OH
};

#define NUM_H2O2_SPECIES 6

/**
 * SLS 发动机真实参数 (来自 RPA cfg 文件和公开数据)
 */
typedef struct {
    const char* name;
    const char* description;
    double chamber_pressure_MPa;  /* 燃烧室压强 MPa */
    double mixture_ratio;         /* 混合比 O/F */
    double thrust_kN;             /* 推力 kN */
    double Isp_vac;              /* 真空比冲 s */
    double area_ratio;           /* 面积比 */
} RealEngineData;

/* RS-25 (SSME) 数据 - 来源: RPA SSME 40k.cfg */
static const RealEngineData ENGINE_RS25_DATA = {
    .name = "RS-25 (SSME)",
    .description = "SLS核心级发动机，液氢/液氧，NASA/Aerojet Rocketdyne",
    .chamber_pressure_MPa = 20.47,
    .mixture_ratio = 6.0,
    .thrust_kN = 2279.0,   /* 真空推力 */
    .Isp_vac = 452.0,
    .area_ratio = 77.5     /* 喷管面积比 */
};

/* RL-10B2 数据 - 来源: RPA RL10B2.cfg */
static const RealEngineData ENGINE_RL10_DATA = {
    .name = "RL-10B2",
    .description = "上面级发动机，液氢/液氧，Aerojet Rocketdyne",
    .chamber_pressure_MPa = 4.3644,
    .mixture_ratio = 5.88,
    .thrust_kN = 110.0,
    .Isp_vac = 465.5,
    .area_ratio = 285.0
};

/* J-2X 数据 - 来源: NASA公开资料 */
static const RealEngineData ENGINE_J2X_DATA = {
    .name = "J-2X",
    .description = "探索上面级发动机，液氢/液氧 (已取消)",
    .chamber_pressure_MPa = 9.2,
    .mixture_ratio = 5.5,
    .thrust_kN = 1307.0,
    .Isp_vac = 448.0,
    .area_ratio = 92.0
};

#endif /* NASA_CEA_DATA_H */
