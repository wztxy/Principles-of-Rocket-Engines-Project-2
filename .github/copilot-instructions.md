# Rocket Engine Thermodynamics Project - AI Coding Guide

## 项目概述

这是一个火箭发动机原理课程大作业项目，实现美国SLS登月火箭系统发动机的热力性能参数计算。核心目标是根据推进剂组分和燃烧室压强，基于最小吉布斯自由能法求解燃烧室温度、平衡组分及喷管性能参数。

## 项目结构

```
SLS_ThermoCalc/                # 🚀 主程序 - Qt GUI应用（C语言核心 + Qt界面）
references/                    # 📚 参考资料文件夹
├── 程序/                      # 早期开发的独立测试程序（最小吉布斯自由能法原型）
├── 热力计算-燃烧室+喷管-待完善/ # 命令行版热力计算程序（Header-only C++）
├── 公式、报告/                 # LaTeX 报告和公式文档
└── RPA-4.0.7/                 # RPA 软件参考配置和脚本
```

### `SLS_ThermoCalc/` - 主程序（Qt GUI）
```
SLS_ThermoCalc/
├── include/               # C语言头文件
│   ├── thermo_core.h     # 热力计算核心API
│   ├── propellant_db.h   # 推进剂数据库（RS-25/RL-10/Raptor预设）
│   ├── matrix_solver.h   # 高斯消元求解器
│   └── constants.h       # 物理常数
├── src/                   # 源文件
│   ├── thermo_core.c     # 最小吉布斯自由能法实现
│   ├── propellant_db.c   # SLS发动机配置
│   ├── matrix_solver.c   # 矩阵求解
│   ├── mainwindow.cpp    # Qt主窗口
│   └── main.cpp          # 程序入口
├── presets/               # 发动机预设配置
│   ├── RS-25_SSME.json   # RS-25 (初始焓 -1685 kJ/kg)
│   ├── RL-10B2.json      # RL-10B2 (初始焓 -987 kJ/kg)
│   ├── Raptor.json       # SpaceX Raptor
│   └── YF-77.json        # YF-77
└── ui/mainwindow.ui      # Qt Designer界面文件
```

### `references/程序/` vs `references/热力计算-燃烧室+喷管-待完善/` 的区别
两个文件夹使用**相同的核心算法**（最小吉布斯自由能法），区别在于：
- `references/程序/test.cpp` - 早期原型，参数硬编码，仅计算燃烧室平衡组分
- `references/热力计算-燃烧室+喷管-待完善/` - 模块化重构，支持文件输入，包含完整的燃烧室+喷管计算流程

## 核心算法架构

### 燃烧室计算流程 (`combuster.h`)
1. **假定化学式** (`jiading.h`) - 将推进剂质量分数转换为元素摩尔数 `N_k`
2. **最小吉布斯自由能法** - 迭代求解平衡组分，使用高斯消元 (`Gassolve.h`)
3. **温度试算** - 扫描 `T_min` 到 `T_max`，通过焓守恒插值确定燃烧温度

### 喷管计算 (`nozzle.h`)
- **平衡流动** (`From_pe`) - 给定出口压强，等熵膨胀求解
- **冻结流动** (`From_Te`) - 给定温度，组分不变

### 热力参数 (`relichanshu.h`)
使用 NASA 9系数多项式计算：焓 `H_i`、熵 `S_i`、比热 `Cp_i`

## 输入文件格式 (`RS-25in.in`)

```
P M N L                    # 推进剂组元数、元素数、平衡组分数、凝相组分数
St_aij[P×M]                # 组元-元素原子数矩阵
gi[P]                      # 组元质量分数
M_k[M]                     # 元素原子量（从小到大排序）
HS_a[N×9]                  # NASA 9系数热力参数
Aij[M×N]                   # 产物-元素原子数矩阵
p                          # 燃烧室压强 (atm)
T_min T_max delta_T        # 温度扫描范围
c[N]                       # 组分迭代初值
H_pre                      # 推进剂初始焓
id                         # 1=给定出口压强, 2=给定出口温度
pe/Te                      # 出口条件
```

## 关键变量命名约定

| 变量 | 含义 |
|------|------|
| `L` | 凝相组分数（前L个为凝相） |
| `N` | 总平衡组分数 |
| `M` | 元素种类数 |
| `ng` | 气相产物总摩尔数 |
| `HS_a[i][0-8]` | 第i种组分的NASA 9系数 |
| `Aij[k][j]` | 1mol第j种产物含第k种元素的摩尔数 |

## 编译和运行

```bash
cd references/热力计算-燃烧室+喷管-待完善
g++ -std=c++11 -o test_all test_all.cpp
./test_all < RS-25in.in
```

## 常见修改场景

### 添加新推进剂
1. 查表获取 NASA 9系数（`thermo.inp`）
2. 确定平衡产物种类和原子组成矩阵
3. 计算推进剂初始焓 `H_pre`
4. 按 `输入输出说明.md` 格式编写 `.in` 文件

### 调试迭代收敛问题
- 检查 `c[N]` 初值（主产物按化学方程式，次产物用 `1e-7`）
- 调整温度步长 `delta_T`（建议 ≥ 25K）
- 检查矩阵是否奇异（`Gassolve.h` 会抛出异常）

## 公式参考

详细公式推导见 `references/公式、报告/热力计算流程及公式.tex`，包括：
- 最小吉布斯自由能矩阵方程 (Section 4)
- 偏微分求解线性方程组 (公式 28-29)
- 喷管等熵膨胀牛顿法/内插法 (Section 5)

## RPA 软件参考

`references/RPA-4.0.7/examples/` 包含真实发动机配置（如 SSME、RD-170），可用于验证计算结果。脚本示例在 `scripting/` 目录。

## Qt GUI 程序编译运行

```bash
cd SLS_ThermoCalc
mkdir build && cd build
qmake ..
make -j4
./bin/SLS_ThermoCalc
```

或使用 Qt Creator 打开 `SLS_ThermoCalc.pro` 直接运行。

### GUI 界面功能
- 🚀 发动机选择（RS-25/RL-10/Raptor/YF-77/自定义）
- ⚗️ 推进剂参数配置（混合比）
- 🔥 燃烧室计算结果（燃烧室温度、燃烧产物平均摩尔质量、燃烧产物平均密度、产物等价气体常数、平衡声速、比热比等）
- 🎯 喷管计算结果（燃气速度、真空比冲、马赫数、推力系数等）
- 📥 结果导出功能
