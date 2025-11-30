# SLS火箭发动机热力计算系统

基于最小吉布斯自由能法的火箭发动机燃烧室和喷管热力计算程序。

## 项目结构

```
SLS_ThermoCalc/
├── SLS_ThermoCalc.pro    # Qt项目文件
├── include/               # 头文件
│   ├── constants.h       # 物理常数定义
│   ├── matrix_solver.h   # 矩阵求解器
│   ├── thermo_core.h     # 热力计算核心
│   ├── propellant_db.h   # 推进剂数据库
│   └── mainwindow.h      # 主窗口
├── src/                   # 源文件
│   ├── main.cpp          # 程序入口
│   ├── mainwindow.cpp    # 主窗口实现
│   ├── thermo_core.c     # 热力计算核心 (C语言)
│   ├── propellant_db.c   # 推进剂数据库 (C语言)
│   └── matrix_solver.c   # 矩阵求解器 (C语言)
├── ui/                    # UI文件
│   └── mainwindow.ui     # 主窗口界面
└── resources/             # 资源文件
    └── resources.qrc     # 资源配置
```

## 编译运行

### 前置要求
- Qt 5.12+ 或 Qt 6.x
- C++17 编译器
- qmake 或 CMake

### 使用 qmake 编译
```bash
cd SLS_ThermoCalc
mkdir build && cd build
qmake ..
make -j4
./bin/SLS_ThermoCalc
```

### 使用 Qt Creator
1. 打开 `SLS_ThermoCalc.pro`
2. 配置 Kit
3. 点击运行

## 功能特性

### 支持的发动机
- **RS-25 (SSME)**: SLS核心级发动机，液氢/液氧，20.47 MPa
- **RL-10B2**: 上面级发动机，液氢/液氧，4.4 MPa
- **J-2X**: 探索上面级发动机，液氢/液氧，9.2 MPa
- **自定义参数**: 用户可自定义混合比、压强等参数

### 计算内容
1. **燃烧室计算**
   - 平衡组分（最小吉布斯自由能法）
   - 燃烧温度（焓守恒迭代）
   - 热力参数（比热、等熵指数等）

2. **喷管计算**
   - 平衡流动/冻结流动
   - 出口参数（温度、速度、马赫数）
   - 性能参数（比冲、推力系数）

## 核心算法

### 最小吉布斯自由能法
通过迭代求解以下矩阵方程确定平衡组分：

$$
\begin{bmatrix}
R & \alpha & A^T \\
\alpha^T & 0 & 0 \\
A & 0 & 0
\end{bmatrix}
\begin{bmatrix}
\lambda \\ \ln n_g \\ n_c
\end{bmatrix}
=
\begin{bmatrix}
N_k + A\Phi_c \\ \sum\Phi_c \\ -y
\end{bmatrix}
$$

其中：
- $\Phi_c = (-y_i + \ln c_i + \ln p - \ln n_g) \cdot c_i$
- $y_i = -\mu_i / (RT)$ 为无量纲化学势

### NASA 9系数多项式
热力参数使用 NASA 9系数多项式计算（来源：NASA CEA thermo.inp）：

$$
\frac{C_p}{R} = a_1 T^{-2} + a_2 T^{-1} + a_3 + a_4 T + a_5 T^2 + a_6 T^3 + a_7 T^4
$$

$$
\frac{H}{RT} = -a_1 T^{-2} + a_2 \frac{\ln T}{T} + a_3 + \frac{a_4}{2} T + \frac{a_5}{3} T^2 + \frac{a_6}{4} T^3 + \frac{a_7}{5} T^4 + \frac{a_8}{T}
$$

## 验证

计算结果可与 RPA (Rocket Propulsion Analysis) 软件进行对比验证：
- 参考配置: `RPA-4.0.7/examples/SSME 40k.cfg`
- RS-25 预期燃烧温度: ~3500 K
- RS-25 预期真空比冲: ~452 s

## 参考文献

1. 火箭发动机原理课程讲义
2. NASA CEA (Chemical Equilibrium with Applications)
3. `公式、报告/热力计算流程及公式.tex`
