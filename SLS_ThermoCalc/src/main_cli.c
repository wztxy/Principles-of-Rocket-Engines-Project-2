/**
 * SLS ThermoCalc - 命令行版本
 * 用于 Windows 交叉编译
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "thermo_core.h"
#include "propellant_db.h"

void print_usage(const char* prog) {
    printf("SLS 火箭发动机热力计算器 v1.0\n");
    printf("用法: %s [选项]\n\n", prog);
    printf("选项:\n");
    printf("  -e <engine>   发动机类型: rs25, rl10, j2x (默认: rs25)\n");
    printf("  -p <MPa>      燃烧室压强 (MPa)\n");
    printf("  -r <ratio>    混合比 O/F\n");
    printf("  -x <atm>      喷管出口压强 (atm, 默认: 0.001)\n");
    printf("  -h            显示帮助\n");
    printf("\n示例:\n");
    printf("  %s -e rs25 -p 20.64 -r 6.0 -x 0.001\n", prog);
}

int main(int argc, char* argv[]) {
    // 默认参数
    EngineType engine = ENGINE_RS25;
    double chamber_pressure = 0;  // 0 表示使用预设值
    double mixture_ratio = 0;
    double exit_pressure = 0.001;
    
    // 解析命令行参数
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-h") == 0 || strcmp(argv[i], "--help") == 0) {
            print_usage(argv[0]);
            return 0;
        }
        else if (strcmp(argv[i], "-e") == 0 && i + 1 < argc) {
            i++;
            if (strcmp(argv[i], "rs25") == 0) engine = ENGINE_RS25;
            else if (strcmp(argv[i], "rl10") == 0) engine = ENGINE_RL10;
            else if (strcmp(argv[i], "j2x") == 0) engine = ENGINE_J2X;
            else {
                fprintf(stderr, "未知发动机类型: %s\n", argv[i]);
                return 1;
            }
        }
        else if (strcmp(argv[i], "-p") == 0 && i + 1 < argc) {
            chamber_pressure = atof(argv[++i]);
        }
        else if (strcmp(argv[i], "-r") == 0 && i + 1 < argc) {
            mixture_ratio = atof(argv[++i]);
        }
        else if (strcmp(argv[i], "-x") == 0 && i + 1 < argc) {
            exit_pressure = atof(argv[++i]);
        }
    }
    
    // 获取发动机预设
    const EnginePreset* preset = get_engine_preset(engine);
    if (!preset) {
        fprintf(stderr, "无法获取发动机配置\n");
        return 1;
    }
    
    // 复制配置
    PropellantInput config = preset->config;
    
    // 覆盖用户指定的参数
    if (chamber_pressure > 0) {
        config.chamber_pressure = chamber_pressure / 0.101325;  // MPa -> atm
    }
    if (mixture_ratio > 0) {
        double ox_frac = mixture_ratio / (1.0 + mixture_ratio);
        double fuel_frac = 1.0 / (1.0 + mixture_ratio);
        config.mass_fraction[0] = fuel_frac;
        config.mass_fraction[1] = ox_frac;
    }
    
    // 打印输入参数
    printf("========================================\n");
    printf("  SLS 火箭发动机热力计算器\n");
    printf("  基于最小吉布斯自由能法\n");
    printf("========================================\n\n");
    
    printf("[输入参数]\n");
    printf("  发动机: %s\n", preset->name);
    printf("  燃烧室压强: %.2f MPa (%.1f atm)\n", 
           config.chamber_pressure * 0.101325, config.chamber_pressure);
    printf("  混合比 O/F: %.2f\n", config.mass_fraction[1] / config.mass_fraction[0]);
    printf("  喷管出口压强: %.4f atm\n\n", exit_pressure);
    
    // 执行计算
    ThermoResult result;
    int ret = thermo_calculate(&config, exit_pressure, &result);
    
    if (ret != 0 || !result.success) {
        fprintf(stderr, "计算失败: %s\n", result.error_msg);
        return 1;
    }
    
    // 输出结果
    printf("[燃烧室结果]\n");
    printf("  燃烧温度: %.2f K\n", result.chamber.temperature);
    printf("  平均分子量: %.3f g/mol\n", result.chamber.mean_molecular_weight);
    printf("  比热比 gamma: %.4f\n", result.chamber.gamma);
    printf("  等熵指数 gamma_s: %.4f\n", result.chamber.gamma_s);
    printf("  声速: %.1f m/s\n", result.chamber.sound_speed);
    printf("  特征速度 c*: %.1f m/s\n", result.chamber.char_velocity);
    printf("\n");
    
    printf("[喷管结果]\n");
    printf("  出口温度: %.2f K\n", result.nozzle.exit_temperature);
    printf("  出口速度: %.1f m/s\n", result.nozzle.exit_velocity);
    printf("  比冲 Isp: %.1f s\n", result.nozzle.specific_impulse);
    printf("  马赫数: %.2f\n", result.nozzle.mach_number);
    printf("  推力系数 Cf: %.4f\n", result.nozzle.thrust_coefficient);
    printf("  面积比: %.2f\n", result.nozzle.area_ratio);
    printf("\n");
    
    printf("[平衡组分 (摩尔分数)]\n");
    const char* species_names[] = {"H", "H2", "H2O", "O", "O2", "OH"};
    for (int i = 0; i < 6 && i < config.num_species; i++) {
        printf("  %-4s: %.6f (燃烧室) -> %.6f (喷管出口)\n", 
               species_names[i],
               result.chamber.mole_fractions[i],
               result.nozzle.mole_fractions[i]);
    }
    
    printf("\n[计算完成]\n");
    
    return 0;
}
