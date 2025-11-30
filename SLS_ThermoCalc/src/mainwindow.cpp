#include "mainwindow.h"

#include <QDateTime>
#include <QFileDialog>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QMessageBox>
#include <QTextStream>
#include <cmath>

#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent), ui(new Ui::MainWindow), m_hasResult(false) {
    ui->setupUi(this);

    setupUI();
    setupConnections();
    setupEnginePresets();

    setStatusMessage("就绪 - 选择发动机并设置参数后点击计算");
}

MainWindow::~MainWindow() { delete ui; }

void MainWindow::setupUI() {
    // 设置表格列宽和行高 - 优化显示效果
    ui->tableChamber->setColumnWidth(0, 260);
    ui->tableChamber->setColumnWidth(1, 160);
    ui->tableChamber->horizontalHeader()->setStretchLastSection(true);
    ui->tableChamber->verticalHeader()->setVisible(false);
    ui->tableChamber->verticalHeader()->setDefaultSectionSize(36);

    ui->tableNozzle->setColumnWidth(0, 260);
    ui->tableNozzle->setColumnWidth(1, 160);
    ui->tableNozzle->horizontalHeader()->setStretchLastSection(true);
    ui->tableNozzle->verticalHeader()->setVisible(false);
    ui->tableNozzle->verticalHeader()->setDefaultSectionSize(36);

    ui->tableSpecies->setColumnWidth(0, 100);
    ui->tableSpecies->setColumnWidth(1, 160);
    ui->tableSpecies->setColumnWidth(2, 160);
    ui->tableSpecies->horizontalHeader()->setStretchLastSection(true);
    ui->tableSpecies->verticalHeader()->setVisible(false);
    ui->tableSpecies->verticalHeader()->setDefaultSectionSize(36);

    // 初始化燃烧室结果表 - 简洁格式，单位用括号
    QStringList chamberParams = {"燃烧温度 Tc (K)",
                                 "总焓 H (kJ/kg)",
                                 "总熵 S (kJ/kg/K)",
                                 "平均分子量 M (g/mol)",
                                 "密度 ρ (kg/m³)",
                                 "定压比热 Cp (kJ/kg/K)",
                                 "定容比热 Cv (kJ/kg/K)",
                                 "比热比 γ",
                                 "等熵指数 γs",
                                 "声速 a (m/s)",
                                 "特征速度 c* (m/s)",
                                 "冻结比热 Cp,f (kJ/kg/K)",
                                 "冻结比热比 γf",
                                 "冻结声速 af (m/s)",
                                 "气体常数 R (J/kg/K)",
                                 "粘性系数 μ (Pa·s)",
                                 "导热系数 λ (W/m/K)",
                                 "普朗特数 Pr",
                                 "收敛状态"};

    ui->tableChamber->setRowCount(chamberParams.size());
    for (int i = 0; i < chamberParams.size(); ++i) {
        ui->tableChamber->setItem(i, 0, new QTableWidgetItem(chamberParams[i]));
        ui->tableChamber->setItem(i, 1, new QTableWidgetItem("-"));
    }

    // 初始化喷管结果表 - 简洁格式，单位用括号
    QStringList nozzleParams = {"出口温度 Te (K)",
                                "出口压强 pe (atm)",
                                "出口速度 ve (m/s)",
                                "比冲 Isp (s)",
                                "真空比冲 Isp,vac (s)",
                                "马赫数 Ma",
                                "推力系数 Cf",
                                "质量流系数 Γ",
                                "平均等熵指数 γm",
                                "压力比 pc/pe",
                                "出口密度 ρe (kg/m³)",
                                "出口声速 ae (m/s)",
                                "喉部温度 T* (K)",
                                "喉部压力 p* (atm)",
                                "喉部速度 v* (m/s)",
                                "收敛状态"};

    ui->tableNozzle->setRowCount(nozzleParams.size());
    for (int i = 0; i < nozzleParams.size(); ++i) {
        ui->tableNozzle->setItem(i, 0, new QTableWidgetItem(nozzleParams[i]));
        ui->tableNozzle->setItem(i, 1, new QTableWidgetItem("-"));
    }

    // 初始化组分表 - 支持多种推进剂类型
    // LOX/LH2: H, H2, H2O, O, O2, OH (6种)
    // LOX/CH4: H2O, H2, OH, H, CO2, CO, O2, O (8种)
    QStringList speciesNames = {"H2O", "H2", "OH", "H", "CO2", "CO", "O2", "O"};
    ui->tableSpecies->setRowCount(speciesNames.size());
    for (int i = 0; i < speciesNames.size(); ++i) {
        ui->tableSpecies->setItem(i, 0, new QTableWidgetItem(speciesNames[i]));
        ui->tableSpecies->setItem(i, 1, new QTableWidgetItem("-"));
        ui->tableSpecies->setItem(i, 2, new QTableWidgetItem("-"));
    }

    // 日志初始化
    ui->textLog->append("========================================");
    ui->textLog->append("  SLS火箭发动机热力计算系统");
    ui->textLog->append("  基于最小吉布斯自由能法");
    ui->textLog->append("========================================");
    ui->textLog->append("");
}

void MainWindow::setupConnections() {
    connect(ui->comboEngine, QOverload<int>::of(&QComboBox::currentIndexChanged), this,
            &MainWindow::onEngineChanged);
    connect(ui->btnCalculate, &QPushButton::clicked, this, &MainWindow::onCalculate);
    connect(ui->btnExport, &QPushButton::clicked, this, &MainWindow::onExportResults);
    connect(ui->btnClear, &QPushButton::clicked, this, &MainWindow::onClearResults);
    connect(ui->spinMixtureRatio, QOverload<double>::of(&QDoubleSpinBox::valueChanged), this,
            &MainWindow::onMixtureRatioChanged);
    
    // 菜单操作
    connect(ui->actionImport, &QAction::triggered, this, &MainWindow::onImportPreset);
    connect(ui->actionExport, &QAction::triggered, this, &MainWindow::onExportPreset);
    connect(ui->actionExportResults, &QAction::triggered, this, &MainWindow::onExportResults);
    connect(ui->actionExit, &QAction::triggered, this, &QMainWindow::close);
    connect(ui->actionAbout, &QAction::triggered, this, &MainWindow::onAbout);
    connect(ui->actionAboutQt, &QAction::triggered, this, &MainWindow::onAboutQt);
}

void MainWindow::setupEnginePresets() {
    // 动态加载预设文件
    loadPresetsFromFolder();
    
    // 添加自定义选项
    ui->comboEngine->addItem("── 自定义参数 ──", -1);

    // 默认选择第一个预设
    if (ui->comboEngine->count() > 1) {
        onEngineChanged(0);
    }
}

QStringList MainWindow::getPresetsFolderPaths() {
    QStringList paths;
    
    // 可能的预设文件夹路径
    QString appDir = QCoreApplication::applicationDirPath();
    
    // macOS: app/Contents/MacOS -> 向上三级到 SLS_ThermoCalc
    paths << appDir + "/../../../presets";
    paths << appDir + "/../../presets";
    paths << appDir + "/../presets";
    paths << appDir + "/presets";
    
    // Windows: 直接在可执行文件旁边
    paths << appDir + "/presets";
    
    // 开发时的路径
    paths << QDir::currentPath() + "/presets";
    paths << QDir::currentPath() + "/../presets";
    
    return paths;
}

void MainWindow::loadPresetsFromFolder() {
    m_presets.clear();
    ui->comboEngine->clear();
    
    QStringList searchPaths = getPresetsFolderPaths();
    QString presetsDir;
    
    // 查找存在的预设文件夹
    for (const QString& path : searchPaths) {
        QDir dir(path);
        if (dir.exists()) {
            presetsDir = dir.absolutePath();
            break;
        }
    }
    
    if (presetsDir.isEmpty()) {
        ui->textLog->append("[警告] 未找到预设文件夹，使用默认配置");
        // 添加默认的内置预设
        ui->comboEngine->addItem("RS-25 (SSME) - SLS核心级", ENGINE_RS25);
        ui->comboEngine->addItem("RL-10B2 - 上面级", ENGINE_RL10);
        ui->comboEngine->addItem("J-2X - 探索上面级", ENGINE_J2X);
        return;
    }
    
    ui->textLog->append(QString("[信息] 从 %1 加载预设...").arg(presetsDir));
    
    // 获取所有 JSON 文件
    QDir dir(presetsDir);
    QStringList filters;
    filters << "*.json";
    QFileInfoList files = dir.entryInfoList(filters, QDir::Files, QDir::Name);
    
    int loadedCount = 0;
    for (const QFileInfo& fileInfo : files) {
        // 跳过模板和自述文件
        if (fileInfo.fileName().contains("template", Qt::CaseInsensitive) ||
            fileInfo.fileName().contains("README", Qt::CaseInsensitive)) {
            continue;
        }
        
        PresetInfo info;
        if (parsePresetInfo(fileInfo.absoluteFilePath(), info)) {
            m_presets.append(info);
            
            // 添加到下拉框，显示名称和简要信息
            QString displayText = QString("%1 (%2 MPa, O/F=%3)")
                                     .arg(info.name)
                                     .arg(info.chamberPressure, 0, 'f', 1)
                                     .arg(info.mixtureRatio, 0, 'f', 2);
            ui->comboEngine->addItem(displayText, m_presets.size() - 1);
            loadedCount++;
        }
    }
    
    ui->textLog->append(QString("[信息] 已加载 %1 个发动机预设").arg(loadedCount));
}

bool MainWindow::parsePresetInfo(const QString& filePath, PresetInfo& info) {
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly)) {
        return false;
    }
    
    QByteArray data = file.readAll();
    file.close();
    
    QJsonParseError error;
    QJsonDocument doc = QJsonDocument::fromJson(data, &error);
    if (error.error != QJsonParseError::NoError) {
        return false;
    }
    
    QJsonObject root = doc.object();
    
    // 解析发动机定义
    QJsonObject engineDef = root["engineDefinition"].toObject();
    info.name = engineDef["name"].toString();
    info.description = engineDef["description"].toString();
    info.thrust_kN = engineDef["thrust_kN"].toDouble();
    info.isp_vac = engineDef["specificImpulse_vac_s"].toDouble();
    info.filePath = filePath;
    
    // 解析燃烧室条件
    QJsonObject combustor = root["combustorConditions"].toObject();
    QJsonObject chamberPressure = combustor["chamberPressure"].toObject();
    info.chamberPressure = chamberPressure["value"].toDouble();
    info.mixtureRatio = combustor["mixtureRatio"].toDouble();
    
    return !info.name.isEmpty();
}

void MainWindow::onEngineChanged(int index) {
    int presetIndex = ui->comboEngine->itemData(index).toInt();
    
    // 检查是否是"自定义参数"
    if (presetIndex < 0 || presetIndex >= m_presets.size()) {
        // 自定义参数模式，不更改当前配置
        ui->lblEngineInfo->setText("自定义参数模式\n手动设置所有参数");
        ui->textLog->append(QString("[%1] 选择: 自定义参数模式")
                               .arg(QDateTime::currentDateTime().toString("hh:mm:ss")));
        return;
    }
    
    const PresetInfo& preset = m_presets[presetIndex];
    
    // 加载预设文件
    if (loadPresetFromJson(preset.filePath)) {
        // 更新发动机信息显示
        QString info = QString("%1\n推力: %2 kN, 真空比冲: %3 s")
                           .arg(preset.description)
                           .arg(preset.thrust_kN, 0, 'f', 1)
                           .arg(preset.isp_vac, 0, 'f', 1);
        ui->lblEngineInfo->setText(info);
        
        // 记录日志
        ui->textLog->append(QString("[%1] 选择发动机: %2")
                                .arg(QDateTime::currentDateTime().toString("hh:mm:ss"))
                                .arg(preset.name));
    } else {
        // 加载失败，使用默认配置
        QMessageBox::warning(this, "加载失败", 
                            QString("无法加载预设文件: %1").arg(preset.filePath));
    }
}

void MainWindow::onMixtureRatioChanged(double value) {
    double ox_frac = value / (1.0 + value);
    double fuel_frac = 1.0 / (1.0 + value);

    ui->editOxidizerFrac->setText(formatNumber(ox_frac));
    ui->editFuelFrac->setText(formatNumber(fuel_frac));

    // 更新配置
    m_currentConfig.mass_fraction[0] = fuel_frac;  // H2
    m_currentConfig.mass_fraction[1] = ox_frac;    // O2
}

void MainWindow::readInputParameters() {
    // 燃烧室压强 (MPa -> atm)
    double pc_mpa = ui->spinChamberPressure->value();
    m_currentConfig.chamber_pressure = pc_mpa / 0.101325;  // 1 atm = 0.101325 MPa

    // 初始焓 (kJ/kg -> J/kg)
    m_currentConfig.initial_enthalpy = ui->spinInitialEnthalpy->value() * 1000.0;
}

void MainWindow::onCalculate() {
    setStatusMessage("正在计算...");
    ui->btnCalculate->setEnabled(false);

    // 读取参数
    readInputParameters();

    // 记录开始
    ui->textLog->append("");
    ui->textLog->append(QString("[%1] ========== 开始计算 ==========")
                            .arg(QDateTime::currentDateTime().toString("hh:mm:ss")));
    ui->textLog->append(QString("  燃烧室压强: %1 MPa (%2 atm)")
                            .arg(ui->spinChamberPressure->value(), 0, 'f', 2)
                            .arg(m_currentConfig.chamber_pressure, 0, 'f', 1));
    ui->textLog->append(QString("  混合比 O/F: %1").arg(ui->spinMixtureRatio->value(), 0, 'f', 2));

    // 执行计算
    double exit_pressure = ui->spinExitPressure->value();
    int ret = thermo_calculate(&m_currentConfig, exit_pressure, &m_lastResult);

    if (ret == 0 && m_lastResult.success) {
        m_hasResult = true;
        displayResults(m_lastResult);
        setStatusMessage("计算完成");

        ui->textLog->append(
            QString("  ✓ 燃烧温度: %1 K").arg(m_lastResult.chamber.temperature, 0, 'f', 1));
        ui->textLog->append(
            QString("  ✓ 比冲: %1 s").arg(m_lastResult.nozzle.specific_impulse, 0, 'f', 1));
        ui->textLog->append("[计算成功]");
    } else {
        m_hasResult = false;
        QString errMsg = QString::fromUtf8(m_lastResult.error_msg);
        if (errMsg.isEmpty())
            errMsg = "未知错误";

        setStatusMessage("计算失败: " + errMsg, true);
        ui->textLog->append(QString("  ✗ 错误: %1").arg(errMsg));

        QMessageBox::warning(this, "计算失败", errMsg);
    }

    ui->btnCalculate->setEnabled(true);
}

void MainWindow::displayResults(const ThermoResult& result) {
    displayChamberResults(result.chamber);
    displayNozzleResults(result.nozzle);

    // 更新组分表 - 根据推进剂类型显示不同组分名称
    QStringList speciesNames;
    if (m_currentConfig.num_elements == 3) {
        // LOX/CH4: H2O, H2, OH, H, CO2, CO, O2, O
        speciesNames = {"H₂O", "H₂", "OH", "H", "CO₂", "CO", "O₂", "O"};
    } else {
        // LOX/LH2: H, H2, H2O, O, O2, OH
        speciesNames = {"H", "H₂", "H₂O", "O", "O₂", "OH", "-", "-"};
    }
    
    int numSpecies = qMin(m_currentConfig.num_species, 8);
    
    // 更新表格行数
    ui->tableSpecies->setRowCount(numSpecies);

    for (int i = 0; i < numSpecies; ++i) {
        if (!ui->tableSpecies->item(i, 0))
            ui->tableSpecies->setItem(i, 0, new QTableWidgetItem());
        if (!ui->tableSpecies->item(i, 1))
            ui->tableSpecies->setItem(i, 1, new QTableWidgetItem());
        if (!ui->tableSpecies->item(i, 2))
            ui->tableSpecies->setItem(i, 2, new QTableWidgetItem());
            
        ui->tableSpecies->item(i, 0)->setText(speciesNames[i]);
        ui->tableSpecies->item(i, 1)->setText(formatNumber(result.chamber.mole_fractions[i], 6));
        ui->tableSpecies->item(i, 2)->setText(formatNumber(result.nozzle.mole_fractions[i], 6));
    }
}

void MainWindow::displayChamberResults(const ChamberResult& chamber) {
    int row = 0;
    ui->tableChamber->item(row++, 1)->setText(formatNumber(chamber.temperature, 2));
    ui->tableChamber->item(row++, 1)->setText(formatNumber(chamber.total_enthalpy / 1000.0, 2));
    ui->tableChamber->item(row++, 1)->setText(formatNumber(chamber.total_entropy / 1000.0, 4));
    ui->tableChamber->item(row++, 1)->setText(formatNumber(chamber.mean_molecular_weight, 3));
    ui->tableChamber->item(row++, 1)->setText(formatNumber(chamber.density, 3));
    ui->tableChamber->item(row++, 1)->setText(formatNumber(chamber.cp / 1000.0, 4));
    ui->tableChamber->item(row++, 1)->setText(formatNumber(chamber.cv / 1000.0, 4));
    ui->tableChamber->item(row++, 1)->setText(formatNumber(chamber.gamma, 4));
    ui->tableChamber->item(row++, 1)->setText(formatNumber(chamber.gamma_s, 4));
    ui->tableChamber->item(row++, 1)->setText(formatNumber(chamber.sound_speed, 1));
    ui->tableChamber->item(row++, 1)->setText(formatNumber(chamber.char_velocity, 1));
    ui->tableChamber->item(row++, 1)->setText(formatNumber(chamber.cp_frozen / 1000.0, 4));
    ui->tableChamber->item(row++, 1)->setText(formatNumber(chamber.gamma_frozen, 4));
    ui->tableChamber->item(row++, 1)->setText(formatNumber(chamber.sound_speed_frozen, 1));
    ui->tableChamber->item(row++, 1)->setText(formatNumber(chamber.R_specific, 2));
    ui->tableChamber->item(row++, 1)->setText(formatNumber(chamber.viscosity * 1e6, 4) + " ×10⁻⁶");
    ui->tableChamber->item(row++, 1)->setText(formatNumber(chamber.conductivity, 4));
    ui->tableChamber->item(row++, 1)->setText(formatNumber(chamber.prandtl, 4));
    ui->tableChamber->item(row++, 1)->setText(chamber.converged ? "✓ 收敛" : "✗ 未收敛");
}

void MainWindow::displayNozzleResults(const NozzleResult& nozzle) {
    int row = 0;
    ui->tableNozzle->item(row++, 1)->setText(formatNumber(nozzle.exit_temperature, 2));
    ui->tableNozzle->item(row++, 1)->setText(formatNumber(nozzle.exit_pressure, 4));
    ui->tableNozzle->item(row++, 1)->setText(formatNumber(nozzle.exit_velocity, 1));
    ui->tableNozzle->item(row++, 1)->setText(formatNumber(nozzle.specific_impulse, 1));
    ui->tableNozzle->item(row++, 1)->setText(formatNumber(nozzle.specific_impulse_vac, 1));
    ui->tableNozzle->item(row++, 1)->setText(formatNumber(nozzle.mach_number, 2));
    ui->tableNozzle->item(row++, 1)->setText(formatNumber(nozzle.thrust_coefficient, 4));
    ui->tableNozzle->item(row++, 1)->setText(formatNumber(nozzle.mass_flow_coefficient, 4));
    ui->tableNozzle->item(row++, 1)->setText(formatNumber(nozzle.mean_gamma, 4));
    ui->tableNozzle->item(row++, 1)->setText(formatNumber(nozzle.pressure_ratio, 2));
    ui->tableNozzle->item(row++, 1)->setText(formatNumber(nozzle.exit_density, 4));
    ui->tableNozzle->item(row++, 1)->setText(formatNumber(nozzle.exit_sound_speed, 1));
    ui->tableNozzle->item(row++, 1)->setText(formatNumber(nozzle.throat_temperature, 2));
    ui->tableNozzle->item(row++, 1)->setText(formatNumber(nozzle.throat_pressure, 4));
    ui->tableNozzle->item(row++, 1)->setText(formatNumber(nozzle.throat_velocity, 1));
    ui->tableNozzle->item(row++, 1)->setText(nozzle.converged ? "✓ 收敛" : "✗ 未收敛");
}

void MainWindow::onExportResults() {
    if (!m_hasResult) {
        QMessageBox::information(this, "提示", "请先进行计算");
        return;
    }

    QString filename = QFileDialog::getSaveFileName(this, "导出结果", "结果.txt",
                                                    "文本文件 (*.txt);;CSV文件 (*.csv)");
    if (filename.isEmpty())
        return;

    QFile file(filename);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QMessageBox::warning(this, "错误", "无法创建文件");
        return;
    }

    QTextStream out(&file);

    out << "========================================\n";
    out << "SLS火箭发动机热力计算结果\n";
    out << "计算时间: " << QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss") << "\n";
    out << "========================================\n\n";

    out << "【输入参数】\n";
    out << QString("发动机: %1\n").arg(ui->comboEngine->currentText());
    out << QString("混合比 O/F: %1\n").arg(ui->spinMixtureRatio->value());
    out << QString("燃烧室压强: %1 MPa\n").arg(ui->spinChamberPressure->value());
    out << QString("喷管出口压强: %1 atm\n\n").arg(ui->spinExitPressure->value());

    out << "【燃烧室热力参数】\n";
    out << QString("燃烧温度 Tc: %1 K\n").arg(m_lastResult.chamber.temperature, 0, 'f', 2);
    out << QString("总焓 H: %1 kJ/mol\n").arg(m_lastResult.chamber.total_enthalpy / 1000.0, 0, 'f', 3);
    out << QString("总熵 S: %1 kJ/mol/K\n").arg(m_lastResult.chamber.total_entropy / 1000.0, 0, 'f', 4);
    out << QString("平均分子量 M: %1 g/mol\n").arg(m_lastResult.chamber.mean_molecular_weight, 0, 'f', 3);
    out << QString("密度 ρ: %1 kg/m³\n").arg(m_lastResult.chamber.density, 0, 'f', 3);
    out << QString("平衡定压比热 cp: %1 kJ/mol/K\n").arg(m_lastResult.chamber.cp / 1000.0, 0, 'f', 4);
    out << QString("平衡定容比热 cv: %1 kJ/mol/K\n").arg(m_lastResult.chamber.cv / 1000.0, 0, 'f', 4);
    out << QString("比热比 γ: %1\n").arg(m_lastResult.chamber.gamma, 0, 'f', 4);
    out << QString("等熵指数 γs: %1\n").arg(m_lastResult.chamber.gamma_s, 0, 'f', 4);
    out << QString("平衡声速 as: %1 m/s\n").arg(m_lastResult.chamber.sound_speed, 0, 'f', 1);
    out << QString("特征速度 c*: %1 m/s\n").arg(m_lastResult.chamber.char_velocity, 0, 'f', 1);
    out << QString("冻结定压比热 cp,f: %1 kJ/mol/K\n").arg(m_lastResult.chamber.cp_frozen / 1000.0, 0, 'f', 4);
    out << QString("冻结比热比 γf: %1\n").arg(m_lastResult.chamber.gamma_frozen, 0, 'f', 4);
    out << QString("冻结声速 as,f: %1 m/s\n").arg(m_lastResult.chamber.sound_speed_frozen, 0, 'f', 1);
    out << QString("等价气体常数 R: %1 J/kg/K\n").arg(m_lastResult.chamber.R_specific, 0, 'f', 2);
    out << QString("粘性系数 μ: %1 Pa·s\n").arg(m_lastResult.chamber.viscosity, 0, 'e', 4);
    out << QString("导热系数 λ: %1 W/m/K\n").arg(m_lastResult.chamber.conductivity, 0, 'f', 4);
    out << QString("普朗特数 Pr: %1\n\n").arg(m_lastResult.chamber.prandtl, 0, 'f', 4);

    out << "【喷管性能参数】\n";
    out << QString("出口温度 Te: %1 K\n").arg(m_lastResult.nozzle.exit_temperature, 0, 'f', 2);
    out << QString("出口压力 pe: %1 atm\n").arg(m_lastResult.nozzle.exit_pressure, 0, 'f', 4);
    out << QString("出口速度 ue: %1 m/s\n").arg(m_lastResult.nozzle.exit_velocity, 0, 'f', 1);
    out << QString("比冲 Isp: %1 s\n").arg(m_lastResult.nozzle.specific_impulse, 0, 'f', 1);
    out << QString("真空比冲 Isp,vac: %1 s\n").arg(m_lastResult.nozzle.specific_impulse_vac, 0, 'f', 1);
    out << QString("马赫数 Me: %1\n").arg(m_lastResult.nozzle.mach_number, 0, 'f', 2);
    out << QString("推力系数 CF: %1\n").arg(m_lastResult.nozzle.thrust_coefficient, 0, 'f', 4);
    out << QString("质量流系数 Γ: %1\n").arg(m_lastResult.nozzle.mass_flow_coefficient, 0, 'f', 4);
    out << QString("平均等熵指数 γm: %1\n").arg(m_lastResult.nozzle.mean_gamma, 0, 'f', 4);
    out << QString("压力比 pc/pe: %1\n").arg(m_lastResult.nozzle.pressure_ratio, 0, 'f', 2);
    out << QString("出口密度 ρe: %1 kg/m³\n").arg(m_lastResult.nozzle.exit_density, 0, 'f', 4);
    out << QString("出口声速 ae: %1 m/s\n").arg(m_lastResult.nozzle.exit_sound_speed, 0, 'f', 1);
    out << QString("喉部温度 T*: %1 K\n").arg(m_lastResult.nozzle.throat_temperature, 0, 'f', 2);
    out << QString("喉部压力 p*: %1 atm\n").arg(m_lastResult.nozzle.throat_pressure, 0, 'f', 4);
    out << QString("喉部速度 u*: %1 m/s\n\n").arg(m_lastResult.nozzle.throat_velocity, 0, 'f', 1);
    
    out << "【平衡组分】\n";
    QStringList speciesNames;
    if (m_currentConfig.num_elements == 3) {
        speciesNames = {"H2O", "H2", "OH", "H", "CO2", "CO", "O2", "O"};
    } else {
        speciesNames = {"H", "H2", "H2O", "O", "O2", "OH"};
    }
    int numExportSpecies = qMin(m_currentConfig.num_species, speciesNames.size());
    for (int i = 0; i < numExportSpecies; ++i) {
        out << QString("%1: 燃烧室 %2, 出口 %3\n")
               .arg(speciesNames[i], -6)
               .arg(m_lastResult.chamber.mole_fractions[i], 0, 'e', 4)
               .arg(m_lastResult.nozzle.mole_fractions[i], 0, 'e', 4);
    }

    file.close();

    setStatusMessage(QString("结果已导出到 %1").arg(filename));
    ui->textLog->append(QString("[%1] 结果已导出: %2")
                            .arg(QDateTime::currentDateTime().toString("hh:mm:ss"))
                            .arg(filename));
}

void MainWindow::onClearResults() {
    m_hasResult = false;

    // 清空表格
    for (int i = 0; i < ui->tableChamber->rowCount(); ++i) {
        ui->tableChamber->item(i, 1)->setText("-");
    }
    for (int i = 0; i < ui->tableNozzle->rowCount(); ++i) {
        ui->tableNozzle->item(i, 1)->setText("-");
    }
    for (int i = 0; i < ui->tableSpecies->rowCount(); ++i) {
        ui->tableSpecies->item(i, 1)->setText("-");
        ui->tableSpecies->item(i, 2)->setText("-");
    }

    setStatusMessage("结果已清空");
}

void MainWindow::onAbout() {
    QMessageBox aboutBox(this);
    aboutBox.setWindowTitle("关于 SLS ThermoCalc");
    aboutBox.setIconPixmap(QPixmap(":/icons/rocket.svg").scaled(64, 64, Qt::KeepAspectRatio, Qt::SmoothTransformation));
    
    QString aboutText = 
        "<h2 style='margin-bottom: 10px;'>SLS ThermoCalc</h2>"
        "<p>版本 1.1.0</p>"
        "<p>SLS 火箭发动机热力计算系统</p>"
        "<p style='margin-top: 15px;'>"
        "基于<b>最小吉布斯自由能法</b>实现燃烧室平衡组分计算和喷管等熵膨胀分析。"
        "采用 NASA CEA 9系数多项式进行热力学性质计算。</p>"
        "<p style='margin-top: 15px;'><b>支持推进剂类型:</b></p>"
        "<ul>"
        "<li>LOX/LH2 - 液氧/液氢 (6种产物)</li>"
        "<li>LOX/CH4 - 液氧/甲烷 (8种产物)</li>"
        "</ul>"
        "<p><b>支持发动机:</b></p>"
        "<ul>"
        "<li>RS-25 (SSME) - SLS 核心级 [LOX/LH2]</li>"
        "<li>RL-10B2 - 上面级 [LOX/LH2]</li>"
        "<li>J-2X - 探索上面级 [LOX/LH2]</li>"
        "<li>SpaceX Raptor - 全流量分级燃烧 [LOX/CH4]</li>"
        "</ul>"
        "<p style='margin-top: 15px;'>"
        "Copyright © 2025. 火箭发动机原理课程大作业。</p>"
        "<p>本程序基于 Qt 框架开发，采用 C/C++ 语言编写。</p>";
    
    aboutBox.setText(aboutText);
    aboutBox.setStandardButtons(QMessageBox::Ok);
    aboutBox.exec();
}

QString MainWindow::formatNumber(double value, int precision) {
    if (std::isnan(value) || std::isinf(value)) {
        return "-";
    }
    return QString::number(value, 'f', precision);
}

void MainWindow::setStatusMessage(const QString& msg, bool isError) {
    if (isError) {
        ui->statusbar->setStyleSheet("color: #f38ba8;");
    } else {
        ui->statusbar->setStyleSheet("color: #a6e3a1;");
    }
    ui->statusbar->showMessage(msg);
}

void MainWindow::onAboutQt() {
    QMessageBox::aboutQt(this, "关于 Qt");
}

void MainWindow::onImportPreset() {
    // 默认打开 presets 文件夹
    QString defaultDir = QCoreApplication::applicationDirPath() + "/../../../presets";
    QDir dir(defaultDir);
    if (!dir.exists())
        defaultDir = "";
    
    QString filename = QFileDialog::getOpenFileName(
        this, "导入预设配置", defaultDir,
        "JSON配置文件 (*.json);;所有文件 (*)");
    
    if (filename.isEmpty())
        return;
    
    if (loadPresetFromJson(filename)) {
        setStatusMessage(QString("已导入预设: %1").arg(filename));
        ui->textLog->append(QString("[%1] 导入预设: %2")
                               .arg(QDateTime::currentDateTime().toString("hh:mm:ss"))
                               .arg(filename));
    } else {
        QMessageBox::warning(this, "导入失败", "无法解析配置文件");
    }
}

void MainWindow::onExportPreset() {
    QString filename = QFileDialog::getSaveFileName(
        this, "导出预设配置", "engine_preset.json",
        "JSON配置文件 (*.json)");
    
    if (filename.isEmpty())
        return;
    
    if (savePresetToJson(filename)) {
        setStatusMessage(QString("已导出预设: %1").arg(filename));
        ui->textLog->append(QString("[%1] 导出预设: %2")
                               .arg(QDateTime::currentDateTime().toString("hh:mm:ss"))
                               .arg(filename));
    } else {
        QMessageBox::warning(this, "导出失败", "无法保存配置文件");
    }
}

bool MainWindow::savePresetToJson(const QString& filename) {
    QJsonObject root;
    
    // 版本和应用信息
    root["version"] = 1;
    root["application"] = "SLS_ThermoCalc";
    
    // 发动机信息
    QJsonObject engineDef;
    engineDef["name"] = ui->comboEngine->currentText();
    engineDef["description"] = "SLS ThermoCalc Preset";
    engineDef["thrust_kN"] = 0;
    engineDef["specificImpulse_vac_s"] = m_hasResult ? m_lastResult.nozzle.specific_impulse : 0;
    root["engineDefinition"] = engineDef;
    
    // 燃烧室条件 (RPA风格)
    QJsonObject combustor;
    QJsonObject chamberPressure;
    chamberPressure["value"] = ui->spinChamberPressure->value();
    chamberPressure["units"] = "MPa";
    combustor["chamberPressure"] = chamberPressure;
    combustor["mixtureRatio"] = ui->spinMixtureRatio->value();
    combustor["initialEnthalpy_kJ_kg"] = ui->spinInitialEnthalpy->value();
    root["combustorConditions"] = combustor;
    
    // 推进剂配置
    QJsonObject propellant;
    propellant["type"] = "bipropellant";
    QJsonObject oxidizer;
    oxidizer["name"] = "O2(L)";
    oxidizer["formula"] = "O2";
    oxidizer["massFraction"] = 1.0;
    propellant["oxidizer"] = oxidizer;
    QJsonObject fuel;
    fuel["name"] = "H2(L)";
    fuel["formula"] = "H2";
    fuel["massFraction"] = 1.0;
    propellant["fuel"] = fuel;
    root["propellant"] = propellant;
    
    // 喷管条件
    QJsonObject nozzle;
    nozzle["exitPressure_atm"] = ui->spinExitPressure->value();
    nozzle["flowType"] = "equilibrium";
    root["nozzleConditions"] = nozzle;
    
    // 热力学模型
    QJsonObject thermoModel;
    thermoModel["method"] = "minimumGibbsFreeEnergy";
    thermoModel["coefficients"] = "NASA-9";
    root["thermodynamicModel"] = thermoModel;
    
    // 物种列表
    QJsonArray species;
    species.append("H");
    species.append("H2");
    species.append("H2O");
    species.append("O");
    species.append("O2");
    species.append("OH");
    root["species"] = species;
    
    // 计算结果（如果有）
    if (m_hasResult) {
        QJsonObject results;
        
        QJsonObject chamber;
        chamber["temperature_K"] = m_lastResult.chamber.temperature;
        chamber["gamma"] = m_lastResult.chamber.gamma;
        chamber["gamma_s"] = m_lastResult.chamber.gamma_s;
        chamber["charVelocity_m_s"] = m_lastResult.chamber.char_velocity;
        chamber["soundSpeed_m_s"] = m_lastResult.chamber.sound_speed;
        chamber["meanMolWeight_g_mol"] = m_lastResult.chamber.mean_molecular_weight;
        results["chamber"] = chamber;
        
        QJsonObject nozzleResult;
        nozzleResult["exitTemperature_K"] = m_lastResult.nozzle.exit_temperature;
        nozzleResult["exitVelocity_m_s"] = m_lastResult.nozzle.exit_velocity;
        nozzleResult["specificImpulse_s"] = m_lastResult.nozzle.specific_impulse;
        nozzleResult["machNumber"] = m_lastResult.nozzle.mach_number;
        nozzleResult["thrustCoefficient"] = m_lastResult.nozzle.thrust_coefficient;
        nozzleResult["areaRatio"] = m_lastResult.nozzle.area_ratio;
        results["nozzle"] = nozzleResult;
        
        // 组分
        QJsonArray speciesFractions;
        QStringList speciesNames = {"H", "H2", "H2O", "O", "O2", "OH"};
        for (int i = 0; i < m_currentConfig.num_species && i < 6; ++i) {
            QJsonObject sp;
            sp["name"] = speciesNames[i];
            sp["moleFraction_chamber"] = m_lastResult.chamber.mole_fractions[i];
            sp["moleFraction_nozzle"] = m_lastResult.nozzle.mole_fractions[i];
            speciesFractions.append(sp);
        }
        results["speciesFractions"] = speciesFractions;
        
        root["calculationResults"] = results;
    }
    
    // 写入文件
    QFile file(filename);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
        return false;
    
    QJsonDocument doc(root);
    file.write(doc.toJson(QJsonDocument::Indented));
    file.close();
    
    return true;
}

bool MainWindow::loadPresetFromJson(const QString& filename) {
    QFile file(filename);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return false;
    
    QByteArray data = file.readAll();
    file.close();
    
    QJsonParseError parseError;
    QJsonDocument doc = QJsonDocument::fromJson(data, &parseError);
    if (parseError.error != QJsonParseError::NoError)
        return false;
    
    QJsonObject root = doc.object();
    QString engineName;
    
    // 读取发动机定义
    if (root.contains("engineDefinition")) {
        QJsonObject engineDef = root["engineDefinition"].toObject();
        if (engineDef.contains("name"))
            engineName = engineDef["name"].toString();
    }
    
    // 读取燃烧室条件 (支持两种格式)
    if (root.contains("combustorConditions")) {
        QJsonObject combustor = root["combustorConditions"].toObject();
        
        // 新格式: chamberPressure 是对象
        if (combustor.contains("chamberPressure")) {
            QJsonValue cpVal = combustor["chamberPressure"];
            if (cpVal.isObject()) {
                QJsonObject cpObj = cpVal.toObject();
                double pressure = cpObj["value"].toDouble();
                QString units = cpObj["units"].toString("MPa");
                if (units == "MPa")
                    ui->spinChamberPressure->setValue(pressure);
                else if (units == "atm")
                    ui->spinChamberPressure->setValue(pressure * 0.101325);
                else if (units == "bar")
                    ui->spinChamberPressure->setValue(pressure * 0.1);
                else if (units == "psi")
                    ui->spinChamberPressure->setValue(pressure * 0.00689476);
            } else {
                ui->spinChamberPressure->setValue(cpVal.toDouble());
            }
        }
        // 旧格式兼容
        if (combustor.contains("chamberPressure_MPa"))
            ui->spinChamberPressure->setValue(combustor["chamberPressure_MPa"].toDouble());
        
        if (combustor.contains("mixtureRatio"))
            ui->spinMixtureRatio->setValue(combustor["mixtureRatio"].toDouble());
        
        if (combustor.contains("initialEnthalpy_kJ_kg"))
            ui->spinInitialEnthalpy->setValue(combustor["initialEnthalpy_kJ_kg"].toDouble());
    }
    
    // 读取喷管条件
    if (root.contains("nozzleConditions")) {
        QJsonObject nozzle = root["nozzleConditions"].toObject();
        
        if (nozzle.contains("exitPressure_atm"))
            ui->spinExitPressure->setValue(nozzle["exitPressure_atm"].toDouble());
    }
    
    // 检测推进剂类型并选择适当的配置
    QString propellantType = "LOX_LH2";  // 默认
    
    if (root.contains("propellant")) {
        QJsonObject propellant = root["propellant"].toObject();
        if (propellant.contains("type")) {
            propellantType = propellant["type"].toString();
        }
    }
    
    // 根据推进剂类型选择热力学配置
    if (propellantType == "LOX_CH4" || propellantType == "CH4_LOX") {
        // 使用 LOX/CH4 配置 (Raptor)
        init_lox_ch4_config(&m_currentConfig);
        ui->textLog->append("[信息] 使用 LOX/CH4 推进剂配置 (8种产物)");
    } else {
        // 默认使用 LOX/LH2 配置
        const EnginePreset* defaultPreset = get_engine_preset(ENGINE_RS25);
        if (defaultPreset) {
            m_currentConfig = defaultPreset->config;
        }
        ui->textLog->append("[信息] 使用 LOX/LH2 推进剂配置 (6种产物)");
    }
    
    // 更新质量分数
    onMixtureRatioChanged(ui->spinMixtureRatio->value());
    
    return true;
}
