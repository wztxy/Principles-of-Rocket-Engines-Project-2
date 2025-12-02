# SLS ThermoCalc 🚀

火箭发动机热力性能计算程序 | Rocket Engine Thermodynamic Performance Calculator

[![Release](https://img.shields.io/badge/Release-v1.0.0-blue.svg)](https://github.com/wztxy/Principles-of-Rocket-Engines-Project-2/releases)
[![License: CC BY-NC-SA 4.0](https://img.shields.io/badge/License-CC%20BY--NC--SA%204.0-lightgrey.svg)](https://creativecommons.org/licenses/by-nc-sa/4.0/)
[![Qt](https://img.shields.io/badge/Qt-6.x-green.svg)](https://www.qt.io/)
[![Platform](https://img.shields.io/badge/Platform-macOS%20%7C%20Windows%20%7C%20Linux-blue.svg)]()

## 简介

SLS ThermoCalc 是一个基于 Qt 开发的火箭发动机热力性能计算程序，用于计算液体火箭发动机燃烧室和喷管的热力参数。本项目是火箭发动机原理课程大作业，实现了基于**最小吉布斯自由能法**的燃烧平衡计算，采用 **NASA 9 系数多项式**格式计算热力学性质，确保高温（200K-20000K）条件下的计算精度。

### 主要功能

- **燃烧室计算**：基于最小吉布斯自由能法求解化学平衡组分和绝热火焰温度
- **喷管计算**：等熵膨胀过程，计算喷管出口参数和性能指标
- **输运性质**：粘性系数、导热系数、普朗特数（Wilke/Eucken混合规则）
- **多种发动机预设**：RS-25 (SSME)、RL-10B2、J-2X、Raptor、Vulcain-2、YF-77
- **多推进剂支持**：LOX/LH₂（6种产物）、LOX/CH₄（8种产物）
- **完整输出**：比冲、特征速度、推力系数、马赫数、比热比等

## 编译与运行

### 依赖

- Qt 6.x (推荐 6.5+)
- C++17 兼容编译器
- qmake 或 CMake

### macOS / Linux

```bash
cd SLS_ThermoCalc
mkdir -p build && cd build
qmake ..
make -j4
./bin/SLS_ThermoCalc.app/Contents/MacOS/SLS_ThermoCalc  # macOS
./bin/SLS_ThermoCalc  # Linux
```

### Windows

```powershell
cd SLS_ThermoCalc
mkdir build && cd build
qmake ..
mingw32-make  # 或 nmake
.\bin\SLS_ThermoCalc.exe
```

或使用 Qt Creator 打开 `SLS_ThermoCalc.pro` 直接编译运行。

## 项目结构

```
Principles-of-Rocket-Engines-Project-2/
├── SLS_ThermoCalc/           # 主程序 - Qt GUI 应用
│   ├── include/              # C 语言头文件
│   │   ├── thermo_core.h     # 热力计算核心 API
│   │   ├── propellant_db.h   # 推进剂数据库 (NASA 9 系数)
│   │   ├── matrix_solver.h   # 高斯消元求解器
│   │   └── constants.h       # 物理常数
│   ├── src/                  # 源代码
│   │   ├── thermo_core.c     # 最小吉布斯自由能法实现
│   │   ├── propellant_db.c   # NASA 9 系数数据库
│   │   ├── mainwindow.cpp    # Qt 主窗口
│   │   └── ...
│   ├── presets/              # 发动机预设 JSON 文件
│   └── ui/                   # Qt Designer UI 文件
│
├── doc/                      # 项目文档
│   ├── PROJECT_REPORT.md     # 完整项目报告
│   └── PPT_报告内容.md        # PPT 演示内容
│
└── references/               # 参考资料
    ├── 公式、报告/             # LaTeX 报告和公式推导
    ├── 热力计算-燃烧室+喷管/    # 命令行版计算程序原型
    ├── 程序/                  # 早期测试程序
    └── RPA-4.0.7/            # RPA 软件参考配置
```

## 核心算法

### 最小吉布斯自由能法

程序采用最小吉布斯自由能法求解燃烧室化学平衡：

1. **假定化学式**：将推进剂质量分数转换为元素摩尔数
2. **吉布斯自由能最小化**：建立拉格朗日乘子方程组
3. **高斯消元求解**：迭代求解平衡组分
4. **温度扫描 + 焓守恒插值**：确定绝热火焰温度

### NASA 9 系数多项式

使用 NASA 9 系数多项式计算热力参数（焓、熵、比热容），数据来源于 NASA CEA thermo.inp：

- **有效范围**：200K - 20000K（分 2-3 个温度区间）
- **高温稳定性**：采用负幂次项（T⁻², T⁻¹），避免 7 系数格式的高温外推误差
- **数据来源**：Cox,1989 (H₂O); Gurvich,1978 (H₂, O₂); Ruscic,2002 (OH)

详细公式推导见 `doc/PROJECT_REPORT.md` 和 `references/公式、报告/热力计算流程及公式.tex`。

## 发动机预设

| 发动机       | 推进剂  | 燃烧室压强 | 混合比 O/F | 真空比冲 | 推力     |
| ------------ | ------- | ---------- | ---------- | -------- | -------- |
| RS-25 (SSME) | LOX/LH₂ | 20.64 MPa  | 6.03       | 452 s    | 2,279 kN |
| RL-10B2      | LOX/LH₂ | 4.36 MPa   | 5.88       | 466 s    | 110 kN   |
| J-2X         | LOX/LH₂ | 9.2 MPa    | 5.5        | 448 s    | 1,307 kN |
| Raptor       | LOX/CH₄ | 30 MPa     | 3.6        | 363 s    | 2,260 kN |
| Vulcain-2    | LOX/LH₂ | 11.7 MPa   | 6.1        | 434 s    | 1,359 kN |
| YF-77        | LOX/LH₂ | 10.1 MPa   | 5.5        | 430 s    | 700 kN   |

## 计算精度

与专业软件 RPA 对比（RS-25 条件）：

| 参数 | 本程序 | RPA | 误差 |
|------|--------|-----|------|
| 燃烧温度 | 3589 K | 3588 K | 0.03% |
| 特征速度 c* | 2361 m/s | 2360 m/s | 0.04% |
| 真空比冲 | 451 s | 452 s | 0.22% |

## 文档

- [完整项目报告](doc/PROJECT_REPORT.md) - 理论基础、算法实现、误差分析
- [PPT 报告内容](doc/PPT_报告内容.md) - 演示用内容，包含架构图和流程图

## 许可证

本项目采用 [CC BY-NC-SA 4.0](https://creativecommons.org/licenses/by-nc-sa/4.0/) 许可证。

## 致谢

- NASA CEA (Chemical Equilibrium with Applications) 热力数据
- RPA (Rocket Propulsion Analysis) 软件参考
- Qt 开源框架

---

_本项目为北京航空航天大学宇航学院宇航推进系火箭发动机原理课程大作业_

_版本：v1.0.0 | 2025年12月_
