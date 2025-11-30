/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 6.9.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDoubleSpinBox>
#include <QtWidgets/QFrame>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QTableWidget>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QAction *actionImport;
    QAction *actionExport;
    QAction *actionExportResults;
    QAction *actionExit;
    QAction *actionAbout;
    QAction *actionAboutQt;
    QWidget *centralwidget;
    QHBoxLayout *horizontalLayout;
    QFrame *inputPanel;
    QVBoxLayout *inputLayout;
    QGroupBox *groupEngine;
    QVBoxLayout *engineLayout;
    QComboBox *comboEngine;
    QLabel *lblEngineInfo;
    QGroupBox *groupPropellant;
    QGridLayout *propellantLayout;
    QLabel *lblMixtureRatio;
    QDoubleSpinBox *spinMixtureRatio;
    QLabel *lblOxidizer;
    QLineEdit *editOxidizerFrac;
    QLabel *lblFuel;
    QLineEdit *editFuelFrac;
    QLabel *lblInitialEnthalpy;
    QDoubleSpinBox *spinInitialEnthalpy;
    QGroupBox *groupChamber;
    QGridLayout *chamberLayout;
    QLabel *lblChamberPressure;
    QDoubleSpinBox *spinChamberPressure;
    QLabel *lblTmin;
    QDoubleSpinBox *spinTmin;
    QLabel *lblTmax;
    QDoubleSpinBox *spinTmax;
    QGroupBox *groupNozzle;
    QGridLayout *nozzleLayout;
    QLabel *lblExitPressure;
    QDoubleSpinBox *spinExitPressure;
    QLabel *lblAmbientPressure;
    QDoubleSpinBox *spinAmbientPressure;
    QHBoxLayout *buttonLayout;
    QPushButton *btnCalculate;
    QHBoxLayout *buttonLayout2;
    QPushButton *btnExport;
    QPushButton *btnClear;
    QSpacerItem *verticalSpacer;
    QTabWidget *tabResults;
    QWidget *tabChamber;
    QVBoxLayout *chamberResultLayout;
    QTableWidget *tableChamber;
    QWidget *tabNozzle;
    QVBoxLayout *nozzleResultLayout;
    QTableWidget *tableNozzle;
    QWidget *tabSpecies;
    QVBoxLayout *speciesLayout;
    QTableWidget *tableSpecies;
    QWidget *tabLog;
    QVBoxLayout *logLayout;
    QTextEdit *textLog;
    QMenuBar *menubar;
    QMenu *menuFile;
    QMenu *menuHelp;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName("MainWindow");
        MainWindow->resize(1280, 800);
        MainWindow->setStyleSheet(QString::fromUtf8("\n"
"/* \347\247\221\346\212\200\351\243\216\346\240\274\344\270\273\351\242\230 - \346\267\261\350\223\235\350\211\262\347\263\273 */\n"
"QMainWindow {\n"
"    background-color: #0a0f1a;\n"
"}\n"
"QGroupBox {\n"
"    font-weight: bold;\n"
"    font-size: 13px;\n"
"    border: 1px solid #1e3a5f;\n"
"    border-radius: 6px;\n"
"    margin-top: 14px;\n"
"    padding-top: 12px;\n"
"    background-color: #0d1520;\n"
"    color: #e0e6ed;\n"
"}\n"
"QGroupBox::title {\n"
"    subcontrol-origin: margin;\n"
"    left: 12px;\n"
"    padding: 0 8px;\n"
"    color: #00b4d8;\n"
"    font-weight: bold;\n"
"}\n"
"QLabel {\n"
"    color: #b8c5d3;\n"
"    font-size: 12px;\n"
"}\n"
"QLineEdit, QDoubleSpinBox, QSpinBox, QComboBox {\n"
"    background-color: #0d1520;\n"
"    border: 1px solid #1e3a5f;\n"
"    border-radius: 4px;\n"
"    padding: 6px 10px;\n"
"    color: #e0e6ed;\n"
"    font-size: 12px;\n"
"    selection-background-color: #00b4d8;\n"
"}\n"
"QLineEdit:focus, QDoubleSpinBox:focus, QSpinBox:focus, QComboBox:focus {\n"
""
                        "    border: 1px solid #00b4d8;\n"
"}\n"
"QComboBox::drop-down {\n"
"    border: none;\n"
"    width: 24px;\n"
"}\n"
"QComboBox::down-arrow {\n"
"    image: none;\n"
"    border-left: 5px solid transparent;\n"
"    border-right: 5px solid transparent;\n"
"    border-top: 6px solid #00b4d8;\n"
"    margin-right: 6px;\n"
"}\n"
"QComboBox QAbstractItemView {\n"
"    background-color: #0d1520;\n"
"    border: 1px solid #1e3a5f;\n"
"    selection-background-color: #1e3a5f;\n"
"    color: #e0e6ed;\n"
"}\n"
"QPushButton {\n"
"    background-color: #00b4d8;\n"
"    color: #0a0f1a;\n"
"    border: none;\n"
"    border-radius: 4px;\n"
"    padding: 10px 24px;\n"
"    font-weight: bold;\n"
"    font-size: 13px;\n"
"}\n"
"QPushButton:hover {\n"
"    background-color: #48cae4;\n"
"}\n"
"QPushButton:pressed {\n"
"    background-color: #0096c7;\n"
"}\n"
"QPushButton:disabled {\n"
"    background-color: #1e3a5f;\n"
"    color: #5a6a7a;\n"
"}\n"
"QPushButton#btnClear {\n"
"    background-color: #e63946;\n"
"}\n"
"QPushButton#bt"
                        "nClear:hover {\n"
"    background-color: #f06c77;\n"
"}\n"
"QPushButton#btnExport {\n"
"    background-color: #2a9d8f;\n"
"}\n"
"QPushButton#btnExport:hover {\n"
"    background-color: #40b8a8;\n"
"}\n"
"QTableWidget {\n"
"    background-color: #0d1520;\n"
"    border: 1px solid #1e3a5f;\n"
"    border-radius: 4px;\n"
"    gridline-color: #1e3a5f;\n"
"    color: #e0e6ed;\n"
"    font-size: 12px;\n"
"}\n"
"QTableWidget::item {\n"
"    padding: 6px;\n"
"    border-bottom: 1px solid #1e3a5f;\n"
"}\n"
"QTableWidget::item:selected {\n"
"    background-color: #1e3a5f;\n"
"}\n"
"QHeaderView::section {\n"
"    background-color: #0d1520;\n"
"    color: #00b4d8;\n"
"    font-weight: bold;\n"
"    padding: 8px;\n"
"    border: none;\n"
"    border-bottom: 2px solid #00b4d8;\n"
"}\n"
"QTextEdit {\n"
"    background-color: #0d1520;\n"
"    border: 1px solid #1e3a5f;\n"
"    border-radius: 4px;\n"
"    color: #90caf9;\n"
"    font-family: \"SF Mono\", \"Monaco\", \"Menlo\", monospace;\n"
"    font-size: 11px;\n"
"}\n"
"QSta"
                        "tusBar {\n"
"    background-color: #0d1520;\n"
"    color: #5a6a7a;\n"
"    border-top: 1px solid #1e3a5f;\n"
"}\n"
"QMenuBar {\n"
"    background-color: #0d1520;\n"
"    color: #b8c5d3;\n"
"    border-bottom: 1px solid #1e3a5f;\n"
"    padding: 4px;\n"
"}\n"
"QMenuBar::item {\n"
"    padding: 6px 12px;\n"
"    border-radius: 4px;\n"
"}\n"
"QMenuBar::item:selected {\n"
"    background-color: #1e3a5f;\n"
"}\n"
"QMenu {\n"
"    background-color: #0d1520;\n"
"    color: #e0e6ed;\n"
"    border: 1px solid #1e3a5f;\n"
"    padding: 4px;\n"
"}\n"
"QMenu::item {\n"
"    padding: 8px 24px;\n"
"    border-radius: 4px;\n"
"}\n"
"QMenu::item:selected {\n"
"    background-color: #1e3a5f;\n"
"}\n"
"QMenu::separator {\n"
"    height: 1px;\n"
"    background-color: #1e3a5f;\n"
"    margin: 4px 8px;\n"
"}\n"
"QTabWidget::pane {\n"
"    border: 1px solid #1e3a5f;\n"
"    border-radius: 6px;\n"
"    background-color: #0d1520;\n"
"    padding: 4px;\n"
"}\n"
"QTabBar::tab {\n"
"    background-color: #0d1520;\n"
"    color: #5a6a7"
                        "a;\n"
"    padding: 10px 20px;\n"
"    border: 1px solid #1e3a5f;\n"
"    border-bottom: none;\n"
"    border-top-left-radius: 6px;\n"
"    border-top-right-radius: 6px;\n"
"    margin-right: 2px;\n"
"}\n"
"QTabBar::tab:selected {\n"
"    background-color: #1e3a5f;\n"
"    color: #00b4d8;\n"
"    border-bottom: 2px solid #00b4d8;\n"
"}\n"
"QTabBar::tab:hover:!selected {\n"
"    background-color: #152030;\n"
"}\n"
"QScrollBar:vertical {\n"
"    background-color: #0d1520;\n"
"    width: 10px;\n"
"    border-radius: 5px;\n"
"}\n"
"QScrollBar::handle:vertical {\n"
"    background-color: #1e3a5f;\n"
"    border-radius: 5px;\n"
"    min-height: 30px;\n"
"}\n"
"QScrollBar::handle:vertical:hover {\n"
"    background-color: #2e4a6f;\n"
"}\n"
"QScrollBar::add-line:vertical, QScrollBar::sub-line:vertical {\n"
"    height: 0px;\n"
"}\n"
"   "));
        actionImport = new QAction(MainWindow);
        actionImport->setObjectName("actionImport");
        actionExport = new QAction(MainWindow);
        actionExport->setObjectName("actionExport");
        actionExportResults = new QAction(MainWindow);
        actionExportResults->setObjectName("actionExportResults");
        actionExit = new QAction(MainWindow);
        actionExit->setObjectName("actionExit");
        actionAbout = new QAction(MainWindow);
        actionAbout->setObjectName("actionAbout");
        actionAboutQt = new QAction(MainWindow);
        actionAboutQt->setObjectName("actionAboutQt");
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName("centralwidget");
        horizontalLayout = new QHBoxLayout(centralwidget);
        horizontalLayout->setSpacing(16);
        horizontalLayout->setObjectName("horizontalLayout");
        horizontalLayout->setContentsMargins(16, 12, 16, 12);
        inputPanel = new QFrame(centralwidget);
        inputPanel->setObjectName("inputPanel");
        inputPanel->setMinimumWidth(360);
        inputPanel->setMaximumWidth(400);
        inputLayout = new QVBoxLayout(inputPanel);
        inputLayout->setSpacing(12);
        inputLayout->setObjectName("inputLayout");
        groupEngine = new QGroupBox(inputPanel);
        groupEngine->setObjectName("groupEngine");
        engineLayout = new QVBoxLayout(groupEngine);
        engineLayout->setObjectName("engineLayout");
        comboEngine = new QComboBox(groupEngine);
        comboEngine->setObjectName("comboEngine");
        comboEngine->setMinimumHeight(32);

        engineLayout->addWidget(comboEngine);

        lblEngineInfo = new QLabel(groupEngine);
        lblEngineInfo->setObjectName("lblEngineInfo");

        engineLayout->addWidget(lblEngineInfo);


        inputLayout->addWidget(groupEngine);

        groupPropellant = new QGroupBox(inputPanel);
        groupPropellant->setObjectName("groupPropellant");
        propellantLayout = new QGridLayout(groupPropellant);
        propellantLayout->setObjectName("propellantLayout");
        propellantLayout->setVerticalSpacing(8);
        lblMixtureRatio = new QLabel(groupPropellant);
        lblMixtureRatio->setObjectName("lblMixtureRatio");

        propellantLayout->addWidget(lblMixtureRatio, 0, 0, 1, 1);

        spinMixtureRatio = new QDoubleSpinBox(groupPropellant);
        spinMixtureRatio->setObjectName("spinMixtureRatio");
        spinMixtureRatio->setMinimum(1.000000000000000);
        spinMixtureRatio->setMaximum(20.000000000000000);
        spinMixtureRatio->setSingleStep(0.100000000000000);
        spinMixtureRatio->setValue(6.000000000000000);
        spinMixtureRatio->setDecimals(2);

        propellantLayout->addWidget(spinMixtureRatio, 0, 1, 1, 1);

        lblOxidizer = new QLabel(groupPropellant);
        lblOxidizer->setObjectName("lblOxidizer");

        propellantLayout->addWidget(lblOxidizer, 1, 0, 1, 1);

        editOxidizerFrac = new QLineEdit(groupPropellant);
        editOxidizerFrac->setObjectName("editOxidizerFrac");
        editOxidizerFrac->setReadOnly(true);

        propellantLayout->addWidget(editOxidizerFrac, 1, 1, 1, 1);

        lblFuel = new QLabel(groupPropellant);
        lblFuel->setObjectName("lblFuel");

        propellantLayout->addWidget(lblFuel, 2, 0, 1, 1);

        editFuelFrac = new QLineEdit(groupPropellant);
        editFuelFrac->setObjectName("editFuelFrac");
        editFuelFrac->setReadOnly(true);

        propellantLayout->addWidget(editFuelFrac, 2, 1, 1, 1);

        lblInitialEnthalpy = new QLabel(groupPropellant);
        lblInitialEnthalpy->setObjectName("lblInitialEnthalpy");

        propellantLayout->addWidget(lblInitialEnthalpy, 3, 0, 1, 1);

        spinInitialEnthalpy = new QDoubleSpinBox(groupPropellant);
        spinInitialEnthalpy->setObjectName("spinInitialEnthalpy");
        spinInitialEnthalpy->setMinimum(-10000.000000000000000);
        spinInitialEnthalpy->setMaximum(10000.000000000000000);
        spinInitialEnthalpy->setValue(-987.000000000000000);

        propellantLayout->addWidget(spinInitialEnthalpy, 3, 1, 1, 1);


        inputLayout->addWidget(groupPropellant);

        groupChamber = new QGroupBox(inputPanel);
        groupChamber->setObjectName("groupChamber");
        chamberLayout = new QGridLayout(groupChamber);
        chamberLayout->setObjectName("chamberLayout");
        chamberLayout->setVerticalSpacing(8);
        lblChamberPressure = new QLabel(groupChamber);
        lblChamberPressure->setObjectName("lblChamberPressure");

        chamberLayout->addWidget(lblChamberPressure, 0, 0, 1, 1);

        spinChamberPressure = new QDoubleSpinBox(groupChamber);
        spinChamberPressure->setObjectName("spinChamberPressure");
        spinChamberPressure->setMinimum(0.100000000000000);
        spinChamberPressure->setMaximum(50.000000000000000);
        spinChamberPressure->setSingleStep(0.500000000000000);
        spinChamberPressure->setValue(20.469999999999999);

        chamberLayout->addWidget(spinChamberPressure, 0, 1, 1, 1);

        lblTmin = new QLabel(groupChamber);
        lblTmin->setObjectName("lblTmin");

        chamberLayout->addWidget(lblTmin, 1, 0, 1, 1);

        spinTmin = new QDoubleSpinBox(groupChamber);
        spinTmin->setObjectName("spinTmin");
        spinTmin->setMinimum(500.000000000000000);
        spinTmin->setMaximum(3000.000000000000000);
        spinTmin->setValue(2500.000000000000000);

        chamberLayout->addWidget(spinTmin, 1, 1, 1, 1);

        lblTmax = new QLabel(groupChamber);
        lblTmax->setObjectName("lblTmax");

        chamberLayout->addWidget(lblTmax, 2, 0, 1, 1);

        spinTmax = new QDoubleSpinBox(groupChamber);
        spinTmax->setObjectName("spinTmax");
        spinTmax->setMinimum(1000.000000000000000);
        spinTmax->setMaximum(6000.000000000000000);
        spinTmax->setValue(4500.000000000000000);

        chamberLayout->addWidget(spinTmax, 2, 1, 1, 1);


        inputLayout->addWidget(groupChamber);

        groupNozzle = new QGroupBox(inputPanel);
        groupNozzle->setObjectName("groupNozzle");
        nozzleLayout = new QGridLayout(groupNozzle);
        nozzleLayout->setObjectName("nozzleLayout");
        nozzleLayout->setVerticalSpacing(8);
        lblExitPressure = new QLabel(groupNozzle);
        lblExitPressure->setObjectName("lblExitPressure");

        nozzleLayout->addWidget(lblExitPressure, 0, 0, 1, 1);

        spinExitPressure = new QDoubleSpinBox(groupNozzle);
        spinExitPressure->setObjectName("spinExitPressure");
        spinExitPressure->setMinimum(0.001000000000000);
        spinExitPressure->setMaximum(10.000000000000000);
        spinExitPressure->setSingleStep(0.010000000000000);
        spinExitPressure->setValue(0.050000000000000);
        spinExitPressure->setDecimals(4);

        nozzleLayout->addWidget(spinExitPressure, 0, 1, 1, 1);

        lblAmbientPressure = new QLabel(groupNozzle);
        lblAmbientPressure->setObjectName("lblAmbientPressure");

        nozzleLayout->addWidget(lblAmbientPressure, 1, 0, 1, 1);

        spinAmbientPressure = new QDoubleSpinBox(groupNozzle);
        spinAmbientPressure->setObjectName("spinAmbientPressure");
        spinAmbientPressure->setMinimum(0.000000000000000);
        spinAmbientPressure->setMaximum(2.000000000000000);
        spinAmbientPressure->setSingleStep(0.010000000000000);
        spinAmbientPressure->setValue(0.000000000000000);
        spinAmbientPressure->setDecimals(4);

        nozzleLayout->addWidget(spinAmbientPressure, 1, 1, 1, 1);


        inputLayout->addWidget(groupNozzle);

        buttonLayout = new QHBoxLayout();
        buttonLayout->setObjectName("buttonLayout");
        btnCalculate = new QPushButton(inputPanel);
        btnCalculate->setObjectName("btnCalculate");
        btnCalculate->setMinimumHeight(42);

        buttonLayout->addWidget(btnCalculate);


        inputLayout->addLayout(buttonLayout);

        buttonLayout2 = new QHBoxLayout();
        buttonLayout2->setObjectName("buttonLayout2");
        btnExport = new QPushButton(inputPanel);
        btnExport->setObjectName("btnExport");

        buttonLayout2->addWidget(btnExport);

        btnClear = new QPushButton(inputPanel);
        btnClear->setObjectName("btnClear");

        buttonLayout2->addWidget(btnClear);


        inputLayout->addLayout(buttonLayout2);

        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Expanding);

        inputLayout->addItem(verticalSpacer);


        horizontalLayout->addWidget(inputPanel);

        tabResults = new QTabWidget(centralwidget);
        tabResults->setObjectName("tabResults");
        tabChamber = new QWidget();
        tabChamber->setObjectName("tabChamber");
        chamberResultLayout = new QVBoxLayout(tabChamber);
        chamberResultLayout->setObjectName("chamberResultLayout");
        tableChamber = new QTableWidget(tabChamber);
        if (tableChamber->columnCount() < 2)
            tableChamber->setColumnCount(2);
        QTableWidgetItem *__qtablewidgetitem = new QTableWidgetItem();
        tableChamber->setHorizontalHeaderItem(0, __qtablewidgetitem);
        QTableWidgetItem *__qtablewidgetitem1 = new QTableWidgetItem();
        tableChamber->setHorizontalHeaderItem(1, __qtablewidgetitem1);
        tableChamber->setObjectName("tableChamber");
        tableChamber->setColumnCount(2);
        tableChamber->setRowCount(12);

        chamberResultLayout->addWidget(tableChamber);

        tabResults->addTab(tabChamber, QString());
        tabNozzle = new QWidget();
        tabNozzle->setObjectName("tabNozzle");
        nozzleResultLayout = new QVBoxLayout(tabNozzle);
        nozzleResultLayout->setObjectName("nozzleResultLayout");
        tableNozzle = new QTableWidget(tabNozzle);
        if (tableNozzle->columnCount() < 2)
            tableNozzle->setColumnCount(2);
        QTableWidgetItem *__qtablewidgetitem2 = new QTableWidgetItem();
        tableNozzle->setHorizontalHeaderItem(0, __qtablewidgetitem2);
        QTableWidgetItem *__qtablewidgetitem3 = new QTableWidgetItem();
        tableNozzle->setHorizontalHeaderItem(1, __qtablewidgetitem3);
        tableNozzle->setObjectName("tableNozzle");
        tableNozzle->setColumnCount(2);
        tableNozzle->setRowCount(8);

        nozzleResultLayout->addWidget(tableNozzle);

        tabResults->addTab(tabNozzle, QString());
        tabSpecies = new QWidget();
        tabSpecies->setObjectName("tabSpecies");
        speciesLayout = new QVBoxLayout(tabSpecies);
        speciesLayout->setObjectName("speciesLayout");
        tableSpecies = new QTableWidget(tabSpecies);
        if (tableSpecies->columnCount() < 3)
            tableSpecies->setColumnCount(3);
        QTableWidgetItem *__qtablewidgetitem4 = new QTableWidgetItem();
        tableSpecies->setHorizontalHeaderItem(0, __qtablewidgetitem4);
        QTableWidgetItem *__qtablewidgetitem5 = new QTableWidgetItem();
        tableSpecies->setHorizontalHeaderItem(1, __qtablewidgetitem5);
        QTableWidgetItem *__qtablewidgetitem6 = new QTableWidgetItem();
        tableSpecies->setHorizontalHeaderItem(2, __qtablewidgetitem6);
        tableSpecies->setObjectName("tableSpecies");
        tableSpecies->setColumnCount(3);

        speciesLayout->addWidget(tableSpecies);

        tabResults->addTab(tabSpecies, QString());
        tabLog = new QWidget();
        tabLog->setObjectName("tabLog");
        logLayout = new QVBoxLayout(tabLog);
        logLayout->setObjectName("logLayout");
        textLog = new QTextEdit(tabLog);
        textLog->setObjectName("textLog");
        textLog->setReadOnly(true);

        logLayout->addWidget(textLog);

        tabResults->addTab(tabLog, QString());

        horizontalLayout->addWidget(tabResults);

        MainWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(MainWindow);
        menubar->setObjectName("menubar");
        menubar->setGeometry(QRect(0, 0, 1280, 30));
        menuFile = new QMenu(menubar);
        menuFile->setObjectName("menuFile");
        menuHelp = new QMenu(menubar);
        menuHelp->setObjectName("menuHelp");
        MainWindow->setMenuBar(menubar);
        statusbar = new QStatusBar(MainWindow);
        statusbar->setObjectName("statusbar");
        MainWindow->setStatusBar(statusbar);

        menubar->addAction(menuFile->menuAction());
        menubar->addAction(menuHelp->menuAction());
        menuFile->addAction(actionImport);
        menuFile->addAction(actionExport);
        menuFile->addSeparator();
        menuFile->addAction(actionExportResults);
        menuFile->addSeparator();
        menuFile->addAction(actionExit);
        menuHelp->addAction(actionAbout);
        menuHelp->addAction(actionAboutQt);

        retranslateUi(MainWindow);

        tabResults->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "SLS Rocket Engine Thermodynamic Calculator", nullptr));
        actionImport->setText(QCoreApplication::translate("MainWindow", "Import Preset...", nullptr));
#if QT_CONFIG(shortcut)
        actionImport->setShortcut(QCoreApplication::translate("MainWindow", "Ctrl+O", nullptr));
#endif // QT_CONFIG(shortcut)
        actionExport->setText(QCoreApplication::translate("MainWindow", "Export Preset...", nullptr));
#if QT_CONFIG(shortcut)
        actionExport->setShortcut(QCoreApplication::translate("MainWindow", "Ctrl+S", nullptr));
#endif // QT_CONFIG(shortcut)
        actionExportResults->setText(QCoreApplication::translate("MainWindow", "Export Results...", nullptr));
#if QT_CONFIG(shortcut)
        actionExportResults->setShortcut(QCoreApplication::translate("MainWindow", "Ctrl+E", nullptr));
#endif // QT_CONFIG(shortcut)
        actionExit->setText(QCoreApplication::translate("MainWindow", "Exit", nullptr));
#if QT_CONFIG(shortcut)
        actionExit->setShortcut(QCoreApplication::translate("MainWindow", "Ctrl+Q", nullptr));
#endif // QT_CONFIG(shortcut)
        actionAbout->setText(QCoreApplication::translate("MainWindow", "About...", nullptr));
        actionAboutQt->setText(QCoreApplication::translate("MainWindow", "About Qt...", nullptr));
        groupEngine->setTitle(QCoreApplication::translate("MainWindow", "Engine Selection", nullptr));
        lblEngineInfo->setText(QCoreApplication::translate("MainWindow", "Select preset engine or custom parameters", nullptr));
        lblEngineInfo->setStyleSheet(QCoreApplication::translate("MainWindow", "color: #5a6a7a; font-style: italic; font-size: 11px;", nullptr));
        groupPropellant->setTitle(QCoreApplication::translate("MainWindow", "Propellant Parameters", nullptr));
        lblMixtureRatio->setText(QCoreApplication::translate("MainWindow", "Mixture Ratio (O/F):", nullptr));
        lblOxidizer->setText(QCoreApplication::translate("MainWindow", "Oxidizer Mass Frac:", nullptr));
        editOxidizerFrac->setText(QCoreApplication::translate("MainWindow", "0.8571", nullptr));
        lblFuel->setText(QCoreApplication::translate("MainWindow", "Fuel Mass Frac:", nullptr));
        editFuelFrac->setText(QCoreApplication::translate("MainWindow", "0.1429", nullptr));
        lblInitialEnthalpy->setText(QCoreApplication::translate("MainWindow", "Initial Enthalpy (kJ/kg):", nullptr));
        groupChamber->setTitle(QCoreApplication::translate("MainWindow", "Chamber Parameters", nullptr));
        lblChamberPressure->setText(QCoreApplication::translate("MainWindow", "Chamber Pressure (MPa):", nullptr));
        lblTmin->setText(QCoreApplication::translate("MainWindow", "Min Scan Temp (K):", nullptr));
        lblTmax->setText(QCoreApplication::translate("MainWindow", "Max Scan Temp (K):", nullptr));
        groupNozzle->setTitle(QCoreApplication::translate("MainWindow", "Nozzle Parameters", nullptr));
        lblExitPressure->setText(QCoreApplication::translate("MainWindow", "Exit Pressure (atm):", nullptr));
        lblAmbientPressure->setText(QCoreApplication::translate("MainWindow", "Ambient Pressure (atm):", nullptr));
        btnCalculate->setText(QCoreApplication::translate("MainWindow", "CALCULATE", nullptr));
        btnExport->setText(QCoreApplication::translate("MainWindow", "Export", nullptr));
        btnClear->setText(QCoreApplication::translate("MainWindow", "Clear", nullptr));
        QTableWidgetItem *___qtablewidgetitem = tableChamber->horizontalHeaderItem(0);
        ___qtablewidgetitem->setText(QCoreApplication::translate("MainWindow", "Parameter", nullptr));
        QTableWidgetItem *___qtablewidgetitem1 = tableChamber->horizontalHeaderItem(1);
        ___qtablewidgetitem1->setText(QCoreApplication::translate("MainWindow", "Value", nullptr));
        tabResults->setTabText(tabResults->indexOf(tabChamber), QCoreApplication::translate("MainWindow", "Chamber", nullptr));
        QTableWidgetItem *___qtablewidgetitem2 = tableNozzle->horizontalHeaderItem(0);
        ___qtablewidgetitem2->setText(QCoreApplication::translate("MainWindow", "Parameter", nullptr));
        QTableWidgetItem *___qtablewidgetitem3 = tableNozzle->horizontalHeaderItem(1);
        ___qtablewidgetitem3->setText(QCoreApplication::translate("MainWindow", "Value", nullptr));
        tabResults->setTabText(tabResults->indexOf(tabNozzle), QCoreApplication::translate("MainWindow", "Nozzle", nullptr));
        QTableWidgetItem *___qtablewidgetitem4 = tableSpecies->horizontalHeaderItem(0);
        ___qtablewidgetitem4->setText(QCoreApplication::translate("MainWindow", "Species", nullptr));
        QTableWidgetItem *___qtablewidgetitem5 = tableSpecies->horizontalHeaderItem(1);
        ___qtablewidgetitem5->setText(QCoreApplication::translate("MainWindow", "Chamber (mol)", nullptr));
        QTableWidgetItem *___qtablewidgetitem6 = tableSpecies->horizontalHeaderItem(2);
        ___qtablewidgetitem6->setText(QCoreApplication::translate("MainWindow", "Exit (mol)", nullptr));
        tabResults->setTabText(tabResults->indexOf(tabSpecies), QCoreApplication::translate("MainWindow", "Species", nullptr));
        tabResults->setTabText(tabResults->indexOf(tabLog), QCoreApplication::translate("MainWindow", "Log", nullptr));
        menuFile->setTitle(QCoreApplication::translate("MainWindow", "File", nullptr));
        menuHelp->setTitle(QCoreApplication::translate("MainWindow", "Help", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
