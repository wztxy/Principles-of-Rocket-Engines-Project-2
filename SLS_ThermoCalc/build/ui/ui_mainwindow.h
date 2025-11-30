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
    QFrame *titleFrame;
    QVBoxLayout *titleLayout;
    QLabel *lblTitle;
    QLabel *lblSubtitle;
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
    QGroupBox *groupNozzle;
    QGridLayout *nozzleLayout;
    QLabel *lblExitPressure;
    QDoubleSpinBox *spinExitPressure;
    QFrame *buttonFrame;
    QVBoxLayout *buttonLayout;
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
        MainWindow->resize(1400, 900);
        MainWindow->setMinimumSize(QSize(1200, 700));
        MainWindow->setStyleSheet(QString::fromUtf8("\n"
"/* ========================================\n"
"   \347\216\260\344\273\243\347\247\221\346\212\200\351\243\216\346\240\274\344\270\273\351\242\230 - \346\270\220\345\217\230\346\267\261\350\223\235\350\211\262\347\263\273\n"
"   ======================================== */\n"
"\n"
"/* \345\205\250\345\261\200\345\255\227\344\275\223\345\222\214\350\203\214\346\231\257 */\n"
"* {\n"
"    font-family: \"SF Pro Display\", \"PingFang SC\", \"Microsoft YaHei\", sans-serif;\n"
"}\n"
"\n"
"QMainWindow {\n"
"    background: qlineargradient(x1:0, y1:0, x2:1, y2:1,\n"
"                stop:0 #0a0e17, stop:0.5 #0d1321, stop:1 #0a1628);\n"
"}\n"
"\n"
"/* ===== \345\210\206\347\273\204\346\241\206\346\240\267\345\274\217 ===== */\n"
"QGroupBox {\n"
"    font-weight: 600;\n"
"    font-size: 13px;\n"
"    border: 1px solid rgba(0, 180, 216, 0.3);\n"
"    border-radius: 12px;\n"
"    margin-top: 16px;\n"
"    padding: 16px 12px 12px 12px;\n"
"    background: qlineargradient(x1:0, y1:0, x2:0, y2:1,\n"
"                sto"
                        "p:0 rgba(13, 21, 32, 0.95), stop:1 rgba(10, 18, 28, 0.98));\n"
"    color: #e0e6ed;\n"
"}\n"
"QGroupBox::title {\n"
"    subcontrol-origin: margin;\n"
"    subcontrol-position: top left;\n"
"    left: 16px;\n"
"    top: 2px;\n"
"    padding: 2px 12px;\n"
"    background: qlineargradient(x1:0, y1:0, x2:1, y2:0,\n"
"                stop:0 #00b4d8, stop:1 #0096c7);\n"
"    border-radius: 8px;\n"
"    color: #ffffff;\n"
"    font-weight: bold;\n"
"    font-size: 12px;\n"
"}\n"
"\n"
"/* ===== \346\240\207\347\255\276\346\240\267\345\274\217 ===== */\n"
"QLabel {\n"
"    color: #a8b8c8;\n"
"    font-size: 12px;\n"
"    font-weight: 500;\n"
"}\n"
"QLabel#lblEngineInfo {\n"
"    color: #5a7a9a;\n"
"    font-style: italic;\n"
"    font-size: 11px;\n"
"    padding: 4px 0;\n"
"}\n"
"QLabel#lblTitle {\n"
"    color: #00b4d8;\n"
"    font-size: 18px;\n"
"    font-weight: bold;\n"
"}\n"
"QLabel#lblSubtitle {\n"
"    color: #5a7a9a;\n"
"    font-size: 11px;\n"
"}\n"
"\n"
"/* ===== \350\276\223\345\205\245\346\216\247\344\273"
                        "\266\346\240\267\345\274\217 ===== */\n"
"QLineEdit, QDoubleSpinBox, QSpinBox {\n"
"    background: qlineargradient(x1:0, y1:0, x2:0, y2:1,\n"
"                stop:0 #0f1926, stop:1 #0a1320);\n"
"    border: 1px solid rgba(30, 58, 95, 0.8);\n"
"    border-radius: 8px;\n"
"    padding: 8px 12px;\n"
"    color: #e0e6ed;\n"
"    font-size: 13px;\n"
"    font-weight: 500;\n"
"    selection-background-color: #00b4d8;\n"
"}\n"
"QLineEdit:focus, QDoubleSpinBox:focus, QSpinBox:focus {\n"
"    border: 2px solid #00b4d8;\n"
"    background: qlineargradient(x1:0, y1:0, x2:0, y2:1,\n"
"                stop:0 #121f2e, stop:1 #0d1825);\n"
"}\n"
"QLineEdit:read-only {\n"
"    background: rgba(30, 58, 95, 0.3);\n"
"    color: #7a9ab8;\n"
"}\n"
"QDoubleSpinBox::up-button, QSpinBox::up-button,\n"
"QDoubleSpinBox::down-button, QSpinBox::down-button {\n"
"    width: 20px;\n"
"    border: none;\n"
"    background: transparent;\n"
"}\n"
"QDoubleSpinBox::up-arrow, QSpinBox::up-arrow {\n"
"    image: none;\n"
"    border-left: 4px s"
                        "olid transparent;\n"
"    border-right: 4px solid transparent;\n"
"    border-bottom: 5px solid #00b4d8;\n"
"}\n"
"QDoubleSpinBox::down-arrow, QSpinBox::down-arrow {\n"
"    image: none;\n"
"    border-left: 4px solid transparent;\n"
"    border-right: 4px solid transparent;\n"
"    border-top: 5px solid #00b4d8;\n"
"}\n"
"\n"
"/* ===== \344\270\213\346\213\211\346\241\206\346\240\267\345\274\217 ===== */\n"
"QComboBox {\n"
"    background: qlineargradient(x1:0, y1:0, x2:0, y2:1,\n"
"                stop:0 #0f1926, stop:1 #0a1320);\n"
"    border: 1px solid rgba(30, 58, 95, 0.8);\n"
"    border-radius: 8px;\n"
"    padding: 10px 14px;\n"
"    color: #e0e6ed;\n"
"    font-size: 13px;\n"
"    font-weight: 500;\n"
"    min-height: 20px;\n"
"}\n"
"QComboBox:hover {\n"
"    border: 1px solid rgba(0, 180, 216, 0.6);\n"
"}\n"
"QComboBox:focus {\n"
"    border: 2px solid #00b4d8;\n"
"}\n"
"QComboBox::drop-down {\n"
"    border: none;\n"
"    width: 30px;\n"
"    background: transparent;\n"
"}\n"
"QComboBox::down-arrow"
                        " {\n"
"    image: none;\n"
"    border-left: 6px solid transparent;\n"
"    border-right: 6px solid transparent;\n"
"    border-top: 7px solid #00b4d8;\n"
"}\n"
"QComboBox QAbstractItemView {\n"
"    background-color: #0d1520;\n"
"    border: 1px solid #1e3a5f;\n"
"    border-radius: 8px;\n"
"    selection-background-color: rgba(0, 180, 216, 0.3);\n"
"    color: #e0e6ed;\n"
"    padding: 4px;\n"
"    outline: none;\n"
"}\n"
"QComboBox QAbstractItemView::item {\n"
"    padding: 8px 12px;\n"
"    border-radius: 4px;\n"
"    min-height: 24px;\n"
"}\n"
"QComboBox QAbstractItemView::item:hover {\n"
"    background-color: rgba(0, 180, 216, 0.2);\n"
"}\n"
"\n"
"/* ===== \346\214\211\351\222\256\346\240\267\345\274\217 ===== */\n"
"QPushButton {\n"
"    background: qlineargradient(x1:0, y1:0, x2:0, y2:1,\n"
"                stop:0 #00c4e8, stop:1 #0096c7);\n"
"    color: #ffffff;\n"
"    border: none;\n"
"    border-radius: 10px;\n"
"    padding: 12px 28px;\n"
"    font-weight: bold;\n"
"    font-size: 14px;\n"
"    m"
                        "in-height: 20px;\n"
"}\n"
"QPushButton:hover {\n"
"    background: qlineargradient(x1:0, y1:0, x2:0, y2:1,\n"
"                stop:0 #48daf4, stop:1 #00b4d8);\n"
"}\n"
"QPushButton:pressed {\n"
"    background: qlineargradient(x1:0, y1:0, x2:0, y2:1,\n"
"                stop:0 #0096c7, stop:1 #007ba7);\n"
"}\n"
"QPushButton:disabled {\n"
"    background: rgba(30, 58, 95, 0.5);\n"
"    color: #4a5a6a;\n"
"}\n"
"QPushButton#btnCalculate {\n"
"    background: qlineargradient(x1:0, y1:0, x2:1, y2:0,\n"
"                stop:0 #00b4d8, stop:0.5 #00c4e8, stop:1 #00d4f8);\n"
"    font-size: 15px;\n"
"    padding: 14px 32px;\n"
"    border-radius: 12px;\n"
"    min-height: 24px;\n"
"}\n"
"QPushButton#btnCalculate:hover {\n"
"    background: qlineargradient(x1:0, y1:0, x2:1, y2:0,\n"
"                stop:0 #00c4e8, stop:0.5 #00d4f8, stop:1 #00e4ff);\n"
"}\n"
"QPushButton#btnClear {\n"
"    background: qlineargradient(x1:0, y1:0, x2:0, y2:1,\n"
"                stop:0 #f06c77, stop:1 #e63946);\n"
"}\n"
"QPushButton#bt"
                        "nClear:hover {\n"
"    background: qlineargradient(x1:0, y1:0, x2:0, y2:1,\n"
"                stop:0 #ff7c87, stop:1 #f06c77);\n"
"}\n"
"QPushButton#btnExport {\n"
"    background: qlineargradient(x1:0, y1:0, x2:0, y2:1,\n"
"                stop:0 #40c8b8, stop:1 #2a9d8f);\n"
"}\n"
"QPushButton#btnExport:hover {\n"
"    background: qlineargradient(x1:0, y1:0, x2:0, y2:1,\n"
"                stop:0 #50d8c8, stop:1 #40c8b8);\n"
"}\n"
"\n"
"/* ===== \350\241\250\346\240\274\346\240\267\345\274\217 - \346\270\205\346\231\260\346\230\223\350\257\273 ===== */\n"
"QTableWidget {\n"
"    background-color: rgba(10, 16, 26, 0.95);\n"
"    alternate-background-color: rgba(15, 24, 38, 0.95);\n"
"    border: 1px solid rgba(30, 58, 95, 0.5);\n"
"    border-radius: 8px;\n"
"    gridline-color: rgba(30, 58, 95, 0.3);\n"
"    color: #e0e6ed;\n"
"    font-size: 13px;\n"
"    selection-background-color: rgba(0, 180, 216, 0.2);\n"
"    outline: none;\n"
"}\n"
"QTableWidget::item {\n"
"    padding: 12px 16px;\n"
"    border-botto"
                        "m: 1px solid rgba(30, 58, 95, 0.25);\n"
"    min-height: 28px;\n"
"}\n"
"QTableWidget::item:selected {\n"
"    background-color: rgba(0, 180, 216, 0.15);\n"
"    color: #ffffff;\n"
"}\n"
"QTableWidget::item:hover:!selected {\n"
"    background-color: rgba(0, 180, 216, 0.08);\n"
"}\n"
"QHeaderView {\n"
"    background: transparent;\n"
"}\n"
"QHeaderView::section {\n"
"    background: qlineargradient(x1:0, y1:0, x2:0, y2:1,\n"
"                stop:0 rgba(20, 35, 55, 0.98), stop:1 rgba(15, 28, 45, 0.98));\n"
"    color: #00d4f8;\n"
"    font-weight: bold;\n"
"    font-size: 12px;\n"
"    padding: 12px 16px;\n"
"    border: none;\n"
"    border-bottom: 2px solid rgba(0, 180, 216, 0.5);\n"
"}\n"
"QHeaderView::section:first {\n"
"    border-top-left-radius: 8px;\n"
"}\n"
"QHeaderView::section:last {\n"
"    border-top-right-radius: 8px;\n"
"}\n"
"\n"
"/* ===== \346\226\207\346\234\254\347\274\226\350\276\221\346\241\206\346\240\267\345\274\217 ===== */\n"
"QTextEdit {\n"
"    background-color: rgba(13, 21, 32, 0.95"
                        ");\n"
"    border: 1px solid rgba(30, 58, 95, 0.6);\n"
"    border-radius: 10px;\n"
"    color: #90caf9;\n"
"    font-family: \"SF Mono\", \"Monaco\", \"Menlo\", \"Consolas\", monospace;\n"
"    font-size: 11px;\n"
"    padding: 8px;\n"
"    selection-background-color: rgba(0, 180, 216, 0.3);\n"
"}\n"
"\n"
"/* ===== \347\212\266\346\200\201\346\240\217\346\240\267\345\274\217 ===== */\n"
"QStatusBar {\n"
"    background: qlineargradient(x1:0, y1:0, x2:0, y2:1,\n"
"                stop:0 #0d1520, stop:1 #0a1218);\n"
"    color: #5a7a9a;\n"
"    border-top: 1px solid rgba(30, 58, 95, 0.5);\n"
"    padding: 4px 12px;\n"
"    font-size: 11px;\n"
"}\n"
"\n"
"/* ===== \350\217\234\345\215\225\346\240\217\346\240\267\345\274\217 ===== */\n"
"QMenuBar {\n"
"    background: qlineargradient(x1:0, y1:0, x2:0, y2:1,\n"
"                stop:0 #101824, stop:1 #0d1520);\n"
"    color: #b8c5d3;\n"
"    border-bottom: 1px solid rgba(30, 58, 95, 0.5);\n"
"    padding: 4px 8px;\n"
"    spacing: 4px;\n"
"}\n"
"QMenuBar::item {\n"
""
                        "    padding: 8px 16px;\n"
"    border-radius: 6px;\n"
"    font-weight: 500;\n"
"}\n"
"QMenuBar::item:selected {\n"
"    background: rgba(0, 180, 216, 0.2);\n"
"    color: #00b4d8;\n"
"}\n"
"QMenu {\n"
"    background-color: rgba(13, 21, 32, 0.98);\n"
"    color: #e0e6ed;\n"
"    border: 1px solid rgba(30, 58, 95, 0.8);\n"
"    border-radius: 10px;\n"
"    padding: 8px;\n"
"}\n"
"QMenu::item {\n"
"    padding: 10px 28px 10px 16px;\n"
"    border-radius: 6px;\n"
"    margin: 2px 4px;\n"
"}\n"
"QMenu::item:selected {\n"
"    background: rgba(0, 180, 216, 0.25);\n"
"    color: #00b4d8;\n"
"}\n"
"QMenu::separator {\n"
"    height: 1px;\n"
"    background: rgba(30, 58, 95, 0.6);\n"
"    margin: 6px 12px;\n"
"}\n"
"\n"
"/* ===== \351\200\211\351\241\271\345\215\241\346\240\267\345\274\217 ===== */\n"
"QTabWidget::pane {\n"
"    border: 1px solid rgba(30, 58, 95, 0.6);\n"
"    border-radius: 12px;\n"
"    background: rgba(13, 21, 32, 0.95);\n"
"    padding: 8px;\n"
"    top: -1px;\n"
"}\n"
"QTabBar::tab {\n"
"    bac"
                        "kground: rgba(13, 21, 32, 0.8);\n"
"    color: #6a8aa8;\n"
"    padding: 12px 24px;\n"
"    border: 1px solid rgba(30, 58, 95, 0.5);\n"
"    border-bottom: none;\n"
"    border-top-left-radius: 10px;\n"
"    border-top-right-radius: 10px;\n"
"    margin-right: 4px;\n"
"    font-weight: 500;\n"
"    font-size: 12px;\n"
"}\n"
"QTabBar::tab:selected {\n"
"    background: qlineargradient(x1:0, y1:0, x2:0, y2:1,\n"
"                stop:0 #1a2a40, stop:1 #152535);\n"
"    color: #00b4d8;\n"
"    border-color: rgba(0, 180, 216, 0.5);\n"
"    border-bottom: 3px solid #00b4d8;\n"
"}\n"
"QTabBar::tab:hover:!selected {\n"
"    background: rgba(30, 58, 95, 0.4);\n"
"    color: #8aaac8;\n"
"}\n"
"\n"
"/* ===== \346\273\232\345\212\250\346\235\241\346\240\267\345\274\217 ===== */\n"
"QScrollBar:vertical {\n"
"    background: rgba(13, 21, 32, 0.5);\n"
"    width: 10px;\n"
"    border-radius: 5px;\n"
"    margin: 2px;\n"
"}\n"
"QScrollBar::handle:vertical {\n"
"    background: qlineargradient(x1:0, y1:0, x2:1, y2:0,\n"
"    "
                        "            stop:0 #2e4a6f, stop:1 #1e3a5f);\n"
"    border-radius: 4px;\n"
"    min-height: 40px;\n"
"}\n"
"QScrollBar::handle:vertical:hover {\n"
"    background: qlineargradient(x1:0, y1:0, x2:1, y2:0,\n"
"                stop:0 #3e5a7f, stop:1 #2e4a6f);\n"
"}\n"
"QScrollBar::add-line:vertical, QScrollBar::sub-line:vertical {\n"
"    height: 0px;\n"
"}\n"
"QScrollBar::add-page:vertical, QScrollBar::sub-page:vertical {\n"
"    background: transparent;\n"
"}\n"
"QScrollBar:horizontal {\n"
"    background: rgba(13, 21, 32, 0.5);\n"
"    height: 10px;\n"
"    border-radius: 5px;\n"
"    margin: 2px;\n"
"}\n"
"QScrollBar::handle:horizontal {\n"
"    background: qlineargradient(x1:0, y1:0, x2:0, y2:1,\n"
"                stop:0 #2e4a6f, stop:1 #1e3a5f);\n"
"    border-radius: 4px;\n"
"    min-width: 40px;\n"
"}\n"
"\n"
"/* ===== \345\267\245\345\205\267\346\217\220\347\244\272\346\240\267\345\274\217 ===== */\n"
"QToolTip {\n"
"    background-color: rgba(13, 21, 32, 0.98);\n"
"    color: #e0e6ed;\n"
"    border: "
                        "1px solid rgba(0, 180, 216, 0.5);\n"
"    border-radius: 6px;\n"
"    padding: 8px 12px;\n"
"    font-size: 11px;\n"
"}\n"
"\n"
"/* ===== \350\277\233\345\272\246\346\214\207\347\244\272\345\231\250\346\240\267\345\274\217 ===== */\n"
"QProgressBar {\n"
"    background: rgba(30, 58, 95, 0.3);\n"
"    border: none;\n"
"    border-radius: 6px;\n"
"    height: 8px;\n"
"    text-align: center;\n"
"}\n"
"QProgressBar::chunk {\n"
"    background: qlineargradient(x1:0, y1:0, x2:1, y2:0,\n"
"                stop:0 #00b4d8, stop:1 #00d4f8);\n"
"    border-radius: 6px;\n"
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
        horizontalLayout->setSpacing(20);
        horizontalLayout->setObjectName("horizontalLayout");
        horizontalLayout->setContentsMargins(20, 16, 20, 16);
        inputPanel = new QFrame(centralwidget);
        inputPanel->setObjectName("inputPanel");
        inputPanel->setMinimumSize(QSize(420, 0));
        inputPanel->setMaximumSize(QSize(480, 16777215));
        inputPanel->setFrameShape(QFrame::NoFrame);
        inputLayout = new QVBoxLayout(inputPanel);
        inputLayout->setSpacing(16);
        inputLayout->setObjectName("inputLayout");
        inputLayout->setContentsMargins(0, 0, 0, 0);
        titleFrame = new QFrame(inputPanel);
        titleFrame->setObjectName("titleFrame");
        titleLayout = new QVBoxLayout(titleFrame);
        titleLayout->setSpacing(4);
        titleLayout->setObjectName("titleLayout");
        titleLayout->setContentsMargins(16, 12, 16, 12);
        lblTitle = new QLabel(titleFrame);
        lblTitle->setObjectName("lblTitle");

        titleLayout->addWidget(lblTitle);

        lblSubtitle = new QLabel(titleFrame);
        lblSubtitle->setObjectName("lblSubtitle");

        titleLayout->addWidget(lblSubtitle);


        inputLayout->addWidget(titleFrame);

        groupEngine = new QGroupBox(inputPanel);
        groupEngine->setObjectName("groupEngine");
        engineLayout = new QVBoxLayout(groupEngine);
        engineLayout->setSpacing(8);
        engineLayout->setObjectName("engineLayout");
        comboEngine = new QComboBox(groupEngine);
        comboEngine->setObjectName("comboEngine");
        comboEngine->setMinimumSize(QSize(0, 40));

        engineLayout->addWidget(comboEngine);

        lblEngineInfo = new QLabel(groupEngine);
        lblEngineInfo->setObjectName("lblEngineInfo");
        lblEngineInfo->setWordWrap(true);

        engineLayout->addWidget(lblEngineInfo);


        inputLayout->addWidget(groupEngine);

        groupPropellant = new QGroupBox(inputPanel);
        groupPropellant->setObjectName("groupPropellant");
        propellantLayout = new QGridLayout(groupPropellant);
        propellantLayout->setObjectName("propellantLayout");
        propellantLayout->setHorizontalSpacing(12);
        propellantLayout->setVerticalSpacing(14);
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
        spinInitialEnthalpy->setValue(-1210.920000000000073);
        spinInitialEnthalpy->setDecimals(2);

        propellantLayout->addWidget(spinInitialEnthalpy, 3, 1, 1, 1);


        inputLayout->addWidget(groupPropellant);

        groupChamber = new QGroupBox(inputPanel);
        groupChamber->setObjectName("groupChamber");
        chamberLayout = new QGridLayout(groupChamber);
        chamberLayout->setObjectName("chamberLayout");
        chamberLayout->setHorizontalSpacing(12);
        chamberLayout->setVerticalSpacing(10);
        lblChamberPressure = new QLabel(groupChamber);
        lblChamberPressure->setObjectName("lblChamberPressure");

        chamberLayout->addWidget(lblChamberPressure, 0, 0, 1, 1);

        spinChamberPressure = new QDoubleSpinBox(groupChamber);
        spinChamberPressure->setObjectName("spinChamberPressure");
        spinChamberPressure->setMinimum(0.100000000000000);
        spinChamberPressure->setMaximum(50.000000000000000);
        spinChamberPressure->setSingleStep(0.500000000000000);
        spinChamberPressure->setValue(20.469999999999999);
        spinChamberPressure->setDecimals(2);

        chamberLayout->addWidget(spinChamberPressure, 0, 1, 1, 1);


        inputLayout->addWidget(groupChamber);

        groupNozzle = new QGroupBox(inputPanel);
        groupNozzle->setObjectName("groupNozzle");
        nozzleLayout = new QGridLayout(groupNozzle);
        nozzleLayout->setObjectName("nozzleLayout");
        nozzleLayout->setHorizontalSpacing(12);
        nozzleLayout->setVerticalSpacing(10);
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


        inputLayout->addWidget(groupNozzle);

        buttonFrame = new QFrame(inputPanel);
        buttonFrame->setObjectName("buttonFrame");
        buttonLayout = new QVBoxLayout(buttonFrame);
        buttonLayout->setSpacing(10);
        buttonLayout->setObjectName("buttonLayout");
        btnCalculate = new QPushButton(buttonFrame);
        btnCalculate->setObjectName("btnCalculate");
        btnCalculate->setMinimumSize(QSize(0, 48));

        buttonLayout->addWidget(btnCalculate);

        buttonLayout2 = new QHBoxLayout();
        buttonLayout2->setSpacing(10);
        buttonLayout2->setObjectName("buttonLayout2");
        btnExport = new QPushButton(buttonFrame);
        btnExport->setObjectName("btnExport");
        btnExport->setMinimumSize(QSize(0, 38));

        buttonLayout2->addWidget(btnExport);

        btnClear = new QPushButton(buttonFrame);
        btnClear->setObjectName("btnClear");
        btnClear->setMinimumSize(QSize(0, 38));

        buttonLayout2->addWidget(btnClear);


        buttonLayout->addLayout(buttonLayout2);


        inputLayout->addWidget(buttonFrame);

        verticalSpacer = new QSpacerItem(20, 20, QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Expanding);

        inputLayout->addItem(verticalSpacer);


        horizontalLayout->addWidget(inputPanel);

        tabResults = new QTabWidget(centralwidget);
        tabResults->setObjectName("tabResults");
        tabResults->setDocumentMode(true);
        tabChamber = new QWidget();
        tabChamber->setObjectName("tabChamber");
        chamberResultLayout = new QVBoxLayout(tabChamber);
        chamberResultLayout->setSpacing(4);
        chamberResultLayout->setObjectName("chamberResultLayout");
        chamberResultLayout->setContentsMargins(12, 16, 12, 12);
        tableChamber = new QTableWidget(tabChamber);
        if (tableChamber->columnCount() < 2)
            tableChamber->setColumnCount(2);
        QTableWidgetItem *__qtablewidgetitem = new QTableWidgetItem();
        tableChamber->setHorizontalHeaderItem(0, __qtablewidgetitem);
        QTableWidgetItem *__qtablewidgetitem1 = new QTableWidgetItem();
        tableChamber->setHorizontalHeaderItem(1, __qtablewidgetitem1);
        tableChamber->setObjectName("tableChamber");
        tableChamber->setColumnCount(2);
        tableChamber->setRowCount(19);
        tableChamber->setAlternatingRowColors(true);
        tableChamber->setShowGrid(false);
        tableChamber->setWordWrap(false);
        tableChamber->setSelectionMode(QAbstractItemView::SingleSelection);
        tableChamber->setSelectionBehavior(QAbstractItemView::SelectRows);

        chamberResultLayout->addWidget(tableChamber);

        tabResults->addTab(tabChamber, QString());
        tabNozzle = new QWidget();
        tabNozzle->setObjectName("tabNozzle");
        nozzleResultLayout = new QVBoxLayout(tabNozzle);
        nozzleResultLayout->setSpacing(4);
        nozzleResultLayout->setObjectName("nozzleResultLayout");
        nozzleResultLayout->setContentsMargins(12, 16, 12, 12);
        tableNozzle = new QTableWidget(tabNozzle);
        if (tableNozzle->columnCount() < 2)
            tableNozzle->setColumnCount(2);
        QTableWidgetItem *__qtablewidgetitem2 = new QTableWidgetItem();
        tableNozzle->setHorizontalHeaderItem(0, __qtablewidgetitem2);
        QTableWidgetItem *__qtablewidgetitem3 = new QTableWidgetItem();
        tableNozzle->setHorizontalHeaderItem(1, __qtablewidgetitem3);
        tableNozzle->setObjectName("tableNozzle");
        tableNozzle->setColumnCount(2);
        tableNozzle->setRowCount(16);
        tableNozzle->setAlternatingRowColors(true);
        tableNozzle->setShowGrid(false);
        tableNozzle->setSelectionMode(QAbstractItemView::SingleSelection);
        tableNozzle->setSelectionBehavior(QAbstractItemView::SelectRows);

        nozzleResultLayout->addWidget(tableNozzle);

        tabResults->addTab(tabNozzle, QString());
        tabSpecies = new QWidget();
        tabSpecies->setObjectName("tabSpecies");
        speciesLayout = new QVBoxLayout(tabSpecies);
        speciesLayout->setSpacing(4);
        speciesLayout->setObjectName("speciesLayout");
        speciesLayout->setContentsMargins(12, 16, 12, 12);
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
        tableSpecies->setAlternatingRowColors(true);
        tableSpecies->setShowGrid(false);
        tableSpecies->setSelectionMode(QAbstractItemView::SingleSelection);
        tableSpecies->setSelectionBehavior(QAbstractItemView::SelectRows);

        speciesLayout->addWidget(tableSpecies);

        tabResults->addTab(tabSpecies, QString());
        tabLog = new QWidget();
        tabLog->setObjectName("tabLog");
        logLayout = new QVBoxLayout(tabLog);
        logLayout->setSpacing(8);
        logLayout->setObjectName("logLayout");
        logLayout->setContentsMargins(12, 16, 12, 12);
        textLog = new QTextEdit(tabLog);
        textLog->setObjectName("textLog");
        textLog->setReadOnly(true);

        logLayout->addWidget(textLog);

        tabResults->addTab(tabLog, QString());

        horizontalLayout->addWidget(tabResults);

        MainWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(MainWindow);
        menubar->setObjectName("menubar");
        menubar->setGeometry(QRect(0, 0, 1400, 32));
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
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "SLS \347\201\253\347\256\255\345\217\221\345\212\250\346\234\272\347\203\255\345\212\233\350\256\241\347\256\227\347\263\273\347\273\237", nullptr));
        actionImport->setText(QCoreApplication::translate("MainWindow", "\345\257\274\345\205\245\351\242\204\350\256\276...", nullptr));
#if QT_CONFIG(shortcut)
        actionImport->setShortcut(QCoreApplication::translate("MainWindow", "Ctrl+O", nullptr));
#endif // QT_CONFIG(shortcut)
        actionExport->setText(QCoreApplication::translate("MainWindow", "\345\257\274\345\207\272\351\242\204\350\256\276...", nullptr));
#if QT_CONFIG(shortcut)
        actionExport->setShortcut(QCoreApplication::translate("MainWindow", "Ctrl+S", nullptr));
#endif // QT_CONFIG(shortcut)
        actionExportResults->setText(QCoreApplication::translate("MainWindow", "\345\257\274\345\207\272\347\273\223\346\236\234...", nullptr));
#if QT_CONFIG(shortcut)
        actionExportResults->setShortcut(QCoreApplication::translate("MainWindow", "Ctrl+E", nullptr));
#endif // QT_CONFIG(shortcut)
        actionExit->setText(QCoreApplication::translate("MainWindow", "\351\200\200\345\207\272", nullptr));
#if QT_CONFIG(shortcut)
        actionExit->setShortcut(QCoreApplication::translate("MainWindow", "Ctrl+Q", nullptr));
#endif // QT_CONFIG(shortcut)
        actionAbout->setText(QCoreApplication::translate("MainWindow", "\345\205\263\344\272\216...", nullptr));
        actionAboutQt->setText(QCoreApplication::translate("MainWindow", "\345\205\263\344\272\216 Qt...", nullptr));
        titleFrame->setStyleSheet(QCoreApplication::translate("MainWindow", "\n"
"QFrame#titleFrame {\n"
"    background: qlineargradient(x1:0, y1:0, x2:1, y2:1,\n"
"                stop:0 rgba(0, 180, 216, 0.15), stop:1 rgba(0, 150, 199, 0.08));\n"
"    border: 1px solid rgba(0, 180, 216, 0.3);\n"
"    border-radius: 12px;\n"
"    padding: 12px;\n"
"}\n"
"          ", nullptr));
        lblTitle->setText(QCoreApplication::translate("MainWindow", "\347\203\255\345\212\233\350\256\241\347\256\227\347\263\273\347\273\237", nullptr));
        lblTitle->setStyleSheet(QCoreApplication::translate("MainWindow", "color: #00b4d8; font-size: 16px; font-weight: bold;", nullptr));
        lblSubtitle->setText(QCoreApplication::translate("MainWindow", "\346\234\200\345\260\217\345\220\211\345\270\203\346\226\257\350\207\252\347\224\261\350\203\275\346\263\225 \302\267 Minimum Gibbs Free Energy", nullptr));
        lblSubtitle->setStyleSheet(QCoreApplication::translate("MainWindow", "color: #5a7a9a; font-size: 11px; font-style: italic;", nullptr));
        groupEngine->setTitle(QCoreApplication::translate("MainWindow", "\345\217\221\345\212\250\346\234\272\351\200\211\346\213\251", nullptr));
#if QT_CONFIG(tooltip)
        comboEngine->setToolTip(QCoreApplication::translate("MainWindow", "\351\200\211\346\213\251\351\242\204\350\256\276\345\217\221\345\212\250\346\234\272\351\205\215\347\275\256\346\210\226\344\275\277\347\224\250\350\207\252\345\256\232\344\271\211\345\217\202\346\225\260", nullptr));
#endif // QT_CONFIG(tooltip)
        lblEngineInfo->setText(QCoreApplication::translate("MainWindow", "\351\200\211\346\213\251\351\242\204\350\256\276\345\217\221\345\212\250\346\234\272\346\210\226\351\205\215\347\275\256\350\207\252\345\256\232\344\271\211\345\217\202\346\225\260", nullptr));
        groupPropellant->setTitle(QCoreApplication::translate("MainWindow", "\346\216\250\350\277\233\345\211\202\345\217\202\346\225\260", nullptr));
        lblMixtureRatio->setText(QCoreApplication::translate("MainWindow", "\346\267\267\345\220\210\346\257\224 O/F", nullptr));
#if QT_CONFIG(tooltip)
        lblMixtureRatio->setToolTip(QCoreApplication::translate("MainWindow", "\346\260\247\345\214\226\345\211\202\344\270\216\347\207\203\346\226\231\350\264\250\351\207\217\346\257\224", nullptr));
#endif // QT_CONFIG(tooltip)
#if QT_CONFIG(tooltip)
        spinMixtureRatio->setToolTip(QCoreApplication::translate("MainWindow", "\345\205\270\345\236\213\345\200\274: LOX/LH\342\202\202 \342\211\210 6.0, LOX/CH\342\202\204 \342\211\210 3.6", nullptr));
#endif // QT_CONFIG(tooltip)
        lblOxidizer->setText(QCoreApplication::translate("MainWindow", "\346\260\247\345\214\226\345\211\202\350\264\250\351\207\217\345\210\206\346\225\260", nullptr));
        editOxidizerFrac->setText(QCoreApplication::translate("MainWindow", "0.8571", nullptr));
        lblFuel->setText(QCoreApplication::translate("MainWindow", "\347\207\203\346\226\231\350\264\250\351\207\217\345\210\206\346\225\260", nullptr));
        editFuelFrac->setText(QCoreApplication::translate("MainWindow", "0.1429", nullptr));
        lblInitialEnthalpy->setText(QCoreApplication::translate("MainWindow", "\345\210\235\345\247\213\347\204\223  [kJ/kg]", nullptr));
#if QT_CONFIG(tooltip)
        lblInitialEnthalpy->setToolTip(QCoreApplication::translate("MainWindow", "\346\216\250\350\277\233\345\211\202\345\234\250\345\217\202\350\200\203\346\270\251\345\272\246\344\270\213\347\232\204\345\210\235\345\247\213\347\204\223\345\200\274", nullptr));
#endif // QT_CONFIG(tooltip)
        groupChamber->setTitle(QCoreApplication::translate("MainWindow", "\347\207\203\347\203\247\345\256\244\345\217\202\346\225\260", nullptr));
        lblChamberPressure->setText(QCoreApplication::translate("MainWindow", "\347\207\203\347\203\247\345\256\244\345\216\213\345\274\272  [MPa]", nullptr));
#if QT_CONFIG(tooltip)
        lblChamberPressure->setToolTip(QCoreApplication::translate("MainWindow", "\347\207\203\347\203\247\345\256\244\346\200\273\345\216\213", nullptr));
#endif // QT_CONFIG(tooltip)
#if QT_CONFIG(tooltip)
        spinChamberPressure->setToolTip(QCoreApplication::translate("MainWindow", "\345\205\270\345\236\213\345\200\274: RS-25 \342\211\210 20.5 MPa, Raptor \342\211\210 30 MPa", nullptr));
#endif // QT_CONFIG(tooltip)
        groupNozzle->setTitle(QCoreApplication::translate("MainWindow", "\345\226\267\347\256\241\345\217\202\346\225\260", nullptr));
        lblExitPressure->setText(QCoreApplication::translate("MainWindow", "\345\207\272\345\217\243\345\216\213\345\274\272  [atm]", nullptr));
#if QT_CONFIG(tooltip)
        lblExitPressure->setToolTip(QCoreApplication::translate("MainWindow", "\345\226\267\347\256\241\345\207\272\345\217\243\351\235\231\345\216\213", nullptr));
#endif // QT_CONFIG(tooltip)
#if QT_CONFIG(tooltip)
        spinExitPressure->setToolTip(QCoreApplication::translate("MainWindow", "\347\234\237\347\251\272\344\274\230\345\214\226\345\226\267\347\256\241\344\275\277\347\224\250\346\233\264\344\275\216\347\232\204\345\207\272\345\217\243\345\216\213\345\274\272", nullptr));
#endif // QT_CONFIG(tooltip)
        buttonFrame->setStyleSheet(QCoreApplication::translate("MainWindow", "\n"
"QFrame#buttonFrame {\n"
"    background: transparent;\n"
"    padding: 4px 0;\n"
"}\n"
"          ", nullptr));
        btnCalculate->setText(QCoreApplication::translate("MainWindow", "\345\274\200\345\247\213\350\256\241\347\256\227", nullptr));
#if QT_CONFIG(tooltip)
        btnCalculate->setToolTip(QCoreApplication::translate("MainWindow", "\346\211\247\350\241\214\347\203\255\345\212\233\350\256\241\347\256\227 (Ctrl+Enter)", nullptr));
#endif // QT_CONFIG(tooltip)
        btnExport->setText(QCoreApplication::translate("MainWindow", "\345\257\274\345\207\272\347\273\223\346\236\234", nullptr));
#if QT_CONFIG(tooltip)
        btnExport->setToolTip(QCoreApplication::translate("MainWindow", "\345\257\274\345\207\272\350\256\241\347\256\227\347\273\223\346\236\234 (Ctrl+E)", nullptr));
#endif // QT_CONFIG(tooltip)
        btnClear->setText(QCoreApplication::translate("MainWindow", "\346\270\205\347\251\272", nullptr));
#if QT_CONFIG(tooltip)
        btnClear->setToolTip(QCoreApplication::translate("MainWindow", "\346\270\205\347\251\272\346\211\200\346\234\211\347\273\223\346\236\234", nullptr));
#endif // QT_CONFIG(tooltip)
        QTableWidgetItem *___qtablewidgetitem = tableChamber->horizontalHeaderItem(0);
        ___qtablewidgetitem->setText(QCoreApplication::translate("MainWindow", "\345\217\202\346\225\260", nullptr));
        QTableWidgetItem *___qtablewidgetitem1 = tableChamber->horizontalHeaderItem(1);
        ___qtablewidgetitem1->setText(QCoreApplication::translate("MainWindow", "\346\225\260\345\200\274", nullptr));
        tabResults->setTabText(tabResults->indexOf(tabChamber), QCoreApplication::translate("MainWindow", "\347\207\203\347\203\247\345\256\244", nullptr));
        QTableWidgetItem *___qtablewidgetitem2 = tableNozzle->horizontalHeaderItem(0);
        ___qtablewidgetitem2->setText(QCoreApplication::translate("MainWindow", "\345\217\202\346\225\260", nullptr));
        QTableWidgetItem *___qtablewidgetitem3 = tableNozzle->horizontalHeaderItem(1);
        ___qtablewidgetitem3->setText(QCoreApplication::translate("MainWindow", "\346\225\260\345\200\274", nullptr));
        tabResults->setTabText(tabResults->indexOf(tabNozzle), QCoreApplication::translate("MainWindow", "\345\226\267\347\256\241", nullptr));
        QTableWidgetItem *___qtablewidgetitem4 = tableSpecies->horizontalHeaderItem(0);
        ___qtablewidgetitem4->setText(QCoreApplication::translate("MainWindow", "\347\273\204\345\210\206", nullptr));
        QTableWidgetItem *___qtablewidgetitem5 = tableSpecies->horizontalHeaderItem(1);
        ___qtablewidgetitem5->setText(QCoreApplication::translate("MainWindow", "\347\207\203\347\203\247\345\256\244 [mol/kg]", nullptr));
        QTableWidgetItem *___qtablewidgetitem6 = tableSpecies->horizontalHeaderItem(2);
        ___qtablewidgetitem6->setText(QCoreApplication::translate("MainWindow", "\345\207\272\345\217\243 [mol/kg]", nullptr));
        tabResults->setTabText(tabResults->indexOf(tabSpecies), QCoreApplication::translate("MainWindow", "\347\273\204\345\210\206", nullptr));
        textLog->setPlaceholderText(QCoreApplication::translate("MainWindow", "\350\256\241\347\256\227\346\227\245\345\277\227\345\260\206\346\230\276\347\244\272\345\234\250\350\277\231\351\207\214...", nullptr));
        tabResults->setTabText(tabResults->indexOf(tabLog), QCoreApplication::translate("MainWindow", "\346\227\245\345\277\227", nullptr));
        menuFile->setTitle(QCoreApplication::translate("MainWindow", "\346\226\207\344\273\266", nullptr));
        menuHelp->setTitle(QCoreApplication::translate("MainWindow", "\345\270\256\345\212\251", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
