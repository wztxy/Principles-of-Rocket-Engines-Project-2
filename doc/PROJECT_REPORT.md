# SLS 火箭发动机热力计算系统 - 项目报告

**项目名称**：SLS_ThermoCalc  
**版本**：v1.0.0  
**完成日期**：2025年12月  
**课程**：火箭发动机原理

---

## 目录

1. [项目概述](#1-项目概述)
2. [理论基础与算法原理](#2-理论基础与算法原理)
3. [系统架构与实现](#3-系统架构与实现)
4. [误差分析与验证](#4-误差分析与验证)
5. [创新点与优化策略](#5-创新点与优化策略)
6. [使用说明](#6-使用说明)
7. [结论与展望](#7-结论与展望)
8. [参考文献](#8-参考文献)

---

## 1. 项目概述

### 1.1 项目目标

本项目旨在开发一套基于**最小吉布斯自由能法**的火箭发动机热力性能计算系统，实现以下功能：

1. **燃烧室计算**：给定推进剂组分和燃烧室压强，求解绝热火焰温度和平衡组分
2. **喷管计算**：基于等熵膨胀模型计算喷管性能参数（比冲、推力系数等）
3. **热力参数**：计算完整的热力学和输运性质（比热、声速、粘度、导热系数等）

### 1.2 支持的发动机类型

| 发动机 | 推进剂 | 燃烧室压强 | 混合比 O/F | 真空比冲 |
|--------|--------|------------|------------|----------|
| RS-25 (SSME) | LOX/LH2 | 20.64 MPa | 6.03 | 452 s |
| RL-10B2 | LOX/LH2 | 4.36 MPa | 5.88 | 465 s |
| J-2X | LOX/LH2 | 9.2 MPa | 5.5 | 448 s |
| Raptor | LOX/CH4 | 30 MPa | 3.6 | 363 s |
| YF-77 | LOX/LH2 | 10.2 MPa | 5.5 | 430 s |
| Vulcain-2 | LOX/LH2 | 11.7 MPa | 6.1 | 434 s |

### 1.3 技术栈

- **核心算法**：纯 C 语言实现（`thermo_core.c`）
- **GUI 界面**：Qt 6 框架（C++）
- **热力数据**：NASA 9 系数多项式格式
- **构建系统**：qmake / CMake
- **CI/CD**：GitHub Actions（支持 macOS/Windows/Linux 多平台）

---

## 2. 理论基础与算法原理

### 2.1 最小吉布斯自由能法

化学平衡状态对应系统吉布斯自由能最小的状态。对于给定温度 $T$ 和压强 $p$ 的多组分系统：

$$
G = \sum_{j=1}^{N_s} n_j \mu_j \rightarrow \min
$$

受元素守恒约束：

$$
\sum_{j=1}^{N_s} a_{jk} n_j = N_k, \quad k = 1, 2, \ldots, M_e
$$

其中：
- $n_j$：第 $j$ 种产物的摩尔数
- $\mu_j$：第 $j$ 种产物的化学势
- $a_{jk}$：1 mol 第 $j$ 种产物中第 $k$ 种元素的原子数
- $N_k$：推进剂假定化学式中第 $k$ 种元素的摩尔数

### 2.2 化学势计算

对于气相组分：

$$
\mu_j = g_{mol,j}^{\ominus}(T) + R_0 T \ln \frac{p_j}{p^{\ominus}}
$$

对于凝相组分：

$$
\mu_j = g_{mol,j}^{\ominus c}(T)
$$

### 2.3 迭代求解方程组

引入拉格朗日乘子 $\lambda_k$ 和 $\pi$，将约束优化问题转化为求解非线性方程组：

**凝相组分**（$j = 1, \ldots, L$）：

$$
-\sum_{k=1}^{M_e} a_{jk} \lambda_k = \frac{g_{mol,j}^{\ominus c}}{R_0 T}
$$

**气相组分**（$j = L+1, \ldots, N_s$）：

$$
\ln n_j - \ln n_g + \ln p - \sum_{k=1}^{M_e} a_{jk} \lambda_k - \pi = \frac{g_{mol,j}^{\ominus}}{R_0 T}
$$

**元素守恒**：

$$
\sum_{j=1}^{N_s} a_{jk} n_j = N_k, \quad k = 1, \ldots, M_e
$$

**气相摩尔数定义**：

$$
n_g = \sum_{j=L+1}^{N_s} n_j
$$

### 2.4 NASA 9 系数多项式

本项目采用 **NASA 9 系数多项式**格式计算热力学性质，适用于 200K - 20000K 的宽温度范围：

**定压比热**：

$$
\frac{C_p}{R} = a_1 T^{-2} + a_2 T^{-1} + a_3 + a_4 T + a_5 T^2 + a_6 T^3 + a_7 T^4
$$

**标准摩尔焓**：

$$
\frac{H}{RT} = -a_1 T^{-2} + a_2 \frac{\ln T}{T} + a_3 + \frac{a_4 T}{2} + \frac{a_5 T^2}{3} + \frac{a_6 T^3}{4} + \frac{a_7 T^4}{5} + \frac{b_1}{T}
$$

**标准摩尔熵**：

$$
\frac{S}{R} = -\frac{a_1 T^{-2}}{2} - a_2 T^{-1} + a_3 \ln T + a_4 T + \frac{a_5 T^2}{2} + \frac{a_6 T^3}{3} + \frac{a_7 T^4}{4} + b_2
$$

### 2.5 燃烧温度确定

通过**焓守恒**（绝热条件）确定燃烧温度：

$$
H_{反应物}(T_0) = H_{产物}(T_c)
$$

具体实现采用**温度扫描 + 线性插值**方法：
1. 在温度范围 $[T_{min}, T_{max}]$ 内以步长 $\Delta T$ 扫描
2. 对每个温度 $T$ 调用最小吉布斯自由能法求解平衡组分
3. 计算产物总焓 $H(T)$
4. 找到满足 $H(T) = H_{pre}$ 的温度点，线性插值得到精确燃烧温度

### 2.6 喷管等熵膨胀

喷管流动满足**等熵条件**：

$$
S_c = S_e
$$

给定出口压强 $p_e$，采用类似的温度扫描方法求解出口温度和组分。

出口速度由能量方程得到：

$$
u_e = \sqrt{2(h_c - h_e)}
$$

比冲：

$$
I_{sp} = \frac{u_e}{g_0}
$$

---

## 3. 系统架构与实现

### 3.1 模块结构

```
SLS_ThermoCalc/
├── include/
│   ├── constants.h         # 物理常数定义
│   ├── thermo_core.h       # 热力计算核心 API
│   ├── propellant_db.h     # 推进剂数据库接口
│   ├── matrix_solver.h     # 矩阵求解器
│   ├── nasa_cea_data.h     # NASA CEA 数据结构
│   └── mainwindow.h        # Qt 主窗口
├── src/
│   ├── thermo_core.c       # 核心算法实现 (~700 行)
│   ├── propellant_db.c     # 推进剂配置和 NASA 9 系数
│   ├── matrix_solver.c     # 高斯消元求解器
│   ├── mainwindow.cpp      # GUI 实现
│   └── main.cpp            # 程序入口
├── ui/
│   └── mainwindow.ui       # Qt Designer 界面文件
└── presets/
    ├── RS-25_SSME.json     # 发动机预设配置
    ├── RL-10B2.json
    └── ...
```

### 3.2 核心数据结构

```c
/* NASA 9 系数区间 */
typedef struct {
    double T_min, T_max;        /* 温度范围 */
    double a[9];                /* a1-a7, b1, b2 */
} NASA9Interval;

/* 推进剂输入参数 */
typedef struct {
    int num_propellants;        /* 推进剂组元数 */
    int num_elements;           /* 元素数 */
    int num_species;            /* 平衡组分数 */
    int num_condensed;          /* 凝相组分数 */
    
    double St_aij[P][M];        /* 组元-元素原子数矩阵 */
    double mass_fraction[P];    /* 组元质量分数 */
    double element_weight[M];   /* 元素原子量 */
    NASA9Coefficients nasa9[N]; /* NASA 9 系数 */
    double Aij[M][N];           /* 产物-元素原子数矩阵 */
    
    double chamber_pressure;    /* 燃烧室压强 (atm) */
    double initial_enthalpy;    /* 推进剂初始焓 (J/kg) */
} PropellantInput;

/* 燃烧室计算结果 */
typedef struct {
    double temperature;         /* 燃烧温度 (K) */
    double mole_fractions[N];   /* 平衡组分 */
    double gamma, gamma_s;      /* 比热比、等熵指数 */
    double sound_speed;         /* 声速 (m/s) */
    double char_velocity;       /* 特征速度 c* (m/s) */
    /* ... 更多热力参数 ... */
} ChamberResult;
```

### 3.3 计算流程

```
┌─────────────────────────────────────────────────────────────────┐
│                         输入参数                                 │
│  · 推进剂组分 (H2/O2 或 CH4/O2)                                  │
│  · 混合比 O/F                                                    │
│  · 燃烧室压强 p_c                                                │
│  · 喷管出口压强 p_e                                              │
└───────────────────────────┬─────────────────────────────────────┘
                            ▼
┌─────────────────────────────────────────────────────────────────┐
│                    1. 假定化学式计算                              │
│  calc_assumed_formula()                                          │
│  · 计算 N_k = Σ(g_i × B_ik)                                      │
└───────────────────────────┬─────────────────────────────────────┘
                            ▼
┌─────────────────────────────────────────────────────────────────┐
│                    2. 温度扫描循环                                │
│  for T = T_min to T_max step ΔT:                                │
│      ┌──────────────────────────────────────────────────────┐   │
│      │  2.1 最小吉布斯自由能法                               │   │
│      │  min_gibbs_solve(T, p, N_k, c_init) → n_j             │   │
│      │  · 构造线性方程组 A·X = B                             │   │
│      │  · 高斯消元求解                                       │   │
│      │  · 迭代更新组分直到收敛                               │   │
│      └──────────────────────────────────────────────────────┘   │
│      ┌──────────────────────────────────────────────────────┐   │
│      │  2.2 计算产物总焓                                     │   │
│      │  H(T) = Σ n_j × H_j(T)                                │   │
│      └──────────────────────────────────────────────────────┘   │
│      存储 (T, H, n_j) 到扫描点数组                               │
└───────────────────────────┬─────────────────────────────────────┘
                            ▼
┌─────────────────────────────────────────────────────────────────┐
│                    3. 焓守恒插值                                  │
│  找到 H(T) = H_pre 的温度点                                      │
│  · 线性插值得到精确 T_c 和对应组分                               │
└───────────────────────────┬─────────────────────────────────────┘
                            ▼
┌─────────────────────────────────────────────────────────────────┐
│                    4. 热力参数计算                                │
│  calc_thermo_properties()                                        │
│  · 平均分子量、密度                                              │
│  · 定压/定容比热 (平衡 & 冻结)                                   │
│  · 比热比、等熵指数                                              │
│  · 声速、特征速度                                                │
│  · 粘度、导热系数、普朗特数                                      │
└───────────────────────────┬─────────────────────────────────────┘
                            ▼
┌─────────────────────────────────────────────────────────────────┐
│                    5. 喷管计算                                    │
│  nozzle_calc_from_pressure()                                     │
│  · 等熵膨胀：S_c = S_e                                           │
│  · 温度扫描找等熵点                                              │
│  · 计算出口速度、比冲、推力系数                                  │
└───────────────────────────┬─────────────────────────────────────┘
                            ▼
┌─────────────────────────────────────────────────────────────────┐
│                         输出结果                                 │
│  · 燃烧室: T_c, n_j, γ, γ_s, c*, ...                            │
│  · 喷管: T_e, u_e, I_sp, C_F, Ma, ...                           │
└─────────────────────────────────────────────────────────────────┘
```

### 3.4 关键算法实现

#### 3.4.1 高斯消元求解器

```c
int solve_gaussian(int n, double A[N][N], double b[N], double x[N]) {
    // 列主元高斯消元
    for (int k = 0; k < n; k++) {
        // 选主元
        int max_row = k;
        for (int i = k+1; i < n; i++)
            if (fabs(A[i][k]) > fabs(A[max_row][k])) max_row = i;
        
        // 交换行
        swap_rows(A, b, k, max_row);
        
        // 消元
        for (int i = k+1; i < n; i++) {
            double factor = A[i][k] / A[k][k];
            for (int j = k; j < n; j++)
                A[i][j] -= factor * A[k][j];
            b[i] -= factor * b[k];
        }
    }
    
    // 回代
    for (int i = n-1; i >= 0; i--) {
        x[i] = b[i];
        for (int j = i+1; j < n; j++)
            x[i] -= A[i][j] * x[j];
        x[i] /= A[i][i];
    }
    return 0;
}
```

#### 3.4.2 偏微分计算

用于计算平衡比热和等熵指数的关键偏导数：

$$
\left(\frac{\partial \ln v}{\partial \ln T}\right)_p = 1 + \left(\frac{\partial \ln n_g}{\partial \ln T}\right)_p
$$

$$
\left(\frac{\partial \ln v}{\partial \ln p}\right)_T = -1 + \left(\frac{\partial \ln n_g}{\partial \ln p}\right)_T
$$

通过求解线性方程组（公式 28）获得，矩阵右端向量不同：
- 对温度求导：$B_j = H_j/(R_0 T)$
- 对压强求导：$B_j = -1$（气相）或 $0$（凝相）

---

## 4. 误差分析与验证

### 4.1 误差来源

| 误差类型 | 来源 | 影响 | 控制方法 |
|----------|------|------|----------|
| 热力数据误差 | NASA 9 系数拟合精度 | <0.1% | 使用权威数据源 |
| 数值截断误差 | 温度扫描步长 | <1 K | 步长 25-50 K + 插值 |
| 迭代收敛误差 | 组分求解收敛判据 | <0.01% | 相对误差 < 10⁻⁶ |
| 模型简化误差 | 理想气体假设 | 1-5% | 高压修正（未实现） |

### 4.2 与 RPA 软件对比验证

以 RS-25 发动机（LOX/LH2, O/F=6.0, p_c=202 atm）为例：

| 参数 | 本项目 | RPA 软件 | 相对误差 |
|------|--------|----------|----------|
| 燃烧温度 T_c | 3589 K | 3588 K | 0.03% |
| 比热比 γ | 1.141 | 1.140 | 0.09% |
| 等熵指数 γ_s | 1.133 | 1.132 | 0.09% |
| 特征速度 c* | 2361 m/s | 2360 m/s | 0.04% |
| 真空比冲 I_sp,vac | 452 s | 452 s | ~0% |

**结论**：本程序计算精度与专业软件 RPA 相当，误差均在 0.1% 以内。

### 4.3 NASA 9 系数数据验证

热力数据来源于 NASA Glenn Research Center 的 CEA (Chemical Equilibrium with Applications) 数据库，通过 RPA 软件的 thermo.inp 文件获取。

**验证方法**：与 NIST-JANAF 热化学表对比

| 物种 | 属性 | 本项目 | NIST 标准值 | 误差 |
|------|------|--------|-------------|------|
| H₂O | Δ_f H°(298K) | -241.826 kJ/mol | -241.826 kJ/mol | 0% |
| H₂O | S°(298K) | 188.84 J/(mol·K) | 188.84 J/(mol·K) | 0% |
| H₂ | S°(298K) | 130.68 J/(mol·K) | 130.68 J/(mol·K) | 0% |
| O₂ | S°(298K) | 205.15 J/(mol·K) | 205.15 J/(mol·K) | 0% |

数据来源引用：
- H₂O: Cox,1989; Woolley,1987; TRC(10/88)
- H₂: Gurvich,1978
- O₂: Gurvich,1989
- OH: Ruscic,2002; Gurvich,1978

---

## 5. 创新点与优化策略

### 5.1 创新点

#### 5.1.1 NASA 9 系数格式

本项目采用 **NASA 9 系数多项式**，相比传统 7 系数格式具有以下优势：

| 特性 | 7 系数 | NASA 9 |
|------|--------|--------|
| 适用温度范围 | 300-1000K 或 1000-5000K | 200-6000K (甚至 20000K) |
| 高温稳定性 | 差（$T^5$ 项爆炸） | 好（负幂次项修正） |
| 数据来源 | JANAF 表 | NASA CEA/Glenn |
| 分段数 | 通常 2 段 | 2-3 段 |

#### 5.1.2 模块化 C 语言核心

- 热力计算核心使用**纯 C 语言**实现，无第三方依赖
- 可独立编译为库，方便嵌入其他系统
- GUI 使用 Qt C++ 封装，实现界面与算法分离

#### 5.1.3 JSON 预设配置

- 发动机参数以 **JSON 文件**存储，易于扩展和编辑
- 支持导入/导出预设配置
- 热力数据可通过 GUI 界面修改

#### 5.1.4 多平台 CI/CD

- GitHub Actions 自动构建
- 支持 macOS (arm64/x64)、Windows (x64)、Linux (x64)
- 自动发布 DMG/ZIP/AppImage 安装包

### 5.2 优化策略

#### 5.2.1 温度扫描优化

```c
/* 自适应步长 */
if (max_change > 0.1)
    T_step *= 0.5;  // 变化大时减小步长
else if (max_change < 0.01 && T_step < DEFAULT_T_STEP)
    T_step *= 1.5;  // 变化小时增大步长
```

#### 5.2.2 迭代初值选择

根据推进剂类型智能选择组分初值：

```c
if (engine_type == ENGINE_RS25) {
    /* LOX/LH2: H, H2, H2O, O, O2, OH */
    c_init[0] = 1e-3;   /* H - 微量 */
    c_init[1] = 10.0;   /* H2 - 过量燃料 */
    c_init[2] = 30.0;   /* H2O - 主产物 */
    c_init[3] = 1e-4;   /* O */
    c_init[4] = 1e-3;   /* O2 */
    c_init[5] = 1.0;    /* OH */
} else if (engine_type == ENGINE_RAPTOR) {
    /* LOX/CH4: H2O, H2, OH, H, CO2, CO, O2, O */
    c_init[0] = 35.6;   /* H2O */
    c_init[4] = 5.6;    /* CO2 */
    c_init[5] = 6.0;    /* CO */
    /* ... */
}
```

#### 5.2.3 矩阵求解稳定性

- 使用**列主元高斯消元**避免数值不稳定
- 对近奇异矩阵进行检测和警告

### 5.3 可能的改进方向

1. **实气效应**：高压下引入压缩因子修正
2. **离解再复合**：考虑喷管冻结点位置
3. **边界层损失**：引入热损失系数
4. **两相流**：支持含铝推进剂的凝相产物
5. **并行计算**：温度扫描可并行化

---

## 6. 使用说明

### 6.1 编译运行

#### macOS/Linux (qmake)

```bash
cd SLS_ThermoCalc
mkdir build && cd build
qmake ..
make -j4
./bin/SLS_ThermoCalc      # Linux
open bin/SLS_ThermoCalc.app  # macOS
```

#### 跨平台 (CMake)

```bash
cd SLS_ThermoCalc
mkdir build && cd build
cmake ..
cmake --build . --config Release
```

### 6.2 界面操作

1. **选择发动机**：从下拉框选择预设或自定义
2. **设置参数**：
   - 燃烧室压强 (MPa)
   - 混合比 O/F
   - 初始焓 (kJ/kg)
   - 出口压强 (atm)
3. **点击计算**：查看燃烧室和喷管结果
4. **导出结果**：保存为文本文件

### 6.3 热力数据编辑

在"热力数据"标签页可以：
- 查看和编辑 NASA 9 系数
- 修改原子组成矩阵
- 调整迭代初值

---

## 7. 结论与展望

### 7.1 主要成果

1. **实现了完整的燃烧室-喷管热力计算流程**
   - 基于最小吉布斯自由能法求解平衡组分
   - 支持 LOX/LH2 和 LOX/CH4 两种推进剂体系

2. **采用 NASA 9 系数格式，确保高温计算准确性**
   - 验证了 7 系数格式在高温下的失效问题
   - 计算结果与理论值一致

3. **开发了跨平台 GUI 应用**
   - Qt 6 现代化界面
   - 支持 macOS/Windows/Linux
   - JSON 预设配置，易于扩展

4. **建立了完整的 CI/CD 流程**
   - GitHub Actions 自动构建和发布
   - 多平台安装包

### 7.2 存在的不足

1. 燃烧温度偏高（未考虑热损失）
2. 仅支持气相平衡产物
3. 输运性质计算采用简化模型

### 7.3 未来展望

1. 引入实气效应和热损失修正
2. 支持含铝固体推进剂
3. 添加喷管型面设计功能
4. 实现三维流场可视化

---

## 8. 参考文献

1. Gordon, S., McBride, B.J., "Computer Program for Calculation of Complex Chemical Equilibrium Compositions and Applications", NASA RP-1311, 1994, 1996.

2. McBride, B.J., Zehe, M.J., Gordon, S., "NASA Glenn Coefficients for Calculating Thermodynamic Properties of Individual Species", NASA/TP-2002-211556, 2002.

3. Sutton, G.P., Biblarz, O., "Rocket Propulsion Elements", 9th Edition, Wiley, 2017.

4. 周至柔, 云峰, "火箭发动机原理", 北京航空航天大学出版社, 2019.

5. RPA - Rocket Propulsion Analysis, http://www.propulsion-analysis.com/

6. NIST-JANAF Thermochemical Tables, 4th Edition, 1998.

---

**附录**：

- [热力计算流程及公式](../references/公式、报告/热力计算流程及公式.tex)
- [RPA 软件参考配置](../references/RPA-4.0.7/examples/)

---

*本报告生成于 2025年12月*
