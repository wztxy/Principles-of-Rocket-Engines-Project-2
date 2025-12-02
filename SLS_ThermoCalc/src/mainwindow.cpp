#include "mainwindow.h"

#include <QDateTime>
#include <QFileDialog>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QMessageBox>
#include <QStandardPaths>
#include <QTextStream>
#include <cmath>

#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent), ui(new Ui::MainWindow), m_hasResult(false) {
    ui->setupUi(this);

    setupUI();
    setupConnections();
    setupEnginePresets();

    // 确保热力配置文件存在，首次运行时生成
    ensureThermoConfigExists();
    // 加载用户热力配置
    loadThermoConfigFromJson();

    setupThermoDataTab();

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
    QStringList chamberParams = {"燃烧室温度 Tc (K)",
                                 "总焓 H (kJ/kg)",
                                 "总熵 S (kJ/kg/K)",
                                 "燃烧产物平均摩尔质量 (g/mol)",
                                 "燃烧产物平均密度 (kg/m³)",
                                 "定压比热 Cp (kJ/kg/K)",
                                 "定容比热 Cv (kJ/kg/K)",
                                 "比热比 γ",
                                 "等熵指数 γs",
                                 "平衡声速 (m/s)",
                                 "特征速度 c* (m/s)",
                                 "冻结比热 Cp,f (kJ/kg/K)",
                                 "冻结比热比 γf",
                                 "冻结声速 (m/s)",
                                 "产物等价气体常数 R (J/kg/K)",
                                 "粘性系数 μ (Pa·s)",
                                 "导热系数 λ (W/m/K)",
                                 "普朗特数 Pr"};

    ui->tableChamber->setRowCount(chamberParams.size());
    for (int i = 0; i < chamberParams.size(); ++i) {
        ui->tableChamber->setItem(i, 0, new QTableWidgetItem(chamberParams[i]));
        ui->tableChamber->setItem(i, 1, new QTableWidgetItem("-"));
    }

    // 初始化喷管结果表 - 简洁格式，单位用括号
    QStringList nozzleParams = {"出口温度 Te (K)",        "出口压强 pe (atm)", "燃气速度 ue (m/s)",
                                "真空比冲 Isp,vac (m/s)", "马赫数 Ma",         "推力系数 Cf",
                                "质量流系数 Γ",           "平均等熵指数 γm",   "压力比 pc/pe",
                                "出口密度 ρe (kg/m³)",    "出口声速 ae (m/s)", "喉部温度 T* (K)",
                                "喉部压力 p* (atm)",      "喉部速度 v* (m/s)"};

    ui->tableNozzle->setRowCount(nozzleParams.size());
    for (int i = 0; i < nozzleParams.size(); ++i) {
        ui->tableNozzle->setItem(i, 0, new QTableWidgetItem(nozzleParams[i]));
        ui->tableNozzle->setItem(i, 1, new QTableWidgetItem("-"));
    }

    // 初始化组分表 - 支持多种推进剂类型
    // LOX/LH2: H, H2, H2O, O, O2, OH (6种)
    // LOX/CH4: H2O, H2, OH, H, CO2, CO, O2, O (8种)
    // 使用 Unicode 下标字符
    QStringList speciesNames = {"H₂O", "H₂", "OH", "H", "CO₂", "CO", "O₂", "O"};
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

    // 热力数据编辑
    connect(ui->comboSpeciesSelect, QOverload<int>::of(&QComboBox::currentIndexChanged), this,
            &MainWindow::onSpeciesSelectionChanged);
    connect(ui->btnApplySpecies, &QPushButton::clicked, this, &MainWindow::onApplySpeciesChange);
    connect(ui->btnResetThermoData, &QPushButton::clicked, this, &MainWindow::onResetThermoData);
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

    QString appDir = QCoreApplication::applicationDirPath();

#ifdef Q_OS_MAC
    // === macOS 搜索顺序 ===
    // 1. 应用程序包内部 (打包分发时使用)
    //    SLS_ThermoCalc.app/Contents/Resources/presets
    paths << appDir + "/../Resources/presets";

    // 2. 应用程序包旁边 (与.app同目录)
    paths << appDir + "/../../../../presets";  // bin/xxx.app/Contents/MacOS -> 同级presets
    paths << appDir + "/../../../presets";

    // 3. 用户数据目录 (独立运行时自动创建)
    QString userDataDir = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
    paths << userDataDir + "/presets";

#else
    // === Windows/Linux 搜索顺序 ===
    // 1. 可执行文件同目录
    paths << appDir + "/presets";

    // 2. 用户数据目录 (独立运行时自动创建)
    QString userDataDir = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
    paths << userDataDir + "/presets";
#endif

    // === 开发环境路径 (通用) ===
    paths << QDir::currentPath() + "/presets";
    paths << QDir::currentPath() + "/../presets";

    return paths;
}

QString MainWindow::ensurePresetsFolder() {
    QStringList searchPaths = getPresetsFolderPaths();

    // 首先查找已存在的预设文件夹
    for (const QString& path : searchPaths) {
        QDir dir(path);
        if (dir.exists()) {
            return dir.absolutePath();
        }
    }

    // 没有找到现有预设文件夹，在用户数据目录创建
    // 这样即使是独立的exe文件也能正常工作
    QString userDataDir = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
    QString presetsDir = userDataDir + "/presets";

    QDir dir;
    if (dir.mkpath(presetsDir)) {
        ui->textLog->append(QString("[信息] 创建预设文件夹: %1").arg(presetsDir));
        return presetsDir;
    }

    // 如果用户目录也失败，尝试在可执行文件旁边创建
    QString appDir = QCoreApplication::applicationDirPath();
#ifdef Q_OS_MAC
    presetsDir = appDir + "/../Resources/presets";
#else
    presetsDir = appDir + "/presets";
#endif

    if (dir.mkpath(presetsDir)) {
        ui->textLog->append(
            QString("[信息] 创建预设文件夹: %1").arg(QDir(presetsDir).absolutePath()));
        return QDir(presetsDir).absolutePath();
    }

    ui->textLog->append("[错误] 无法创建预设文件夹");
    return QString();
}

void MainWindow::createDefaultPresets(const QString& presetsDir) {
    // 检查是否已有预设文件
    QDir dir(presetsDir);
    QStringList existing = dir.entryList(QStringList() << "*.json", QDir::Files);

    // 过滤掉模板文件
    int realPresets = 0;
    for (const QString& f : existing) {
        if (!f.contains("template", Qt::CaseInsensitive) &&
            !f.contains("README", Qt::CaseInsensitive)) {
            realPresets++;
        }
    }

    if (realPresets > 0) {
        // 已有预设文件，不需要创建
        return;
    }

    ui->textLog->append("[信息] 正在创建默认预设文件...");

    // 辅助函数：创建预设JSON
    auto createPresetJson = [](const QString& name, const QString& desc, double thrust, double isp,
                               double pc, double of, double enthalpy, const QString& propType,
                               double pe) -> QByteArray {
        QJsonObject root;
        root["version"] = 1;
        root["application"] = "SLS_ThermoCalc";

        QJsonObject engine;
        engine["name"] = name;
        engine["description"] = desc;
        engine["thrust_kN"] = thrust;
        engine["specificImpulse_vac_s"] = isp;
        root["engineDefinition"] = engine;

        QJsonObject combustor;
        QJsonObject pressure;
        pressure["value"] = pc;
        pressure["units"] = "MPa";
        combustor["chamberPressure"] = pressure;
        combustor["mixtureRatio"] = of;
        combustor["initialEnthalpy_kJ_kg"] = enthalpy;
        root["combustorConditions"] = combustor;

        QJsonObject propellant;
        propellant["type"] = propType;
        QJsonObject ox, fuel;
        ox["name"] = "O2(L)";
        ox["formula"] = "O2";
        propellant["oxidizer"] = ox;
        if (propType == "LOX_CH4") {
            fuel["name"] = "CH4(L)";
            fuel["formula"] = "CH4";
        } else {
            fuel["name"] = "H2(L)";
            fuel["formula"] = "H2";
        }
        propellant["fuel"] = fuel;
        root["propellant"] = propellant;

        QJsonObject nozzle;
        nozzle["exitPressure_atm"] = pe;
        nozzle["flowType"] = "equilibrium";
        root["nozzleConditions"] = nozzle;

        QJsonObject thermo;
        thermo["method"] = "minimumGibbsFreeEnergy";
        thermo["coefficients"] = "NASA-9";
        root["thermodynamicModel"] = thermo;

        return QJsonDocument(root).toJson(QJsonDocument::Indented);
    };

    // 写入预设文件
    auto writePreset = [&](const QString& filename, const QByteArray& content) {
        QFile file(presetsDir + "/" + filename);
        if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
            file.write(content);
            file.close();
            return true;
        }
        return false;
    };

    int created = 0;

    // RS-25 (SSME) - SLS核心级
    if (writePreset(
            "RS-25_SSME.json",
            createPresetJson("RS-25 (SSME)", "Space Shuttle Main Engine - SLS Core Stage (LOX/LH2)",
                             2279, 452.3, 20.64, 6.03, -987.0, "LOX_LH2", 0.05)))
        created++;

    // RL-10B2 - 上面级
    if (writePreset("RL-10B2.json",
                    createPresetJson("RL-10B2", "Pratt & Whitney RL10B-2 - Upper Stage (LOX/LH2)",
                                     110.1, 465.5, 4.36, 5.88, -987.0, "LOX_LH2", 0.001)))
        created++;

    // SpaceX Raptor (LOX/CH4)
    if (writePreset(
            "Raptor.json",
            createPresetJson("SpaceX Raptor", "Full-Flow Staged Combustion Engine (LOX/CH4)", 2260,
                             363, 30.0, 3.6, -2875.72, "LOX_CH4", 1.0)))
        created++;

    // YF-77 - 长征五号
    if (writePreset("YF-77.json", createPresetJson("YF-77", "长征五号芯一级发动机 (LOX/LH2)", 700,
                                                   430, 10.2, 5.5, -987.0, "LOX_LH2", 0.1)))
        created++;

    ui->textLog->append(QString("[信息] 已创建 %1 个默认预设文件").arg(created));
}

void MainWindow::loadPresetsFromFolder() {
    m_presets.clear();
    ui->comboEngine->clear();

    // 确保预设文件夹存在，必要时创建
    QString presetsDir = ensurePresetsFolder();

    if (presetsDir.isEmpty()) {
        ui->textLog->append("[错误] 无法创建或找到预设文件夹");
        // 添加自定义选项作为唯一选择
        ui->comboEngine->addItem("── 自定义参数 ──", -1);
        return;
    }

    // 如果没有预设文件，创建默认预设
    createDefaultPresets(presetsDir);

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

    // 检查是否是"自定义参数"（-1）
    if (presetIndex < 0) {
        ui->lblEngineInfo->setText("自定义参数模式\n手动设置所有参数");
        ui->textLog->append(QString("[%1] 选择: 自定义参数模式")
                                .arg(QDateTime::currentDateTime().toString("hh:mm:ss")));
        return;
    }

    // 使用 JSON 文件预设
    if (presetIndex >= m_presets.size()) {
        ui->lblEngineInfo->setText("自定义参数模式\n手动设置所有参数");
        return;
    }

    const PresetInfo& preset = m_presets[presetIndex];

    // 加载预设文件
    if (loadPresetFromJson(preset.filePath)) {
        // 更新发动机信息显示
        ui->lblEngineInfo->setText(preset.description);

        // 记录日志
        ui->textLog->append(QString("[%1] 选择发动机: %2")
                                .arg(QDateTime::currentDateTime().toString("hh:mm:ss"))
                                .arg(preset.name));
    } else {
        // 加载失败
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
    // 确保配置已初始化（如果尚未初始化，使用 RS-25 作为默认）
    if (m_currentConfig.num_species == 0) {
        const EnginePreset* defaultPreset = get_engine_preset(ENGINE_RS25);
        if (defaultPreset) {
            m_currentConfig = defaultPreset->config;
        }
    }

    // 燃烧室压强 (MPa -> atm)
    double pc_mpa = ui->spinChamberPressure->value();
    m_currentConfig.chamber_pressure = pc_mpa / 0.101325;  // 1 atm = 0.101325 MPa

    // 初始焓从预设配置保持不变（已在 onEngineChanged 中设置）

    // 更新质量分数
    double of_ratio = ui->spinMixtureRatio->value();
    m_currentConfig.mass_fraction[0] = 1.0 / (1.0 + of_ratio);       // 燃料
    m_currentConfig.mass_fraction[1] = of_ratio / (1.0 + of_ratio);  // 氧化剂
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
            QString("  ✓ 真空比冲: %1 m/s")
                .arg(m_lastResult.nozzle.specific_impulse_vac * 9.80665, 0, 'f', 1));
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

    // 更新组分表 - 根据推进剂类型显示不同组分名称 (使用 Unicode 下标)
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
    auto setTableValue = [this](int row, const QString& value) {
        if (row < ui->tableChamber->rowCount()) {
            QTableWidgetItem* item = ui->tableChamber->item(row, 1);
            if (item) {
                item->setText(value);
            } else {
                ui->tableChamber->setItem(row, 1, new QTableWidgetItem(value));
            }
        }
    };

    int row = 0;
    setTableValue(row++, formatNumber(chamber.temperature, 2));
    setTableValue(row++, formatNumber(chamber.total_enthalpy / 1000.0, 2));
    setTableValue(row++, formatNumber(chamber.total_entropy / 1000.0, 4));
    setTableValue(row++, formatNumber(chamber.mean_molecular_weight, 3));
    setTableValue(row++, formatNumber(chamber.density, 3));
    setTableValue(row++, formatNumber(chamber.cp / 1000.0, 4));
    setTableValue(row++, formatNumber(chamber.cv / 1000.0, 4));
    setTableValue(row++, formatNumber(chamber.gamma, 4));
    setTableValue(row++, formatNumber(chamber.gamma_s, 4));
    setTableValue(row++, formatNumber(chamber.sound_speed, 1));
    setTableValue(row++, formatNumber(chamber.char_velocity, 1));
    setTableValue(row++, formatNumber(chamber.cp_frozen / 1000.0, 4));
    setTableValue(row++, formatNumber(chamber.gamma_frozen, 4));
    setTableValue(row++, formatNumber(chamber.sound_speed_frozen, 1));
    setTableValue(row++, formatNumber(chamber.R_specific, 2));
    setTableValue(row++, formatNumber(chamber.viscosity * 1e6, 4) + " ×10⁻⁶");
    setTableValue(row++, formatNumber(chamber.conductivity, 4));
    setTableValue(row++, formatNumber(chamber.prandtl, 4));
}

void MainWindow::displayNozzleResults(const NozzleResult& nozzle) {
    auto setTableValue = [this](int row, const QString& value) {
        if (row < ui->tableNozzle->rowCount()) {
            QTableWidgetItem* item = ui->tableNozzle->item(row, 1);
            if (item) {
                item->setText(value);
            } else {
                ui->tableNozzle->setItem(row, 1, new QTableWidgetItem(value));
            }
        }
    };

    int row = 0;
    setTableValue(row++, formatNumber(nozzle.exit_temperature, 2));
    setTableValue(row++, formatNumber(nozzle.exit_pressure, 4));
    setTableValue(row++, formatNumber(nozzle.exit_velocity, 1));                  /* 燃气速度 */
    setTableValue(row++, formatNumber(nozzle.specific_impulse_vac * 9.80665, 1)); /* m/s = s × g₀ */
    setTableValue(row++, formatNumber(nozzle.mach_number, 2));
    setTableValue(row++, formatNumber(nozzle.thrust_coefficient, 4));
    setTableValue(row++, formatNumber(nozzle.mass_flow_coefficient, 4));
    setTableValue(row++, formatNumber(nozzle.mean_gamma, 4));
    setTableValue(row++, formatNumber(nozzle.pressure_ratio, 2));
    setTableValue(row++, formatNumber(nozzle.exit_density, 4));
    setTableValue(row++, formatNumber(nozzle.exit_sound_speed, 1));
    setTableValue(row++, formatNumber(nozzle.throat_temperature, 2));
    setTableValue(row++, formatNumber(nozzle.throat_pressure, 4));
    setTableValue(row++, formatNumber(nozzle.throat_velocity, 1));
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
    out << QString("总焓 H: %1 kJ/mol\n")
               .arg(m_lastResult.chamber.total_enthalpy / 1000.0, 0, 'f', 3);
    out << QString("总熵 S: %1 kJ/mol/K\n")
               .arg(m_lastResult.chamber.total_entropy / 1000.0, 0, 'f', 4);
    out << QString("平均分子量 M: %1 g/mol\n")
               .arg(m_lastResult.chamber.mean_molecular_weight, 0, 'f', 3);
    out << QString("密度 ρ: %1 kg/m³\n").arg(m_lastResult.chamber.density, 0, 'f', 3);
    out << QString("平衡定压比热 cp: %1 kJ/mol/K\n")
               .arg(m_lastResult.chamber.cp / 1000.0, 0, 'f', 4);
    out << QString("平衡定容比热 cv: %1 kJ/mol/K\n")
               .arg(m_lastResult.chamber.cv / 1000.0, 0, 'f', 4);
    out << QString("比热比 γ: %1\n").arg(m_lastResult.chamber.gamma, 0, 'f', 4);
    out << QString("等熵指数 γs: %1\n").arg(m_lastResult.chamber.gamma_s, 0, 'f', 4);
    out << QString("平衡声速 as: %1 m/s\n").arg(m_lastResult.chamber.sound_speed, 0, 'f', 1);
    out << QString("特征速度 c*: %1 m/s\n").arg(m_lastResult.chamber.char_velocity, 0, 'f', 1);
    out << QString("冻结定压比热 cp,f: %1 kJ/mol/K\n")
               .arg(m_lastResult.chamber.cp_frozen / 1000.0, 0, 'f', 4);
    out << QString("冻结比热比 γf: %1\n").arg(m_lastResult.chamber.gamma_frozen, 0, 'f', 4);
    out << QString("冻结声速 as,f: %1 m/s\n")
               .arg(m_lastResult.chamber.sound_speed_frozen, 0, 'f', 1);
    out << QString("等价气体常数 R: %1 J/kg/K\n").arg(m_lastResult.chamber.R_specific, 0, 'f', 2);
    out << QString("粘性系数 μ: %1 Pa·s\n").arg(m_lastResult.chamber.viscosity, 0, 'e', 4);
    out << QString("导热系数 λ: %1 W/m/K\n").arg(m_lastResult.chamber.conductivity, 0, 'f', 4);
    out << QString("普朗特数 Pr: %1\n\n").arg(m_lastResult.chamber.prandtl, 0, 'f', 4);

    out << "【喷管性能参数】\n";
    out << QString("出口温度 Te: %1 K\n").arg(m_lastResult.nozzle.exit_temperature, 0, 'f', 2);
    out << QString("出口压力 pe: %1 atm\n").arg(m_lastResult.nozzle.exit_pressure, 0, 'f', 4);
    out << QString("燃气速度 ue: %1 m/s\n").arg(m_lastResult.nozzle.exit_velocity, 0, 'f', 1);
    out << QString("真空比冲 Isp,vac: %1 m/s\n")
               .arg(m_lastResult.nozzle.specific_impulse_vac * 9.80665, 0, 'f', 1);
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
    aboutBox.setIconPixmap(QPixmap(":/icons/rocket.svg")
                               .scaled(64, 64, Qt::KeepAspectRatio, Qt::SmoothTransformation));

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

void MainWindow::onAboutQt() { QMessageBox::aboutQt(this, "关于 Qt"); }

void MainWindow::onImportPreset() {
    // 默认打开 presets 文件夹
    QString defaultDir = QCoreApplication::applicationDirPath() + "/../../../presets";
    QDir dir(defaultDir);
    if (!dir.exists())
        defaultDir = "";

    QString filename = QFileDialog::getOpenFileName(this, "导入预设配置", defaultDir,
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
    QString filename = QFileDialog::getSaveFileName(this, "导出预设配置", "engine_preset.json",
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
    combustor["initialEnthalpy_kJ_kg"] = m_currentConfig.initial_enthalpy / 1000.0;
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
            m_currentConfig.initial_enthalpy =
                combustor["initialEnthalpy_kJ_kg"].toDouble() * 1000.0;
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

    // 更新热力数据显示
    loadThermoDataToUI();

    return true;
}

/* ============ 热力数据 Tab 功能实现 ============ */

QStringList MainWindow::getSpeciesNames() {
    QStringList names;
    if (m_currentConfig.num_elements == 3) {
        // LOX/CH4: H2O, H2, OH, H, CO2, CO, O2, O
        names = {"H₂O", "H₂", "OH", "H", "CO₂", "CO", "O₂", "O"};
    } else {
        // LOX/LH2: H, H2, H2O, O, O2, OH
        names = {"H", "H₂", "H₂O", "O", "O₂", "OH"};
    }
    return names;
}

void MainWindow::setupThermoDataTab() {
    // 初始化 NASA 9 系数表
    ui->tableNasa9->setRowCount(1);
    ui->tableNasa9->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tableNasa9->verticalHeader()->setVisible(false);

    // 初始化推进剂矩阵表
    ui->tablePropMatrix->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tablePropMatrix->verticalHeader()->setDefaultSectionSize(28);

    // 初始化迭代初值表
    ui->tableInitialGuess->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tableInitialGuess->verticalHeader()->setVisible(false);

    // 设置元素原子量范围
    ui->spinElemH->setRange(0.5, 10.0);
    ui->spinElemO->setRange(10.0, 20.0);
    ui->spinElemC->setRange(10.0, 15.0);

    // 确保 m_currentConfig 已初始化
    if (m_currentConfig.num_species == 0) {
        const EnginePreset* defaultPreset = get_engine_preset(ENGINE_RS25);
        if (defaultPreset) {
            m_currentConfig = defaultPreset->config;
        }
    }

    // 加载热力数据到 UI
    loadThermoDataToUI();
}

void MainWindow::loadThermoDataToUI() {
    // 阻止信号触发
    ui->comboSpeciesSelect->blockSignals(true);
    ui->spinNumSpecies->blockSignals(true);
    ui->spinNumElements->blockSignals(true);

    // 更新基本参数
    ui->spinNumSpecies->setValue(m_currentConfig.num_species);
    ui->spinNumElements->setValue(m_currentConfig.num_elements);

    // 更新组分选择下拉框
    ui->comboSpeciesSelect->clear();
    QStringList speciesNames = getSpeciesNames();
    for (int i = 0; i < m_currentConfig.num_species && i < speciesNames.size(); ++i) {
        ui->comboSpeciesSelect->addItem(QString("%1 - %2").arg(i).arg(speciesNames[i]));
    }

    // 更新元素标签
    if (m_currentConfig.num_elements == 3) {
        ui->lblAtom1->setText("H:");
        ui->lblAtom2->setText("O:");
        ui->lblAtom3->setText("C:");
        ui->lblAtom3->setVisible(true);
        ui->spinAtom3->setVisible(true);
        ui->lblElemC->setVisible(true);
        ui->spinElemC->setVisible(true);
    } else {
        ui->lblAtom1->setText("H:");
        ui->lblAtom2->setText("O:");
        ui->lblAtom3->setVisible(false);
        ui->spinAtom3->setVisible(false);
        ui->lblElemC->setVisible(false);
        ui->spinElemC->setVisible(false);
    }

    // 更新元素原子量
    if (m_currentConfig.num_elements >= 1)
        ui->spinElemH->setValue(m_currentConfig.element_weight[0]);
    if (m_currentConfig.num_elements >= 2)
        ui->spinElemO->setValue(m_currentConfig.element_weight[1]);
    if (m_currentConfig.num_elements >= 3)
        ui->spinElemC->setValue(m_currentConfig.element_weight[2]);

    // 更新推进剂矩阵表
    int numProps = m_currentConfig.num_propellants;
    int numElems = m_currentConfig.num_elements;
    ui->tablePropMatrix->setRowCount(numProps);
    ui->tablePropMatrix->setColumnCount(numElems);

    QStringList elemHeaders;
    if (numElems >= 1)
        elemHeaders << "H";
    if (numElems >= 2)
        elemHeaders << "O";
    if (numElems >= 3)
        elemHeaders << "C";
    ui->tablePropMatrix->setHorizontalHeaderLabels(elemHeaders);

    QStringList propHeaders;
    if (m_currentConfig.num_elements == 3) {
        propHeaders << "CH₄ (燃料)" << "O₂ (氧化剂)";
    } else {
        propHeaders << "H₂ (燃料)" << "O₂ (氧化剂)";
    }
    ui->tablePropMatrix->setVerticalHeaderLabels(propHeaders);

    for (int i = 0; i < numProps; ++i) {
        for (int j = 0; j < numElems; ++j) {
            QTableWidgetItem* item =
                new QTableWidgetItem(QString::number(m_currentConfig.St_aij[i][j], 'f', 0));
            ui->tablePropMatrix->setItem(i, j, item);
        }
    }

    // 更新迭代初值表
    int numSpecies = m_currentConfig.num_species;
    ui->tableInitialGuess->setColumnCount(numSpecies);
    ui->tableInitialGuess->setRowCount(1);

    QStringList initHeaders;
    for (int i = 0; i < numSpecies && i < speciesNames.size(); ++i) {
        initHeaders << speciesNames[i];
    }
    ui->tableInitialGuess->setHorizontalHeaderLabels(initHeaders);

    // 这里需要从预设中获取初始值（目前使用默认值）
    double c_init[MAX_SPECIES] = {0};
    get_initial_guess(ENGINE_RS25, c_init, numSpecies);

    for (int i = 0; i < numSpecies; ++i) {
        QTableWidgetItem* item = new QTableWidgetItem(QString::number(c_init[i], 'e', 2));
        ui->tableInitialGuess->setItem(0, i, item);
    }

    // 加载第一个组分的数据
    if (m_currentConfig.num_species > 0) {
        loadSpeciesDataToUI(0);
    }

    // 恢复信号
    ui->comboSpeciesSelect->blockSignals(false);
    ui->spinNumSpecies->blockSignals(false);
    ui->spinNumElements->blockSignals(false);
}

void MainWindow::loadSpeciesDataToUI(int speciesIndex) {
    if (speciesIndex < 0 || speciesIndex >= m_currentConfig.num_species)
        return;

    QStringList speciesNames = getSpeciesNames();
    QString name = (speciesIndex < speciesNames.size()) ? speciesNames[speciesIndex]
                                                        : QString("Species %1").arg(speciesIndex);

    // 移除 Unicode 下标用于显示
    QString plainName = name;
    plainName.replace("₂", "2");
    ui->editSpeciesName->setText(plainName);

    // 从 NASA 9 系数计算分子量（简化：使用原子组成计算）
    double molWeight = 0.0;
    for (int k = 0; k < m_currentConfig.num_elements; ++k) {
        molWeight += m_currentConfig.Aij[k][speciesIndex] * m_currentConfig.element_weight[k];
    }
    ui->spinMolWeight->setValue(molWeight);

    // 设置原子组成
    if (m_currentConfig.num_elements >= 1)
        ui->spinAtom1->setValue(static_cast<int>(m_currentConfig.Aij[0][speciesIndex]));
    if (m_currentConfig.num_elements >= 2)
        ui->spinAtom2->setValue(static_cast<int>(m_currentConfig.Aij[1][speciesIndex]));
    if (m_currentConfig.num_elements >= 3)
        ui->spinAtom3->setValue(static_cast<int>(m_currentConfig.Aij[2][speciesIndex]));

    // 加载 NASA 9 系数（高温区 1000-6000K）
    const NASA9Coefficients* coeff = &m_currentConfig.nasa9[speciesIndex];
    int highTempInterval = (coeff->num_intervals > 1) ? 1 : 0;

    ui->tableNasa9->setRowCount(1);
    for (int i = 0; i < NASA9_COEFF_NUM; ++i) {
        double val = coeff->intervals[highTempInterval].a[i];
        QTableWidgetItem* item = new QTableWidgetItem(QString::number(val, 'e', 6));
        ui->tableNasa9->setItem(0, i, item);
    }

    // 计算 Hf(298K) 并显示
    // 注意：这是一个近似值，实际需要从数据库获取
    ui->spinHf298->setValue(0.0);  // 默认值
}

void MainWindow::saveSpeciesDataFromUI(int speciesIndex) {
    if (speciesIndex < 0 || speciesIndex >= m_currentConfig.num_species)
        return;

    // 保存原子组成到 Aij 矩阵
    if (m_currentConfig.num_elements >= 1)
        m_currentConfig.Aij[0][speciesIndex] = ui->spinAtom1->value();
    if (m_currentConfig.num_elements >= 2)
        m_currentConfig.Aij[1][speciesIndex] = ui->spinAtom2->value();
    if (m_currentConfig.num_elements >= 3)
        m_currentConfig.Aij[2][speciesIndex] = ui->spinAtom3->value();

    // 保存 NASA 9 系数
    NASA9Coefficients* coeff = &m_currentConfig.nasa9[speciesIndex];
    int highTempInterval = (coeff->num_intervals > 1) ? 1 : 0;

    for (int i = 0; i < NASA9_COEFF_NUM; ++i) {
        QTableWidgetItem* item = ui->tableNasa9->item(0, i);
        if (item) {
            coeff->intervals[highTempInterval].a[i] = item->text().toDouble();
        }
    }

    ui->textLog->append(QString("[%1] 已更新组分 %2 的热力数据")
                            .arg(QDateTime::currentDateTime().toString("hh:mm:ss"))
                            .arg(speciesIndex));
}

void MainWindow::onSpeciesSelectionChanged(int index) { loadSpeciesDataToUI(index); }

void MainWindow::onApplySpeciesChange() {
    int currentIndex = ui->comboSpeciesSelect->currentIndex();
    saveSpeciesDataFromUI(currentIndex);

    // 同时保存元素原子量
    if (m_currentConfig.num_elements >= 1)
        m_currentConfig.element_weight[0] = ui->spinElemH->value();
    if (m_currentConfig.num_elements >= 2)
        m_currentConfig.element_weight[1] = ui->spinElemO->value();
    if (m_currentConfig.num_elements >= 3)
        m_currentConfig.element_weight[2] = ui->spinElemC->value();

    // 保存推进剂矩阵
    for (int i = 0; i < m_currentConfig.num_propellants; ++i) {
        for (int j = 0; j < m_currentConfig.num_elements; ++j) {
            QTableWidgetItem* item = ui->tablePropMatrix->item(i, j);
            if (item) {
                m_currentConfig.St_aij[i][j] = item->text().toDouble();
            }
        }
    }

    setStatusMessage("热力数据已更新");
    ui->textLog->append(
        QString("[%1] 热力数据配置已应用").arg(QDateTime::currentDateTime().toString("hh:mm:ss")));

    // 自动保存到配置文件
    saveThermoConfigToJson();
}

void MainWindow::onResetThermoData() {
    // 获取当前选择的发动机类型
    int presetIndex = ui->comboEngine->currentData().toInt();

    if (presetIndex >= 0 && presetIndex < m_presets.size()) {
        // 从预设文件重新加载
        loadPresetFromJson(m_presets[presetIndex].filePath);
    } else {
        // 使用默认 RS-25 配置
        const EnginePreset* defaultPreset = get_engine_preset(ENGINE_RS25);
        if (defaultPreset) {
            m_currentConfig = defaultPreset->config;
        }
    }

    loadThermoDataToUI();

    // 保存重置后的配置
    saveThermoConfigToJson();

    setStatusMessage("热力数据已重置为默认值");
    ui->textLog->append(
        QString("[%1] 热力数据已重置").arg(QDateTime::currentDateTime().toString("hh:mm:ss")));
}

void MainWindow::onThermoDataCellChanged(int row, int col) {
    Q_UNUSED(row);
    Q_UNUSED(col);
    // 当表格数据改变时自动标记为已修改
    setStatusMessage("热力数据已修改，点击'应用修改'保存");
}

/* ============ 用户热力数据配置文件 ============ */

QString MainWindow::getThermoConfigPath() {
    // 配置文件放在应用程序目录下的 config 文件夹
    QString appDir = QCoreApplication::applicationDirPath();

#ifdef Q_OS_MAC
    // macOS: SLS_ThermoCalc.app/Contents/MacOS -> SLS_ThermoCalc/config
    QString configDir = appDir + "/../../../../config";
#else
    // Windows/Linux: 可执行文件同级 config 文件夹
    QString configDir = appDir + "/config";
#endif

    return QDir(configDir).absolutePath() + "/thermo_config.json";
}

void MainWindow::ensureThermoConfigExists() {
    QString configPath = getThermoConfigPath();
    QFileInfo fileInfo(configPath);

    // 确保目录存在
    QDir dir;
    dir.mkpath(fileInfo.absolutePath());

    // 如果配置文件不存在，创建默认配置
    if (!fileInfo.exists()) {
        ui->textLog->append(QString("[信息] 首次运行，正在生成热力数据配置文件..."));

        // 初始化默认配置 (RS-25 LOX/LH2)
        const EnginePreset* defaultPreset = get_engine_preset(ENGINE_RS25);
        if (defaultPreset) {
            m_currentConfig = defaultPreset->config;
        }

        // 保存到文件
        if (saveThermoConfigToJson()) {
            ui->textLog->append(QString("[信息] 配置文件已创建: %1").arg(configPath));
        }
    }
}

bool MainWindow::saveThermoConfigToJson() {
    QString configPath = getThermoConfigPath();
    QFile file(configPath);

    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        ui->textLog->append(QString("[错误] 无法保存配置文件: %1").arg(configPath));
        return false;
    }

    QJsonObject root;
    root["version"] = 1;
    root["application"] = "SLS_ThermoCalc";
    root["description"] = "用户热力数据配置文件";
    root["lastModified"] = QDateTime::currentDateTime().toString(Qt::ISODate);

    // 基本参数
    QJsonObject config;
    config["num_propellants"] = m_currentConfig.num_propellants;
    config["num_elements"] = m_currentConfig.num_elements;
    config["num_species"] = m_currentConfig.num_species;
    config["num_condensed"] = m_currentConfig.num_condensed;

    // 元素原子量
    QJsonArray elementWeights;
    for (int i = 0; i < m_currentConfig.num_elements; ++i) {
        elementWeights.append(m_currentConfig.element_weight[i]);
    }
    config["element_weights"] = elementWeights;

    // 推进剂矩阵 St_aij
    QJsonArray stMatrix;
    for (int i = 0; i < m_currentConfig.num_propellants; ++i) {
        QJsonArray row;
        for (int j = 0; j < m_currentConfig.num_elements; ++j) {
            row.append(m_currentConfig.St_aij[i][j]);
        }
        stMatrix.append(row);
    }
    config["St_aij"] = stMatrix;

    // 产物-元素矩阵 Aij
    QJsonArray aijMatrix;
    for (int i = 0; i < m_currentConfig.num_elements; ++i) {
        QJsonArray row;
        for (int j = 0; j < m_currentConfig.num_species; ++j) {
            row.append(m_currentConfig.Aij[i][j]);
        }
        aijMatrix.append(row);
    }
    config["Aij"] = aijMatrix;

    // NASA 9 系数
    QJsonArray speciesData;
    QStringList speciesNames = getSpeciesNames();
    for (int i = 0; i < m_currentConfig.num_species; ++i) {
        QJsonObject species;
        species["name"] =
            (i < speciesNames.size()) ? speciesNames[i] : QString("Species_%1").arg(i);
        species["num_intervals"] = m_currentConfig.nasa9[i].num_intervals;

        QJsonArray intervals;
        for (int k = 0; k < m_currentConfig.nasa9[i].num_intervals; ++k) {
            QJsonObject interval;
            interval["T_min"] = m_currentConfig.nasa9[i].intervals[k].T_min;
            interval["T_max"] = m_currentConfig.nasa9[i].intervals[k].T_max;

            QJsonArray coeffs;
            for (int c = 0; c < NASA9_COEFF_NUM; ++c) {
                coeffs.append(m_currentConfig.nasa9[i].intervals[k].a[c]);
            }
            interval["coefficients"] = coeffs;
            intervals.append(interval);
        }
        species["intervals"] = intervals;
        speciesData.append(species);
    }
    config["species"] = speciesData;

    root["thermoConfig"] = config;

    // 写入文件
    QJsonDocument doc(root);
    file.write(doc.toJson(QJsonDocument::Indented));
    file.close();

    ui->textLog->append(
        QString("[%1] 热力配置已保存").arg(QDateTime::currentDateTime().toString("hh:mm:ss")));
    return true;
}

bool MainWindow::loadThermoConfigFromJson() {
    QString configPath = getThermoConfigPath();
    QFile file(configPath);

    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        return false;
    }

    QByteArray data = file.readAll();
    file.close();

    QJsonParseError parseError;
    QJsonDocument doc = QJsonDocument::fromJson(data, &parseError);
    if (parseError.error != QJsonParseError::NoError) {
        ui->textLog->append(QString("[错误] 配置文件解析失败: %1").arg(parseError.errorString()));
        return false;
    }

    QJsonObject root = doc.object();
    if (!root.contains("thermoConfig")) {
        return false;
    }

    QJsonObject config = root["thermoConfig"].toObject();

    // 读取基本参数
    m_currentConfig.num_propellants = config["num_propellants"].toInt(2);
    m_currentConfig.num_elements = config["num_elements"].toInt(2);
    m_currentConfig.num_species = config["num_species"].toInt(6);
    m_currentConfig.num_condensed = config["num_condensed"].toInt(0);

    // 读取元素原子量
    QJsonArray elementWeights = config["element_weights"].toArray();
    for (int i = 0; i < elementWeights.size() && i < MAX_ELEMENTS; ++i) {
        m_currentConfig.element_weight[i] = elementWeights[i].toDouble();
    }

    // 读取推进剂矩阵
    QJsonArray stMatrix = config["St_aij"].toArray();
    for (int i = 0; i < stMatrix.size() && i < MAX_PROPELLANTS; ++i) {
        QJsonArray row = stMatrix[i].toArray();
        for (int j = 0; j < row.size() && j < MAX_ELEMENTS; ++j) {
            m_currentConfig.St_aij[i][j] = row[j].toDouble();
        }
    }

    // 读取产物-元素矩阵
    QJsonArray aijMatrix = config["Aij"].toArray();
    for (int i = 0; i < aijMatrix.size() && i < MAX_ELEMENTS; ++i) {
        QJsonArray row = aijMatrix[i].toArray();
        for (int j = 0; j < row.size() && j < MAX_SPECIES; ++j) {
            m_currentConfig.Aij[i][j] = row[j].toDouble();
        }
    }

    // 读取 NASA 9 系数
    QJsonArray speciesData = config["species"].toArray();
    for (int i = 0; i < speciesData.size() && i < MAX_SPECIES; ++i) {
        QJsonObject species = speciesData[i].toObject();
        m_currentConfig.nasa9[i].num_intervals = species["num_intervals"].toInt(2);

        QJsonArray intervals = species["intervals"].toArray();
        for (int k = 0; k < intervals.size() && k < NASA9_TEMP_RANGES; ++k) {
            QJsonObject interval = intervals[k].toObject();
            m_currentConfig.nasa9[i].intervals[k].T_min = interval["T_min"].toDouble();
            m_currentConfig.nasa9[i].intervals[k].T_max = interval["T_max"].toDouble();

            QJsonArray coeffs = interval["coefficients"].toArray();
            for (int c = 0; c < coeffs.size() && c < NASA9_COEFF_NUM; ++c) {
                m_currentConfig.nasa9[i].intervals[k].a[c] = coeffs[c].toDouble();
            }
        }
    }

    ui->textLog->append(QString("[信息] 已加载热力配置文件"));
    return true;
}
