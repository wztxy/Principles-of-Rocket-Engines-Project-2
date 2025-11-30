# SLS ThermoCalc 预设配置文件

本目录包含火箭发动机预设配置文件，采用 JSON 格式存储。

## 文件列表

| 文件名 | 发动机 | 用途 |
|--------|--------|------|
| `RS-25_SSME.json` | RS-25 (SSME) | SLS 核心级主发动机 |
| `RL-10B2.json` | RL-10B2 | 上面级发动机 |
| `J-2X.json` | J-2X | 探索上面级发动机 |
| `custom_template.json` | 自定义模板 | 用户自定义配置模板 |

## JSON 格式说明

```json
{
  "version": 1,                    // 配置文件版本
  "application": "SLS_ThermoCalc", // 应用程序标识
  
  "engineDefinition": {
    "name": "发动机名称",
    "description": "描述信息",
    "thrust_kN": 推力(kN),
    "specificImpulse_vac_s": 真空比冲(s)
  },
  
  "combustorConditions": {
    "chamberPressure": {
      "value": 压强值,
      "units": "MPa"
    },
    "mixtureRatio": 混合比(O/F),
    "initialEnthalpy_kJ_kg": 初始焓(kJ/kg)
  },
  
  "propellant": {
    "type": "bipropellant",
    "oxidizer": { "name": "O2(L)", "formula": "O2" },
    "fuel": { "name": "H2(L)", "formula": "H2" }
  },
  
  "nozzleConditions": {
    "exitPressure_atm": 出口压强(atm),
    "areaRatio": 面积比,
    "flowType": "equilibrium" 或 "frozen"
  }
}
```

## 使用方法

1. **导入预设**: 菜单 → 文件 → 导入预设，选择 `.json` 文件
2. **导出预设**: 菜单 → 文件 → 导出预设，保存当前配置
3. **修改配置**: 可直接编辑 JSON 文件或在软件中修改后导出

## 参考资料

配置格式参考 RPA (Rocket Propulsion Analysis) 软件的 `.cfg` 格式设计。

## 典型参数范围

| 参数 | RS-25 | RL-10B2 | J-2X |
|------|-------|---------|------|
| 燃烧室压强 (MPa) | 20.64 | 4.36 | 12.4 |
| 混合比 O/F | 6.0 | 5.88 | 5.5 |
| 真空比冲 (s) | 452 | 465 | 448 |
| 面积比 | 77.5 | 285 | 92 |
