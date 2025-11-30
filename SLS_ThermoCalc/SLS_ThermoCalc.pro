QT       += core gui widgets charts

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# 项目名称
TARGET = SLS_ThermoCalc
TEMPLATE = app

# 源文件
SOURCES += \
    src/main.cpp \
    src/mainwindow.cpp \
    src/thermo_core.c \
    src/propellant_db.c \
    src/matrix_solver.c

# 头文件
HEADERS += \
    include/mainwindow.h \
    include/thermo_core.h \
    include/propellant_db.h \
    include/matrix_solver.h \
    include/constants.h

# UI文件
FORMS += \
    ui/mainwindow.ui

# 资源文件
RESOURCES += \
    resources/resources.qrc

# 包含路径
INCLUDEPATH += include

# 输出目录
DESTDIR = $$PWD/bin
OBJECTS_DIR = $$PWD/build/obj
MOC_DIR = $$PWD/build/moc
UI_DIR = $$PWD/build/ui
RCC_DIR = $$PWD/build/rcc

# 默认规则
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
