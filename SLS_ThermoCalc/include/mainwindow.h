#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QVector>
#include <QDir>
#include <QFileInfo>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

// 前向声明C结构体
extern "C" {
#include "thermo_core.h"
#include "propellant_db.h"
}

// 预设信息结构体
struct PresetInfo {
    QString name;           // 发动机名称
    QString description;    // 描述
    QString filePath;       // JSON文件路径
    double thrust_kN;       // 推力
    double isp_vac;         // 真空比冲
    double chamberPressure; // 燃烧室压力 MPa
    double mixtureRatio;    // 混合比
};

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
    
    // 热力数据编辑
    void onSpeciesSelectionChanged(int index);
    void onApplySpeciesChange();
    void onResetThermoData();
    void onThermoDataCellChanged(int row, int col);

private:
    Ui::MainWindow *ui;
    
    // 当前配置
    PropellantInput m_currentConfig;
    ThermoResult m_lastResult;
    bool m_hasResult;
    
    // 预设列表
    QVector<PresetInfo> m_presets;
    
    // 初始化界面
    void setupUI();
    void setupConnections();
    void setupEnginePresets();
    
    // 动态加载预设文件
    void loadPresetsFromFolder();
    QStringList getPresetsFolderPaths();
    bool parsePresetInfo(const QString& filePath, PresetInfo& info);
    
    // 自动创建默认预设文件
    QString ensurePresetsFolder();
    void createDefaultPresets(const QString& presetsDir);
    
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
    
    // 热力数据显示和编辑
    void setupThermoDataTab();
    void loadThermoDataToUI();
    void loadSpeciesDataToUI(int speciesIndex);
    void saveSpeciesDataFromUI(int speciesIndex);
    QStringList getSpeciesNames();
    
    // 辅助函数
    QString formatNumber(double value, int precision = 4);
    void setStatusMessage(const QString& msg, bool isError = false);
};

#endif // MAINWINDOW_H
