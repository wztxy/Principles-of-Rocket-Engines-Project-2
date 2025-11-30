#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QVector>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

// 前向声明C结构体
extern "C" {
#include "thermo_core.h"
#include "propellant_db.h"
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    // 发动机选择
    void onEngineChanged(int index);
    
    // 计算按钮
    void onCalculate();
    
    // 导出结果
    void onExportResults();
    
    // 清空结果
    void onClearResults();
    
    // 混合比变化
    void onMixtureRatioChanged(double value);
    
    // 关于对话框
    void onAbout();
    
    // 关于Qt对话框
    void onAboutQt();
    
    // 导入预设
    void onImportPreset();
    
    // 导出预设
    void onExportPreset();

private:
    Ui::MainWindow *ui;
    
    // 当前配置
    PropellantInput m_currentConfig;
    ThermoResult m_lastResult;
    bool m_hasResult;
    
    // 初始化界面
    void setupUI();
    void setupConnections();
    void setupEnginePresets();
    
    // 从界面读取参数
    void readInputParameters();
    
    // 显示结果
    void displayResults(const ThermoResult& result);
    void displayChamberResults(const ChamberResult& chamber);
    void displayNozzleResults(const NozzleResult& nozzle);
    
    // 更新图表
    void updateCharts();
    
    // JSON 导入/导出
    bool savePresetToJson(const QString& filename);
    bool loadPresetFromJson(const QString& filename);
    
    // 辅助函数
    QString formatNumber(double value, int precision = 4);
    void setStatusMessage(const QString& msg, bool isError = false);
};

#endif // MAINWINDOW_H
