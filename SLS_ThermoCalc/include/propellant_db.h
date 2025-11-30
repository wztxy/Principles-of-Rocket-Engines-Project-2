/**
 * @file propellant_db.h
 * @brief 推进剂数据库 - SLS发动机预设配置
 */

#ifndef PROPELLANT_DB_H
#define PROPELLANT_DB_H

#ifdef __cplusplus
extern "C" {
#endif

#include "thermo_core.h"

/**
 * @brief 预设发动机类型枚举
 */
typedef enum {
    ENGINE_RS25,            /* RS-25 (SSME) - 液氢/液氧 */
    ENGINE_RL10,            /* RL-10 - 液氢/液氧 */
    ENGINE_J2X,             /* J-2X - 液氢/液氧 */
    ENGINE_CUSTOM           /* 自定义 */
} EngineType;

/**
 * @brief 发动机预设配置结构体
 */
typedef struct {
    const char* name;                   /* 发动机名称 */
    const char* description;            /* 描述 */
    double chamber_pressure;            /* 燃烧室压强 (MPa) */
    double mixture_ratio;               /* 混合比 O/F */
    double thrust;                      /* 推力 (kN) */
    double specific_impulse_vac;        /* 真空比冲 (s) */
    PropellantInput config;             /* 详细配置 */
} EnginePreset;

/**
 * @brief 获取预设发动机配置
 * @param type 发动机类型
 * @return 预设配置指针，NULL表示未找到
 */
const EnginePreset* get_engine_preset(EngineType type);

/**
 * @brief 获取所有预设发动机列表
 * @param count 输出预设数量
 * @return 预设配置数组指针
 */
const EnginePreset* get_all_presets(int* count);

/**
 * @brief 初始化RS-25发动机配置
 * @param input 输出配置
 */
void init_rs25_config(PropellantInput* input);

/**
 * @brief 初始化RL-10发动机配置
 */
void init_rl10_config(PropellantInput* input);

/**
 * @brief 根据混合比计算推进剂质量分数
 * @param mixture_ratio O/F 混合比
 * @param oxidizer_fraction 输出氧化剂质量分数
 * @param fuel_fraction 输出燃料质量分数
 */
void calc_mass_fractions(double mixture_ratio, 
                         double* oxidizer_fraction,
                         double* fuel_fraction);

/**
 * @brief 获取组分初始猜测值（用于迭代）
 * @param engine_type 发动机类型
 * @param c_init 输出初始组分数组
 * @param num_species 组分数
 */
void get_initial_guess(EngineType engine_type, 
                       double c_init[MAX_SPECIES],
                       int num_species);

#ifdef __cplusplus
}
#endif

#endif /* PROPELLANT_DB_H */
