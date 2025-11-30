# SLS_ThermoCalc 发动机预设文件

本文件夹包含 LOX/LH₂ 液体火箭发动机的预设配置文件，用于热力计算。

## 预设列表

| 文件名 | 发动机 | 燃烧室压力 | 混合比 | 真空比冲 | 来源 |
|--------|--------|-----------|--------|---------|------|
| RS-25_SSME.json | RS-25 (SSME) | 20.64 MPa | 6.03 | 452.3 s | NASA/Wikipedia |
| SSME_40k.json | SSME 40k 测试版 | 20.47 MPa | 6.0 | 453 s | RPA |
| RL-10B2.json | RL10B-2 | 4.36 MPa | 5.88 | 465.5 s | RPA |
| RL10A3-3A.json | RL10A-3-3A | 475 psi | 5.5 | 444 s | RPA |
| Vulcain-2.json | Vulcain 2 | 117.3 bar | 6.7 | 434 s | RPA |
| J-2X.json | J-2X | 9.2 MPa | 5.5 | 448 s | NASA |
| YF-77.json | YF-77 | 10.2 MPa | 5.5 | 430 s | 公开资料 |
| custom_template.json | 自定义模板 | - | - | - | - |

## JSON 格式说明

预设文件格式参考 RPA 软件配置和 `热力计算-燃烧室+喷管-待完善/RS-25in.in` 输入格式：

```json
{
  "version": 1,
  "application": "SLS_ThermoCalc",
  "engineDefinition": {
    "name": "发动机名称",
    "description": "描述",
    "thrust_kN": 推力,
    "specificImpulse_vac_s": 真空比冲
  },
  "combustorConditions": {
    "chamberPressure": {
      "value": 数值,
      "units": "MPa|bar|atm|psi",
      "value_atm": 换算为atm的值
    },
    "mixtureRatio": 混合比,
    "initialEnthalpy_kJ_kg": 初始焓,
    "temperatureScan": {
      "T_min_K": 最低试算温度,
      "T_max_K": 最高试算温度,
      "delta_T_K": 温度步长
    }
  },
  "propellant": {
    "oxidizer": { "name": "O2(L)", "massFraction": 质量分数 },
    "fuel": { "name": "H2(L)", "massFraction": 质量分数 }
  },
  "equilibriumProducts": {
    "numSpecies": 8,
    "species": ["H", "H2", "H2O", "O", "O2", "OH", "HO2", "H2O2"],
    "initialGuess": [迭代初值],
    "Aij_H": [各组分H原子数],
    "Aij_O": [各组分O原子数]
  },
  "nozzleConditions": {
    "exitPressure_atm": 出口压力,
    "areaRatio": 面积比,
    "flowType": "equilibrium|frozen",
    "calculationType": 1或2
  }
}
```

## 关键参数说明

### 初始焓 (initialEnthalpy_kJ_kg)
- 标准值: **-1210.92 kJ/kg** (来自 RS-25in.in)
- 这是液氧液氢推进剂在标准条件下的混合焓
- 计算公式: H_pre = Σ(g_i · H_f,i^0 / M_i)

### 平衡组分迭代初值 (initialGuess)
- 标准值: `[1e-3, 1e-7, 10, 30, 1e-7, 1e-5, 1e-5, 1e-5]`
- 对应: H, H2, H2O, O, O2, OH, HO2, H2O2
- 主要产物 (H2O, H2) 初值较大，次要产物用小量

### 温度扫描范围
- 标准设置: 3000K ~ 4000K, 步长 50K
- LOX/LH2 燃烧温度典型值: 3200K ~ 3600K

### 喷管计算类型 (calculationType)
- **1**: 给定出口压强，使用平衡流动计算
- **2**: 给定出口温度，使用冻结流动计算

## 支持的压力单位

程序自动转换以下单位到 MPa:
- `MPa` - 直接使用
- `bar` - ×0.1
- `atm` - ×0.101325
- `psi` - ×0.00689476

## 添加新预设

1. 复制 `custom_template.json` 为新文件名
2. 修改发动机参数
3. 根据混合比计算质量分数: 
   - oxidizer_fraction = O/F / (1 + O/F)
   - fuel_fraction = 1 / (1 + O/F)
4. 程序启动时会自动加载 presets 文件夹中的所有 .json 文件

## ⚠️ 重要限制

**本程序仅支持 LOX/LH₂ 推进剂组合！**

热力数据库 (`propellant_db.c`) 中只包含以下 6 种平衡组分的 NASA-9 系数：
- H, H2, H2O, O, O2, OH

不支持其他推进剂（如 RP-1/LOX, N₂O₄/UDMH, CH₄/LOX 等）。
